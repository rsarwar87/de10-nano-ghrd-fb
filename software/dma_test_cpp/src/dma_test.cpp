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

#define R2S_mSGDMA_CSR_BASEADDR 0xFF2a5000
#define R2S_mSGDMA_DESC_BASEADDR 0xFF2a6000
#define HPS_MEM_mSGDMA_SENADDR 0x3A000000
#define S2R_mSGDMA_CSR_BASEADDR 0xFF2a7000
#define S2R_mSGDMA_DESC_BASEADDR 0xFF2a8000
#define HPS_MEM_mSGDMA_RECADDR 0x3B000000

#define S2R_DISPATCHER_CSR_BASEADDR 0xFF2a1000
#define S2R_DISPATCHER_DESC_BASEADDR 0xFF2a2000
#define S2R_DISPATCHER_RESP_BASEADDR 0xFF2a6000
#define HPS_MEM_DISP_S2RDDR 0x3C000000
#define R2S_DISPATCHER_CSR_BASEADDR 0xFF2a3000
#define R2S_DISPATCHER_DESC_BASEADDR 0xFF2a4000
#define R2S_DISPATCHER_RESP_BASEADDR 0xFF2a6000
#define HPS_MEM_DISP_R2SADDR 0x3D000000


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
		descriptor.length = 0xFFFFFFF;
		// The dispatcher will use the End of Packet flag to end the transfer
		//descriptor.control.msBits.end_on_eop = 1;
		//descriptor.control.msBits.park_wr = 1;
		// Tell the dispatcher to start
		descriptor.control.msBits.go = 1;

		printf("Starting Descriptor, write starts at addr: %x: ", HPS_MEM_STORE_STARTADDR);
		dispatcher.SetDescriptor(descriptor);
		dispatcher.SetPacketization(true);
		dispatcher.WriteDescriptor();
		tuSgdmaStatus ret = dispatcher.GetStatusReg();
		printf("DONE %x\n", ret.msBits);

		return 0;
}


int TestBridgeDMAStd()
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
				R2S_mSGDMA_DESC_BASEADDR , 0);
		tcSGDMADispatcher stream2ram(S2R_mSGDMA_CSR_BASEADDR,
				S2R_mSGDMA_DESC_BASEADDR, 0);
		printf("DONE\n");

		// Create send descriptor
		tsSGDMADescriptor descriptor_rs = {0};
		// Set the HPS Memory start address
		descriptor_rs.read_addr = HPS_MEM_mSGDMA_SENADDR;
		//descriptor_rs.write_addr = 0;
		// We are using packetized streams so set length to max
		descriptor_rs.length = 0x20*3;
		// The dispatcher will use the End of Packet flag to end the transfer
		descriptor_rs.control.msBits.gen_sop = 1;
		descriptor_rs.control.msBits.gen_eop = 1;
		// Tell the dispatcher to start
		descriptor_rs.control.msBits.go = 1;

		// Create recv descriptor
		tsSGDMADescriptor descriptor_sr = {0};
		// Set the HPS Memory start address
		//descriptor_sr.read_addr = 0;
		descriptor_sr.write_addr = HPS_MEM_mSGDMA_RECADDR;
		// We are using packetized streams so set length to max
		descriptor_sr.length = 0x20*3;
		// The dispatcher will use the End of Packet flag to end the transfer

		descriptor_sr.control.msBits.end_on_eop = 0;
		// Tell the dispatcher to start
		descriptor_sr.control.msBits.go = 1;


		printf("Starting Descriptor, write starts from %x addr to %x addr\n\t\t ",
								HPS_MEM_mSGDMA_SENADDR, HPS_MEM_mSGDMA_RECADDR);
		stream2ram.SetDescriptor(descriptor_sr);
		stream2ram.WriteDescriptor();
		sleep(0.5);
		ram2stream.SetDescriptor(descriptor_rs);
		ram2stream.WriteDescriptor();

		printf("DONE r2s status=%x s2r status=%x\n",
				ram2stream.isBufferEmpty(desc)*2+ ram2stream.isBusy(),
				stream2ram.isBufferEmpty(desc)*2+ stream2ram.isBusy() );

		return 0;
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
		tcSGDMADispatcher ram2stream(S2R_DISPATCHER_CSR_BASEADDR,
				R2S_DISPATCHER_DESC_BASEADDR , 0);
		tcSGDMADispatcher stream2ram(S2R_DISPATCHER_CSR_BASEADDR,
				S2R_DISPATCHER_DESC_BASEADDR, 0);
		printf("DONE\n");

		// Create send descriptor
		tsSGDMADescriptor descriptor_rs = {0};
		// Set the HPS Memory start address
		descriptor_rs.read_addr = HPS_MEM_DISP_S2RDDR;
		//descriptor_rs.write_addr = 0;
		// We are using packetized streams so set length to max
		descriptor_rs.length = 0x20*3;
		// The dispatcher will use the End of Packet flag to end the transfer
		descriptor_rs.control.msBits.gen_sop = 1;
		descriptor_rs.control.msBits.gen_eop = 1;
		// Tell the dispatcher to start
		descriptor_rs.control.msBits.go = 1;

		// Create recv descriptor
		tsSGDMADescriptor descriptor_sr = {0};
		// Set the HPS Memory start address
		//descriptor_sr.read_addr = 0;
		descriptor_sr.write_addr = HPS_MEM_DISP_R2SADDR;
		// We are using packetized streams so set length to max
		descriptor_sr.length = 0x20*3;
		// The dispatcher will use the End of Packet flag to end the transfer

		descriptor_sr.control.msBits.end_on_eop = 0;
		// Tell the dispatcher to start
		descriptor_sr.control.msBits.go = 1;


		printf("Starting Descriptor, write starts from %x addr to %x addr\n\t\t ",
								HPS_MEM_mSGDMA_SENADDR, HPS_MEM_mSGDMA_RECADDR);
		stream2ram.SetDescriptor(descriptor_sr);
		stream2ram.WriteDescriptor();
		sleep(0.5);
		ram2stream.SetDescriptor(descriptor_rs);
		ram2stream.WriteDescriptor();

		printf("DONE r2s status=%x s2r status=%x\n",
				2*stream2ram.isBusy()+stream2ram.isBufferEmpty(desc),
				2*ram2stream.isBusy()+ram2stream.isBufferEmpty(desc));

		return 0;
}

int main(int argc, char* argv[])
{
	TestDispatcher();
	TestBridgeDMAStd();
	TestBridge();
	return 0;
}
