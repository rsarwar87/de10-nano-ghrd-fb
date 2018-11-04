#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/poll.h>
#include <linux/sched.h>
#include <linux/stat.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/miscdevice.h>
#include <linux/platform_device.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/of.h>
#include <linux/of_irq.h>

#define FPGAINT_IRQ_FIFO_SIZE       2
#define FPGAINT_IRQ_FIFO_EMPTY(pdev) \
            ((pdev)->irq_fifo_head == (pdev)->irq_fifo_tail)
#define FPGAINT_IRQ_FIFO_FULL(pdev)	\
            ((((pdev)->irq_fifo_tail + 1) % FPGAINT_IRQ_FIFO_SIZE) == \
					(pdev)->irq_fifo_head)

/* default module parameters */
#define LONGNAME "FPGA Interrupt Driver"
#define FPGAINT_IRQ0_NUM       72
#define FPGAINT_IRQ1_NUM       73
#define FPGAINT_MAX_DEVS       16

void *hps_led_mem;
bool on = false;

MODULE_AUTHOR("Chris Knight <crknight@qti.qualcomm.com>");
MODULE_DESCRIPTION(LONGNAME);
MODULE_LICENSE("Dual BSD/GPL");
MODULE_VERSION("0.1");

/* declarations */
static int fpgaint_open(struct inode *inode, struct file *file);
static int fpgaint_release(struct inode *inode, struct file *file);
static ssize_t fpgaint_read(struct file *filp, char __user *buf, size_t count,
        loff_t *f_pos);
static ssize_t fpgaint_write(struct file *file, const char __user *buffer,
        size_t count, loff_t *ppos);
static unsigned int fpgaint_poll(struct file *filp, 
        struct poll_table_struct *wait);
static irqreturn_t fpgaint_irq_handler(int irq, void *data);

/* our file operations structure. this will be pointed to by
 * every device as they are all the same */
static const struct file_operations fpgaint_fops = {
    .owner = THIS_MODULE,
    .open = fpgaint_open,
    .release = fpgaint_release,
    .read = fpgaint_read,
    .write = fpgaint_write,
    .poll = fpgaint_poll,
};

/* our device information structure. one of these exists for
 * each FPGA interrupt device created. */
struct fpgaint_dev {
    spinlock_t          lock;
    char                irq_fifo[FPGAINT_IRQ_FIFO_SIZE];
    unsigned int        irq_fifo_head, irq_fifo_tail;
    wait_queue_head_t   wq;
    unsigned int        irq;
    const char          *name;
    struct device_node  *nodep;
    struct miscdevice   misc_dev;
};

/* we hold onto all the pointers to fpgaint_dev structures so
 * we can find them again when the user calls open on a device */
struct fpgaint_dev_records {
    spinlock_t lock;
    struct fpgaint_dev * devs[FPGAINT_MAX_DEVS];
    unsigned int num_devs;
};

static struct fpgaint_dev_records fpgaint_private = {
    .num_devs = 0,
};

static int fpgaint_open(struct inode *inode, struct file *filp)
{
    struct fpgaint_dev *our_dev;
    unsigned int minor, dev_index;

    printk(KERN_ALERT "FPGA interrupt module: %s called\n", __FUNCTION__);
    
    /* get the minor number opened */
    minor = iminor(inode);

    /* find which of our devices is associated with that minor */
    spin_lock(&fpgaint_private.lock);
    our_dev = NULL;
    for (dev_index = 0; dev_index < FPGAINT_MAX_DEVS; dev_index++) {
        if (fpgaint_private.devs[dev_index] && 
                fpgaint_private.devs[dev_index]->misc_dev.minor == minor) {
            our_dev = fpgaint_private.devs[dev_index];
            break;
        }
    }
    spin_unlock(&fpgaint_private.lock);

    /* store our device into the file pointer for later access */
    filp->private_data = our_dev;

    /* see if we were successful */
    if (!filp->private_data) {
        printk(KERN_ALERT "FPGA interrupt module: failed to open device with minor number %u\n",
                minor);
        return -ENXIO;
    }

    return 0;
}

static int fpgaint_release(struct inode *inode, struct file *filp)
{
    printk(KERN_ALERT "FPGA interrupt module: %s called\n", __FUNCTION__);
    /* nothing to clean up */
    return 0;
}

