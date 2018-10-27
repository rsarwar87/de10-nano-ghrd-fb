# Golden Reference Design for DE10-Nano Board to be used with OEM supplied LXDE based linux firmware: (Linux LXDE Desktop (kernel 4.5 : http://download.terasic.com/downloads/cd-rom/de10-nano/linux_BSP/DE10_Nano_LXDE.zip))

the top level was is implemented in VHDL. The remaining ip/soc_system was taken from SoC_FB example.

This particular example implements three different varities of DMA to transfer data  between HPS DDR3 RAM and the fpga using mm-stream interface.
The 1 GB RAM is divided in two halves: 0-900 MB for linux partition; whilst the range 900-1000GB belongs to the FPGA.

Examples realized:
1. av-stream from custom ip -> Write Master -> mSGDMA Dispatcher -> HPS RAM (0x3900_0000) [Working]
2. HPS RAM (0x3600_0000) -> Read master -> mSGDMA Dispatcher -> mSGDMA Dispatcher -> Write Master -> HPS_RAM (0x3700_0000) [Problem with mm2s interface]
3. HPS RAM -> mSGDMA -> mSGDMA -> HPS RAM [Not tested yet]

ref: https://support.criticallink.com/redmine/projects/mityarm-5cs/wiki/writing_to_hps_memory

ref: https://fpgawiki.intel.com/wiki/File:Modular_SGDMA_DE.zip

ref: https://github.com/rsarwar87/soc-workshop/tree/master/WS1-IntroToSoC/software/Linux_fft_app
