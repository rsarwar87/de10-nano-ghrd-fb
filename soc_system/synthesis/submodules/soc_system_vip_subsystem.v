// soc_system_vip_subsystem.v

// Generated using ACDS version 18.1 625

`timescale 1 ps / 1 ps
module soc_system_vip_subsystem (
		input  wire         alt_vip_itc_0_clocked_video_vid_clk,          //         alt_vip_itc_0_clocked_video.vid_clk
		output wire [31:0]  alt_vip_itc_0_clocked_video_vid_data,         //                                    .vid_data
		output wire         alt_vip_itc_0_clocked_video_underflow,        //                                    .underflow
		output wire         alt_vip_itc_0_clocked_video_vid_datavalid,    //                                    .vid_datavalid
		output wire         alt_vip_itc_0_clocked_video_vid_v_sync,       //                                    .vid_v_sync
		output wire         alt_vip_itc_0_clocked_video_vid_h_sync,       //                                    .vid_h_sync
		output wire         alt_vip_itc_0_clocked_video_vid_f,            //                                    .vid_f
		output wire         alt_vip_itc_0_clocked_video_vid_h,            //                                    .vid_h
		output wire         alt_vip_itc_0_clocked_video_vid_v,            //                                    .vid_v
		input  wire         alt_vip_itc_0_is_clk_rst_clk,                 //            alt_vip_itc_0_is_clk_rst.clk
		input  wire         alt_vip_itc_0_is_clk_rst_reset_reset,         //      alt_vip_itc_0_is_clk_rst_reset.reset
		output wire [31:0]  alt_vip_vfr_hdmi_avalon_master_address,       //      alt_vip_vfr_hdmi_avalon_master.address
		output wire [5:0]   alt_vip_vfr_hdmi_avalon_master_burstcount,    //                                    .burstcount
		input  wire [127:0] alt_vip_vfr_hdmi_avalon_master_readdata,      //                                    .readdata
		output wire         alt_vip_vfr_hdmi_avalon_master_read,          //                                    .read
		input  wire         alt_vip_vfr_hdmi_avalon_master_readdatavalid, //                                    .readdatavalid
		input  wire         alt_vip_vfr_hdmi_avalon_master_waitrequest,   //                                    .waitrequest
		input  wire [4:0]   alt_vip_vfr_hdmi_avalon_slave_address,        //       alt_vip_vfr_hdmi_avalon_slave.address
		input  wire         alt_vip_vfr_hdmi_avalon_slave_write,          //                                    .write
		input  wire [31:0]  alt_vip_vfr_hdmi_avalon_slave_writedata,      //                                    .writedata
		input  wire         alt_vip_vfr_hdmi_avalon_slave_read,           //                                    .read
		output wire [31:0]  alt_vip_vfr_hdmi_avalon_slave_readdata,       //                                    .readdata
		input  wire         alt_vip_vfr_hdmi_clock_master_clk,            //       alt_vip_vfr_hdmi_clock_master.clk
		input  wire         alt_vip_vfr_hdmi_clock_master_reset_reset,    // alt_vip_vfr_hdmi_clock_master_reset.reset
		input  wire         alt_vip_vfr_hdmi_clock_reset_clk,             //        alt_vip_vfr_hdmi_clock_reset.clk
		input  wire         alt_vip_vfr_hdmi_clock_reset_reset_reset,     //  alt_vip_vfr_hdmi_clock_reset_reset.reset
		output wire         alt_vip_vfr_hdmi_interrupt_sender_irq         //   alt_vip_vfr_hdmi_interrupt_sender.irq
	);

	wire         alt_vip_vfr_hdmi_avalon_streaming_source_valid;         // alt_vip_vfr_hdmi:dout_valid -> alt_vip_itc_0:is_valid
	wire  [31:0] alt_vip_vfr_hdmi_avalon_streaming_source_data;          // alt_vip_vfr_hdmi:dout_data -> alt_vip_itc_0:is_data
	wire         alt_vip_vfr_hdmi_avalon_streaming_source_ready;         // alt_vip_itc_0:is_ready -> alt_vip_vfr_hdmi:dout_ready
	wire         alt_vip_vfr_hdmi_avalon_streaming_source_startofpacket; // alt_vip_vfr_hdmi:dout_startofpacket -> alt_vip_itc_0:is_sop
	wire         alt_vip_vfr_hdmi_avalon_streaming_source_endofpacket;   // alt_vip_vfr_hdmi:dout_endofpacket -> alt_vip_itc_0:is_eop

	alt_vipitc131_IS2Vid #(
		.NUMBER_OF_COLOUR_PLANES       (4),
		.COLOUR_PLANES_ARE_IN_PARALLEL (1),
		.BPS                           (8),
		.INTERLACED                    (0),
		.H_ACTIVE_PIXELS               (1024),
		.V_ACTIVE_LINES                (768),
		.ACCEPT_COLOURS_IN_SEQ         (0),
		.FIFO_DEPTH                    (1920),
		.CLOCKS_ARE_SAME               (0),
		.USE_CONTROL                   (0),
		.NO_OF_MODES                   (1),
		.THRESHOLD                     (1919),
		.STD_WIDTH                     (1),
		.GENERATE_SYNC                 (0),
		.USE_EMBEDDED_SYNCS            (0),
		.AP_LINE                       (0),
		.V_BLANK                       (0),
		.H_BLANK                       (0),
		.H_SYNC_LENGTH                 (136),
		.H_FRONT_PORCH                 (24),
		.H_BACK_PORCH                  (160),
		.V_SYNC_LENGTH                 (6),
		.V_FRONT_PORCH                 (3),
		.V_BACK_PORCH                  (29),
		.F_RISING_EDGE                 (0),
		.F_FALLING_EDGE                (0),
		.FIELD0_V_RISING_EDGE          (0),
		.FIELD0_V_BLANK                (0),
		.FIELD0_V_SYNC_LENGTH          (0),
		.FIELD0_V_FRONT_PORCH          (0),
		.FIELD0_V_BACK_PORCH           (0),
		.ANC_LINE                      (0),
		.FIELD0_ANC_LINE               (0)
	) alt_vip_itc_0 (
		.is_clk        (alt_vip_itc_0_is_clk_rst_clk),                           //       is_clk_rst.clk
		.rst           (alt_vip_itc_0_is_clk_rst_reset_reset),                   // is_clk_rst_reset.reset
		.is_data       (alt_vip_vfr_hdmi_avalon_streaming_source_data),          //              din.data
		.is_valid      (alt_vip_vfr_hdmi_avalon_streaming_source_valid),         //                 .valid
		.is_ready      (alt_vip_vfr_hdmi_avalon_streaming_source_ready),         //                 .ready
		.is_sop        (alt_vip_vfr_hdmi_avalon_streaming_source_startofpacket), //                 .startofpacket
		.is_eop        (alt_vip_vfr_hdmi_avalon_streaming_source_endofpacket),   //                 .endofpacket
		.vid_clk       (alt_vip_itc_0_clocked_video_vid_clk),                    //    clocked_video.export
		.vid_data      (alt_vip_itc_0_clocked_video_vid_data),                   //                 .export
		.underflow     (alt_vip_itc_0_clocked_video_underflow),                  //                 .export
		.vid_datavalid (alt_vip_itc_0_clocked_video_vid_datavalid),              //                 .export
		.vid_v_sync    (alt_vip_itc_0_clocked_video_vid_v_sync),                 //                 .export
		.vid_h_sync    (alt_vip_itc_0_clocked_video_vid_h_sync),                 //                 .export
		.vid_f         (alt_vip_itc_0_clocked_video_vid_f),                      //                 .export
		.vid_h         (alt_vip_itc_0_clocked_video_vid_h),                      //                 .export
		.vid_v         (alt_vip_itc_0_clocked_video_vid_v)                       //                 .export
	);

	alt_vipvfr131_vfr #(
		.BITS_PER_PIXEL_PER_COLOR_PLANE (8),
		.NUMBER_OF_CHANNELS_IN_PARALLEL (4),
		.NUMBER_OF_CHANNELS_IN_SEQUENCE (1),
		.MAX_IMAGE_WIDTH                (1024),
		.MAX_IMAGE_HEIGHT               (768),
		.MEM_PORT_WIDTH                 (128),
		.RMASTER_FIFO_DEPTH             (64),
		.RMASTER_BURST_TARGET           (32),
		.CLOCKS_ARE_SEPARATE            (1)
	) alt_vip_vfr_hdmi (
		.clock                (alt_vip_vfr_hdmi_clock_reset_clk),                       //             clock_reset.clk
		.reset                (alt_vip_vfr_hdmi_clock_reset_reset_reset),               //       clock_reset_reset.reset
		.master_clock         (alt_vip_vfr_hdmi_clock_master_clk),                      //            clock_master.clk
		.master_reset         (alt_vip_vfr_hdmi_clock_master_reset_reset),              //      clock_master_reset.reset
		.slave_address        (alt_vip_vfr_hdmi_avalon_slave_address),                  //            avalon_slave.address
		.slave_write          (alt_vip_vfr_hdmi_avalon_slave_write),                    //                        .write
		.slave_writedata      (alt_vip_vfr_hdmi_avalon_slave_writedata),                //                        .writedata
		.slave_read           (alt_vip_vfr_hdmi_avalon_slave_read),                     //                        .read
		.slave_readdata       (alt_vip_vfr_hdmi_avalon_slave_readdata),                 //                        .readdata
		.slave_irq            (alt_vip_vfr_hdmi_interrupt_sender_irq),                  //        interrupt_sender.irq
		.dout_data            (alt_vip_vfr_hdmi_avalon_streaming_source_data),          // avalon_streaming_source.data
		.dout_valid           (alt_vip_vfr_hdmi_avalon_streaming_source_valid),         //                        .valid
		.dout_ready           (alt_vip_vfr_hdmi_avalon_streaming_source_ready),         //                        .ready
		.dout_startofpacket   (alt_vip_vfr_hdmi_avalon_streaming_source_startofpacket), //                        .startofpacket
		.dout_endofpacket     (alt_vip_vfr_hdmi_avalon_streaming_source_endofpacket),   //                        .endofpacket
		.master_address       (alt_vip_vfr_hdmi_avalon_master_address),                 //           avalon_master.address
		.master_burstcount    (alt_vip_vfr_hdmi_avalon_master_burstcount),              //                        .burstcount
		.master_readdata      (alt_vip_vfr_hdmi_avalon_master_readdata),                //                        .readdata
		.master_read          (alt_vip_vfr_hdmi_avalon_master_read),                    //                        .read
		.master_readdatavalid (alt_vip_vfr_hdmi_avalon_master_readdatavalid),           //                        .readdatavalid
		.master_waitrequest   (alt_vip_vfr_hdmi_avalon_master_waitrequest)              //                        .waitrequest
	);

endmodule