/* this read function reads the value of the last interrupt received */
static ssize_t fpgaint_read(struct file *filp, char __user *buf, size_t count,
        loff_t *f_pos)
{
	signed char irq = -1;
	unsigned long irq_flags;
    struct fpgaint_dev * our_dev;

    printk(KERN_ALERT "FPGA interrupt module: %s called\n", __FUNCTION__);

    /* get our device */
    our_dev = (struct fpgaint_dev*)filp->private_data;

    /* we want to loop until we get our interrupt */
	do {
        /* protect access to our device structure */
		spin_lock_irqsave(&our_dev->lock, irq_flags);
		if (!FPGAINT_IRQ_FIFO_EMPTY(our_dev)) {
            /* we have an interrupt in our FIFO so pull it out */
			irq = our_dev->irq_fifo[our_dev->irq_fifo_head];
            /* update the fifo head */
			if (++our_dev->irq_fifo_head == FPGAINT_IRQ_FIFO_SIZE)
				our_dev->irq_fifo_head = 0;
		}
        /* end of protected access */
		spin_unlock_irqrestore(&our_dev->lock, irq_flags);

		if (irq < 0)
            /* we didn't get a valid interrupt, wait */
			if (wait_event_interruptible(our_dev->wq,
				!FPGAINT_IRQ_FIFO_EMPTY(our_dev)) < 0)
                /* we were interrupted by something else */
				break;
	} while (irq < 0);

	if (irq < 0) {
		/* No pending interrupt */
		return 0;
	} else {
        /* copy the received interrupt to user space */
		put_user(irq, buf);
        /* we read one byte */
		return 1;
	}

	return 0;
}

/* we can't write to this device */
static ssize_t fpgaint_write(struct file *filp, const char __user *buffer,
        size_t count, loff_t *ppos)
{
    printk(KERN_ALERT "FPGA interrupt module: %s called\n", __FUNCTION__);
    return 0;
}

static unsigned int fpgaint_poll(struct file *filp, 
        struct poll_table_struct *wait)
{
	unsigned mask = 0;
    struct fpgaint_dev * our_dev;

    printk(KERN_ALERT "FPGA interrupt module: %s called\n", __FUNCTION__);

    /* get our device */
    our_dev = (struct fpgaint_dev*)filp->private_data;

	if (!FPGAINT_IRQ_FIFO_EMPTY(our_dev)) {
        /* we have an interrupt in our queue that is ready to be read */
        mask |= POLLIN;
	}

	if (mask == 0) {
        /* no interrupt ready, let's wait for it */
		poll_wait(filp, &our_dev->wq, wait);
		if (!FPGAINT_IRQ_FIFO_EMPTY(our_dev)) {
            mask |= POLLIN;
		}
	}

	return mask;
}

static irqreturn_t fpgaint_irq_handler(int irq, void *data)
{
    struct fpgaint_dev * our_dev;

    printk(KERN_ALERT "FPGA interrupt module: %s called\n", __FUNCTION__);

    /* get our device */
    our_dev = (struct fpgaint_dev*)data;

    /* Adds the pending interrupts to irq_fifo */
    spin_lock(&our_dev->lock);

    if (!FPGAINT_IRQ_FIFO_FULL(our_dev)) {
        our_dev->irq_fifo[our_dev->irq_fifo_tail] = irq;
        if (++our_dev->irq_fifo_tail == FPGAINT_IRQ_FIFO_SIZE)
            our_dev->irq_fifo_tail = 0;
    }
    spin_unlock(&our_dev->lock);

    /* Wakes up pending processes */
    wake_up_interruptible(&our_dev->wq);
    return IRQ_HANDLED;
}

static int fpgaint_init_dev(struct fpgaint_dev * dev)
{
    int ret;
    
    /* set up our lock and wait queue */
    spin_lock_init(&dev->lock);
    init_waitqueue_head(&dev->wq);

    /* set up our interrupt handler queue */
    dev->irq_fifo_head = 0;
    dev->irq_fifo_tail = 0;

    /* get our name from the device tree */
    ret = of_property_read_string(dev->nodep, "label", &dev->name);
    if (ret) {
        printk(KERN_ALERT "FPGA interrupt module: failed to get label\n");
        return ret;
    }

    /* get our IRQ from the device tree */
    dev->irq = irq_of_parse_and_map(dev->nodep, 0);
    if (dev->irq == NO_IRQ) {
        printk(KERN_ALERT "FPGA interrupt module: failed to map interrupt\n");
        return dev->irq;
    }

    /* register our IRQ */
    ret = request_irq(dev->irq, fpgaint_irq_handler, 
            0, dev->name, dev);
    if (ret) {
        printk(KERN_ALERT "FPGA interrupt module: failed to register interrupt for %s at %u\n"               , dev->name, dev->irq);
        irq_dispose_mapping(dev->irq);
        return ret;
    }
    else
        printk(KERN_ALERT "FPGA interrupt module: registered interrupt for %s at %u line\n",                dev->name, dev->irq);


    /* register our device */
    dev->misc_dev.name = dev->name;
    dev->misc_dev.minor = MISC_DYNAMIC_MINOR;
    dev->misc_dev.fops = &fpgaint_fops;
    ret = misc_register(&dev->misc_dev);
    if (ret) {
        printk(KERN_ALERT "FPGA interrupt module: failed to register device %s\n",
                dev->name);
        free_irq(dev->irq, 0);
        irq_dispose_mapping(dev->irq);
        return ret;
    }

    return 0;
}

