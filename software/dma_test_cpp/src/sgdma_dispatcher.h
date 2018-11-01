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

enum _mtype {rd, wr, desc, desp, resp, sta};

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

        int SetDescriptor(tsSGDMADescriptor& descript);
	int WriteDescriptor();

        int InitDescriptor(unsigned int wr_addr, unsigned int rd_addr, unsigned int len = 0xFFFFFFFF);
        int SetPacketization(bool val);
        int SetPacketSize(unsigned int val);

        tuSgdmaCtrl GetControlReg();
        int GetSequenceNumber(_mtype _ty); // read, write
        int GetBufferFillLevel(_mtype _ty); // read, write, response
	tuSgdmaStatus GetStatusReg();
	int GetResponseReg();
        
        bool isBusy();
        bool isBufferEmpty(_mtype _ty); // desc, response
        bool isFull(_mtype _ty); // desc, response
        bool isResting();
        bool isStopped();
        bool isStoppedOnError();
        bool isStoppedOnEarlyTermination();

        int SetStopOnError(bool val);
        int SetStopOnEarlyTermination(bool val);
        int Enable(_mtype _ty, bool val); // disp, descript
        int SetGlobalInterruptMask(bool val);
        int Reset();
        int ReadIRQ();
        int ClearIRQ();




protected:
	bool		mbEnhanced; //!< when true, use enhanced descriptors
	tcCSRMap *mpCsr;
	tcCSRMap *mpDescriptors;
	tcCSRMap *mpResponse;
        tsSGDMADescriptor descriptor;
        tuSgdmaCtrl mCtlrReg;
        bool ready;
        bool isReady(_mtype _ty);
};

#endif
