KERNEL_LOCATION=/wsfat/terasic/mm/de10_nano/linux-socfpga
ARMMAKE=make ARCH=arm SUBARCH=arm CROSS_COMPILE=/opt/arm-linux-guneabihf/bin/arm-linux-gnueabihf-

obj-m := fpga_int.o

fpga_uinput.ko: fpga_int.c
	$(ARMMAKE) -C $(KERNEL_LOCATION) M=$(PWD) modules

clean:
	rm -f *.ko *.o *.mod.c *.symvers *.order
