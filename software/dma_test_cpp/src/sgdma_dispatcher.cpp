/**
 * \file sgdma_dispatcher.cpp
 *
 * \description SGDMA Dispatcher interface class
 *
 *
 * \copyright Critical Link LLC 2013
 */
#include "sgdma_dispatcher.h"
#include "sgdma_dispatcher_regs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include "csr_map.h"

/**
 * Constructor
 *
 * \param CtrlRegsAddr physical address of control register slave port
 * \param DescRegsAddr physical address of descriptor slave port
 * \param RespRegsAddr physical address of ST->MM Response slave port (set to zero if not used)
 * \param EnhanceMode set to true to use enhanced descriptors.
 */
tcSGDMADispatcher::tcSGDMADispatcher(unsigned int CtrlRegsAddr,
					unsigned int DescRegsAddr,
					unsigned int RespRegsAddr,
					bool EnhancedMode)
: mbEnhanced(EnhancedMode)
, mpCsr(NULL)
, mpDescriptors(NULL)
, mpResponse(NULL)
, ready(false)
{
    descriptor = {0};
	// Memory map in the dispatcher's CSR
	mpCsr = new tcCSRMap(CtrlRegsAddr);
	// Memory map in the dispatcher's Discriptor Reg
	mpDescriptors = new tcCSRMap(DescRegsAddr);
	// Memory map in the dispatcher's Response Reg,
	// if it has one (only write master have one)
	if(RespRegsAddr != 0)
		mpResponse = new tcCSRMap(RespRegsAddr);
}

/**
 *   Destructor.
 */
tcSGDMADispatcher::~tcSGDMADispatcher(void)
{
	if(mpCsr)
		delete mpCsr;
	if(mpDescriptors)
		delete mpDescriptors;
	if(mpResponse)
		delete mpResponse;
}

/**
 * Writes a descriptor to the dispatcher
 *
 * \param descriptor the descriptor that will be added
 * \return 0 on success
 */
bool tcSGDMADispatcher::isReady(_mtype _ty)
{
    if (_ty == desc)
    {
        if(mpDescriptors == NULL || !ready)
            return false; 
        else
           return ready;
    }
    else if (_ty == resp)
    {
        if(mpResponse == NULL || !ready)
            return false; 
        else
           return ready;
    }
    else if (_ty == sta)
    {
        if(mpCsr == NULL || !ready)
            return false; 
        else
           return ready;
    }
    return false;
}

int tcSGDMADispatcher::WriteDescriptor()
{

	if(!isReady(desc))
	    return -1;

	// Write the descriptor to the dispatcher
	mpDescriptors->WriteCtrlReg(DESCRIPTOR_READ_ADDRESS_REG, descriptor.read_addr);
	mpDescriptors->WriteCtrlReg(DESCRIPTOR_WRITE_ADDRESS_REG, descriptor.write_addr);
	mpDescriptors->WriteCtrlReg(DESCRIPTOR_LENGTH_REG, descriptor.length);
	// The control reg needs to be written last because it has the go bit, which posts
	// the descriptor to the dispatcher
	if(mbEnhanced)
	{
		// TODO: implement
            mpDescriptors->WriteCtrlReg16(DESCRIPTOR_SEQUENCE_NUMBER_REG, descriptor.sequence_number);
            mpDescriptors->WriteCtrlReg8(DESCRIPTOR_READ_BURST_REG, descriptor.read_burst_count);
            mpDescriptors->WriteCtrlReg8(DESCRIPTOR_WRITE_BURST_REG, descriptor.write_burst_count );


            mpDescriptors->WriteCtrlReg16(DESCRIPTOR_READ_STRIDE_REG, descriptor.read_stride);
            mpDescriptors->WriteCtrlReg16(DESCRIPTOR_WRITE_STRIDE_REG, descriptor.write_stride);

            mpDescriptors->WriteCtrlReg(DESCRIPTOR_READ_ADDRESS_HIGH_REG, descriptor.read_addr_high);
            mpDescriptors->WriteCtrlReg(DESCRIPTOR_WRITE_ADDRESS_HIGH_REG, descriptor.write_addr_high);
	}
	else
	{
		mpDescriptors->WriteCtrlReg(DESCRIPTOR_CONTROL_STANDARD_REG, descriptor.control.mnWord);
	}

	return 0;
}

/**
 * Reads and returns the status register of the Dispatcher
 *
 * \return the status register
 */
tuSgdmaStatus tcSGDMADispatcher::GetStatusReg()
{
	tuSgdmaStatus status;
	if(isReady(sta))
	    status.mnWord = mpCsr->ReadCtrlReg(CSR_STATUS_REG);
	return status;
}
tuSgdmaCtrl tcSGDMADispatcher::GetControlReg()
{
	tuSgdmaCtrl ctrl;
	if(isReady(sta))
	    ctrl.mnWord = mpCsr->ReadCtrlReg(CSR_CONTROL_REG);
	return ctrl;
}

int tcSGDMADispatcher::SetPacketization(bool val)
{
    descriptor.control.msBits.end_on_eop = val ? 1 : 0;
    return 0;
}


int tcSGDMADispatcher::SetPacketSize(unsigned int val)
{
    descriptor.length = val;
    return 0;
}

bool tcSGDMADispatcher::isBusy()
{
    return (GetStatusReg().msBits.busy == 1) ? true : false;
}