static void fpgaint_release_dev(struct fpgaint_dev * dev)
{
    misc_deregister(&dev->misc_dev);
    free_irq(dev->irq, dev);
}

static int fpgaint_store_dev(struct fpgaint_dev * dev) {
    unsigned int index;

    /* update our records */
    spin_lock(&fpgaint_private.lock);

    /* check if we have space */
    if (fpgaint_private.num_devs >= FPGAINT_MAX_DEVS) {
        spin_unlock(&fpgaint_private.lock);
        return -ENOMEM;
    }

    /* we do, so store it */
    fpgaint_private.num_devs++;
    for (index = 0; index < FPGAINT_MAX_DEVS; index++) {
        if (fpgaint_private.devs[index] == NULL) {
            fpgaint_private.devs[index] = dev;
            break;
        }
    }
    spin_unlock(&fpgaint_private.lock);

    return 0;
}

static void fpgaint_unstore_dev(struct fpgaint_dev * dev) {
    unsigned int index;

    /* update our records */
    spin_lock(&fpgaint_private.lock);
    fpgaint_private.num_devs--;
    for (index = 0; index < FPGAINT_MAX_DEVS; index++) {
        if (dev == fpgaint_private.devs[index]) {
            fpgaint_private.devs[index] = NULL;
            break;
        }
    }
    spin_unlock(&fpgaint_private.lock);
}

static int fpgaint_probe(struct platform_device *pdev)
{
    int ret;
    struct fpgaint_dev * dev;

    printk(KERN_ALERT "FPGA interrupt module: %s called\n", __FUNCTION__);

    /* allocate our device specific memory */
    dev = kzalloc(sizeof(struct fpgaint_dev), GFP_KERNEL);
    if (!dev) {
        ret = -ENOMEM;
        goto error_alloc;
    }

    /* add it to our records */
    ret = fpgaint_store_dev(dev);
    if (ret)
        goto error_store_dev;

    /* get our device tree node */
    dev->nodep = pdev->dev.of_node;

    /* store our private data in the device struct */
    platform_set_drvdata(pdev, dev);

    /* initialize the device */
    ret = fpgaint_init_dev(dev);
    if (ret)
        goto error_init_dev;

    /* successful return */
    return 0;

    /* error handling */
error_init_dev:
    fpgaint_unstore_dev(dev);
    platform_set_drvdata(pdev, NULL);
error_store_dev:
    kzfree(dev);
error_alloc:
    return ret;
}

static int fpgaint_remove(struct platform_device *pdev)
{
    struct fpgaint_dev * dev = NULL;

    printk(KERN_ALERT "FPGA interrupt module: %s called\n", __FUNCTION__);

    /* get our private data */
    dev = (struct fpgaint_dev*)platform_get_drvdata(pdev);
    if (!dev) {
        printk(KERN_ALERT "FPGA interrupt module: no private data to remove!\n");
        return -EINVAL;
    }

    /* release the device */
    fpgaint_release_dev(dev);

    /* update our records */
    fpgaint_unstore_dev(dev);

    /* ensure it is not accessible */
    platform_set_drvdata(pdev, NULL);

    /* free the memory */
    kzfree(dev);

    return 0;
}

/* our compatibility list for the device tree */
static struct of_device_id fpgaint_of_match[] = {
    { .compatible = "altr,fpgaint-0.1", },
    {}
};

MODULE_DEVICE_TABLE(of, fpgaint_of_match);

/* our platform driver structure */
static struct platform_driver fpgaint_platform_driver = {
    .probe = fpgaint_probe,
    .remove = fpgaint_remove,
    .driver = {
        .name = "altera-fpga-interrupt-driver",
        .owner = THIS_MODULE,
        .of_match_table = fpgaint_of_match,
    },
};

static int __init fpgaint_init(void)
{
    unsigned int index;

    printk(KERN_ALERT "FPGA interrupt module: loaded\n");

    /* initialize our private data */
    for (index = 0; index < FPGAINT_MAX_DEVS; index++) {
        fpgaint_private.devs[index] = NULL;
    }
    fpgaint_private.num_devs = 0;
    spin_lock_init(&fpgaint_private.lock);

    return platform_driver_register(&fpgaint_platform_driver);
}

static void __exit fpgaint_exit(void)
{
    printk(KERN_ALERT "FPGA interrupt module: unloaded\n");

    return platform_driver_unregister(&fpgaint_platform_driver);
}

module_init(fpgaint_init);
module_exit(fpgaint_exit);



