/**
 * \file main.cpp
 *
 * \description main entry point for Application.
 *
 *
 * \copyright Critical Link LLC 2013
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "sgdma_dispatcher.h"
#include "sgdma_dispatcher_regs.h"

#define DISPATCHER_CSR_BASEADDR 0xFF2b1000
#define DISPATCHER_DESC_BASEADDR 0xFF2b2000
#define DISPATCHER_RESP_BASEADDR 0xFF2b3000
#define HPS_MEM_STORE_STARTADDR 0x39000000

#define R2S_mSGDMA_CSR_BASEADDR 0xFF2a1000
#define R2S_mSGDMA_DESC_BASEADDR 0xFF2a2000
#define HPS_MEM_mSGDMA_SENADDR 0x36000000
#define S2R_mSGDMA_CSR_BASEADDR 0xFF2a3000
#define S2R_mSGDMA_DESC_BASEADDR 0xFF2a4000
#define HPS_MEM_mSGDMA_RECADDR 0x37000000

#define S2R_DISPATCHER_CSR_BASEADDR 0xFF2a5000
#define S2R_DISPATCHER_DESC_BASEADDR 0xFF2a6000
#define S2R_DISPATCHER_RESP_BASEADDR 0xFF2a6000
#define HPS_MEM_DISP_RECADDR 0x3A000000
#define R2S_DISPATCHER_CSR_BASEADDR 0xFF2a5000
#define R2S_DISPATCHER_DESC_BASEADDR 0xFF2a6000
#define R2S_DISPATCHER_RESP_BASEADDR 0xFF2a6000
#define HPS_MEM_DISP_RECADDR 0x3B000000


int TestDispatcher()
{
		printf("\n\nCreating dispatcher control --  : ");
		tcSGDMADispatcher dispatcher(DISPATCHER_CSR_BASEADDR, DISPATCHER_DESC_BASEADDR, 0);
		printf("DONE\n");

		// Create our descriptor
		tsSGDMADescriptor descriptor = {0};
		// Set the HPS Memory start address
		descriptor.write_addr = HPS_MEM_STORE_STARTADDR;
		// We are using packetized streams so set length to max
		descriptor.length = 0x20*3;
		// The dispatcher will use the End of Packet flag to end the transfer
		descriptor.control.msBits.end_on_eop = 0;
		// Tell the dispatcher to start
		descriptor.control.msBits.go = 1;

		printf("Starting Descriptor, write starts at addr: %x: ", HPS_MEM_STORE_STARTADDR);
		dispatcher.WriteDescriptor(descriptor);
		tuSgdmaStatus ret = dispatcher.GetStatusReg();
		printf("DONE %x\n", ret.msBits);

}


int TestBridge()
{
/*
#define R2S_mSGDMA_CSR_BASEADDR 0xFF2a1000
#define R2S_mSGDMA_DESC_BASEADDR 0xFF2a2000
#define HPS_MEM_mSGDMA_SENADDR 0x36000000
#define S2R_mSGDMA_CSR_BASEADDR 0xFF2a3000
#define S2R_mSGDMA_DESC_BASEADDR 0xFF2a4000
#define HPS_MEM_mSGDMA_RECADDR 0x37000000
*/
		printf("\n\nCreating bridge control --  : ");
		tcSGDMADispatcher ram2stream(R2S_mSGDMA_CSR_BASEADDR,
									 R2S_mSGDMA_DESC_BASEADDR, 0);
		tcSGDMADispatcher stream2ram(S2R_mSGDMA_CSR_BASEADDR,
									 S2R_mSGDMA_DESC_BASEADDR, 0);
		printf("DONE\n");

		// Create send descriptor
		tsSGDMADescriptor descriptor_rs = {0};
		// Set the HPS Memory start address
		descriptor_rs.read_addr = HPS_MEM_mSGDMA_SENADDR;
		descriptor_rs.write_addr = 0;
		// We are using packetized streams so set length to max
		descriptor_rs.length = 0x20*3;
		// The dispatcher will use the End of Packet flag to end the transfer
		descriptor_rs.control.msBits.end_on_eop = 0;
		// Tell the dispatcher to start
		descriptor_rs.control.msBits.go = 1;

		// Create recv descriptor
		tsSGDMADescriptor descriptor_sr = {0};
		// Set the HPS Memory start address
		descriptor_sr.read_addr = 0;
		descriptor_sr.write_addr = HPS_MEM_mSGDMA_RECADDR;
		// We are using packetized streams so set length to max
		descriptor_sr.length = 0x20*3;
		// The dispatcher will use the End of Packet flag to end the transfer
		descriptor_sr.control.msBits.end_on_eop = 0;
		// Tell the dispatcher to start
		descriptor_sr.control.msBits.go = 1;


		printf("Starting Descriptor, write starts from %x addr to %x addr\n\t\t ",
								HPS_MEM_mSGDMA_SENADDR, HPS_MEM_mSGDMA_RECADDR);
		ram2stream.WriteDescriptor(descriptor_sr);
		stream2ram.WriteDescriptor(descriptor_rs);
		tuSgdmaStatus ret1 = ram2stream.GetStatusReg();
		tuSgdmaStatus ret2 = stream2ram.GetStatusReg();
		printf("DONE r2s status=%x s2r status=%x\n", ret1.msBits, ret2.msBits);
}

int main(int argc, char* argv[])
{
	TestDispatcher();
	TestBridge();
	return 0;
}
