/**
 * \file sgdma_dispatcher.cpp
 *
 * \description SGDMA Dispatcher interface class
 *
 *
 * \copyright Critical Link LLC 2013
 */
#include "sgdma_dispatcher.h"
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
{
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
int tcSGDMADispatcher::WriteDescriptor(tsSGDMADescriptor& descriptor)
{

	if(mpDescriptors == NULL)
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
	status.mnWord = mpCsr->ReadCtrlReg(CSR_STATUS_REG);
	return status;
}
