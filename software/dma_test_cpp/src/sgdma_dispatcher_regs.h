/**
 * \file sgdma_dispatcher_regs.h
 *
 * \description Register address for the Modular SGDMA Dispatcher
 *
 *
 * \copyright Critical Link LLC 2013
 */
#ifndef SGDMA_DISPATCHER_REGS_H
#define SGDMA_DISPATCHER_REGS_H

#define CSR_STATUS_REG                          (0x0)
#define CSR_CONTROL_REG                         (0x1)
#define CSR_DESCRIPTOR_FILL_LEVEL_REG           (0x2)
#define CSR_RESPONSE_FILL_LEVEL_REG             (0x3)
#define CSR_SEQUENCE_NUMBER_REG                 (0x4)  // this register only exists when the enhanced features are enabled


#define DESCRIPTOR_READ_ADDRESS_REG                      (0x0)
#define DESCRIPTOR_WRITE_ADDRESS_REG                     (0x1)
#define DESCRIPTOR_LENGTH_REG                            (0x2)
#define DESCRIPTOR_CONTROL_STANDARD_REG                  (0x3)

#define DESCRIPTOR_SEQUENCE_NUMBER_REG                   (0x6)
#define DESCRIPTOR_READ_BURST_REG                        (0xE)
#define DESCRIPTOR_WRITE_BURST_REG                       (0xF)
#define DESCRIPTOR_READ_STRIDE_REG                       (0x8)
#define DESCRIPTOR_WRITE_STRIDE_REG                      (0xA)
#define DESCRIPTOR_READ_ADDRESS_HIGH_REG                 (0x5)
#define DESCRIPTOR_WRITE_ADDRESS_HIGH_REG                (0x6)
#define DESCRIPTOR_CONTROL_ENHANCED_REG                  (0x7)


typedef union
{
	struct
	{
		unsigned int tx_chan			: 8;
		unsigned int gen_sop    		: 1;
		unsigned int gen_eop   			: 1;
		unsigned int park_rd			: 1;
		unsigned int park_wr			: 1;
		unsigned int end_on_eop			: 1;
		unsigned int reserved_1			: 1;
		unsigned int tx_cmplte_mask		: 1;
		unsigned int early_term_mask	: 1;
		unsigned int tx_error_mask		: 8;
		unsigned int early_done			: 1;
		unsigned int reserved_2			: 6;
		unsigned int go					: 1;
	} msBits;
	unsigned int mnWord;
} tuSGDMADescCtrl;

struct tsSGDMADescriptor
{
	unsigned int read_addr;
	unsigned int write_addr;
	unsigned int length;
        unsigned int sequence_number : 16;
        unsigned int read_burst_count : 8;
        unsigned int write_burst_count: 8;
        unsigned int read_stride : 16;
        unsigned int write_stride: 16;
        unsigned int read_addr_high;
        unsigned int write_addr_high;

	tuSGDMADescCtrl control;
};

struct tsSGDMADescriptorExtended
{
	unsigned int read_addr_low;
	unsigned int write_addr_low;
	unsigned int length;
        unsigned int sequence_number : 16;
        unsigned int read_burst_count : 8;
        unsigned int write_burst_count: 8;
        unsigned int read_stride : 16;
        unsigned int write_stride: 16;
        unsigned int read_addr_high;
        unsigned int write_addr_high;


	tuSGDMADescCtrl control;
};

typedef union
{
	struct
	{
		unsigned int busy 			: 1;
		unsigned int desc_buf_empty	: 1;
		unsigned int desc_buf_full 	: 1;
		unsigned int desc_res_empty : 1;
		unsigned int desc_res_full 	: 1;
		unsigned int stopped 		: 1;
		unsigned int resetting 		: 1;
		unsigned int stop_on_err 	: 1;
		unsigned int stop_on_early 	: 1;
		unsigned int irq 			: 1;
		unsigned int reserved 		: 22;
	} msBits;
	unsigned int mnWord;
} tuSgdmaStatus;

typedef union
{
	struct
	{
		unsigned int stop_dispatcher 	: 1;
		unsigned int reset_dispatcher	: 1;
		unsigned int stop_on_err 		: 1;
		unsigned int stop_on_early 		: 1;
		unsigned int gl_int_en 			: 1;
		unsigned int stop_desc 			: 1;
		unsigned int reserved 			: 26;
	} msBits;
	unsigned int mnWord;
} tuSgdmaCtrl;

typedef union
{
    unsigned int actual_bytes_transfered;
    struct
    {
        unsigned int error      : 8;
        unsigned int early_done : 1;
        unsigned int reserved   : 23;
    } msBits;
} tuSgdmaResponse;

#endif /*SGDMA_DISPATCHER_REGS_H*/