bool tcSGDMADispatcher::isBufferEmpty(_mtype _ty) // desc, response
{
    if (_ty == desc) return (GetStatusReg().msBits.desc_buf_empty == 1) ? true : false;
    else return (GetStatusReg().msBits.desc_res_empty == 1) ? true : false;
}

bool tcSGDMADispatcher::isFull(_mtype _ty) // desc, response
{
    if (_ty == desc) return (GetStatusReg().msBits.desc_buf_full == 1) ? true : false;
    else return (GetStatusReg().msBits.desc_res_full == 1) ? true : false;
}

bool tcSGDMADispatcher::isResting()
{
    return (GetStatusReg().msBits.resetting == 1) ? true : false;
}

bool tcSGDMADispatcher::isStopped()
{
    return (GetStatusReg().msBits.stopped == 1) ? true : false;
}

bool tcSGDMADispatcher::isStoppedOnError()
{
    return (GetStatusReg().msBits.stop_on_err == 1) ? true : false;
}

bool tcSGDMADispatcher::isStoppedOnEarlyTermination()
{
    return (GetStatusReg().msBits.stop_on_early == 1) ? true : false;
}

int tcSGDMADispatcher::SetStopOnError(bool val)
{
    if(!isReady(sta)) return -1;
    tuSgdmaCtrl tmp = GetControlReg();
    if (val == tmp.msBits.stop_on_err) return 0;
    tmp.msBits.stop_on_err = (val ? 1 : 0);
    mpCsr->WriteCtrlReg(CSR_CONTROL_REG, *(unsigned int*)&(tmp.msBits));
    return 0;
}

int tcSGDMADispatcher::SetStopOnEarlyTermination(bool val)
{

    if(!isReady(sta)) return -1;
    tuSgdmaCtrl tmp = GetControlReg();
    if (val == tmp.msBits.stop_on_early) return 0;
    tmp.msBits.stop_on_early = (val ? 1 : 0);
    mpCsr->WriteCtrlReg(CSR_CONTROL_REG, *(unsigned int*)&(tmp.msBits));
    return 0;
}
int tcSGDMADispatcher::GetSequenceNumber(_mtype _ty) // read, write
{
    if(!isReady(sta)) return -1;
    int shift = 0;
    if (_ty == wr) shift = 16;
    return ((mpCsr->ReadCtrlReg(CSR_SEQUENCE_NUMBER_REG) >> shift) & 0xFFFF);
}

int tcSGDMADispatcher::GetBufferFillLevel(_mtype _ty) // read, write, response
{
    if(!isReady(sta)) return -1;
    int shift = 0;
    if (_ty == wr) shift = 16;
    if (_ty == resp)
        return mpCsr->ReadCtrlReg(CSR_RESPONSE_FILL_LEVEL_REG);
    else
        return ((mpCsr->ReadCtrlReg(CSR_DESCRIPTOR_FILL_LEVEL_REG) >> shift) & 0xFFFF);
}

int tcSGDMADispatcher::Enable(_mtype _ty, bool val) // disp, descrip
{
    if(!isReady(sta)) return -1;
    if (_ty == desp)
    {
        tuSgdmaCtrl tmp = GetControlReg();
        if (val == tmp.msBits.stop_dispatcher) return 0;
        tmp.msBits.stop_dispatcher = (val ? 1 : 0);
        mpCsr->WriteCtrlReg(CSR_CONTROL_REG, *(unsigned int*)&(tmp.msBits));
    }
    else
    {
        tuSgdmaCtrl tmp = GetControlReg();
        if (val == tmp.msBits.stop_desc) return 0;
        tmp.msBits.stop_desc = (val ? 1 : 0);
        tmp.msBits.stop_desc = 0;
        mpCsr->WriteCtrlReg(CSR_CONTROL_REG, *(unsigned int*)&(tmp.msBits));

    }
    return 0;
}

int tcSGDMADispatcher::SetGlobalInterruptMask(bool val)
{
    if(!isReady(sta)) return -1;
    tuSgdmaCtrl tmp = GetControlReg();
    if (val == tmp.msBits.gl_int_en) return 0;
    tmp.msBits.gl_int_en = (val ? 1 : 0);
    mpCsr->WriteCtrlReg(CSR_CONTROL_REG, *(unsigned int*)&(tmp.msBits));
    return 0;

}

int tcSGDMADispatcher::Reset()
{
    if(!isReady(sta)) return -1;
    tuSgdmaCtrl tmp;
    tmp.msBits.reset_dispatcher = 1;
    mpCsr->WriteCtrlReg(CSR_CONTROL_REG, *(unsigned int*)&(tmp.msBits));
    return 0;
}
int tcSGDMADispatcher::ReadIRQ()
{
    return (GetStatusReg().msBits.irq);
}

int tcSGDMADispatcher::ClearIRQ()
{
    tuSgdmaStatus tmp = GetStatusReg(); 
    if ( tmp.msBits.irq == 0 )  return -2;
    tmp.msBits.irq = 0;
    mpCsr->WriteCtrlReg(CSR_STATUS_REG, *(unsigned int*)&(tmp.msBits));
    return 0;
}


int tcSGDMADispatcher::InitDescriptor(unsigned int wr_addr, unsigned int rd_addr, unsigned int len)
{
    descriptor.read_addr = rd_addr;
    descriptor.write_addr = wr_addr;
    descriptor.length = len;
    return 0;
}

int tcSGDMADispatcher::SetDescriptor(tsSGDMADescriptor& descript)
{
    descriptor = descript;
    ready = true;
    return 0;
}


