/**
 * \file sgdma_dispatcher.h
 *
 * \description SGDMA Dispatcher interface class
 *
 *
 * \copyright Critical Link LLC 2013
 */
#ifndef SGDMA_DISPATCHER_H
#define SGDMA_DISPATCHER_H
#include "stdio.h"
#include "sgdma_dispatcher_regs.h"


class tcCSRMap;
/**
 *  The tcSDGMADispatcher class wraps a user space control class around the
 *  Scatter Gather DMA master Avalon core in ST->MM or MM->ST mode.
 */
class tcSGDMADispatcher
{
public:
	tcSGDMADispatcher(unsigned int CtrlRegsAddr,
						unsigned int DescRegsAddr,
		          		unsigned int RespRegsAddr,
						bool EnhancedMode = false);


	~tcSGDMADispatcher(void);

	int WriteDescriptor(tsSGDMADescriptor& descriptor);
	int SetDescriptor(tsSGDMADescriptor& descriptor);
	int GetStatus(tsSGDMADescriptor& descriptor);
	int GetResponse(tsSGDMADescriptor& descriptor);

	tuSgdmaStatus GetStatusReg();
protected:
	bool		mbEnhanced; //!< when true, use enhanced descriptors
	tcCSRMap *mpCsr;
	tcCSRMap *mpDescriptors;
	tcCSRMap *mpResponse;
};

#endif
