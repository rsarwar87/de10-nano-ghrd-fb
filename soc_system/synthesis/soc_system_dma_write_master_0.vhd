-- soc_system_dma_write_master_0.vhd

-- Generated using ACDS version 18.1 625

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity soc_system_dma_write_master_0 is
	generic (
		DATA_WIDTH                     : integer := 256;
		LENGTH_WIDTH                   : integer := 32;
		FIFO_DEPTH                     : integer := 32;
		STRIDE_ENABLE                  : integer := 0;
		BURST_ENABLE                   : integer := 0;
		PACKET_ENABLE                  : integer := 1;
		ERROR_ENABLE                   : integer := 0;
		ERROR_WIDTH                    : integer := 8;
		BYTE_ENABLE_WIDTH              : integer := 32;
		BYTE_ENABLE_WIDTH_LOG2         : integer := 5;
		ADDRESS_WIDTH                  : integer := 32;
		FIFO_DEPTH_LOG2                : integer := 5;
		SYMBOL_WIDTH                   : integer := 8;
		NUMBER_OF_SYMBOLS              : integer := 32;
		NUMBER_OF_SYMBOLS_LOG2         : integer := 5;
		MAX_BURST_COUNT_WIDTH          : integer := 1;
		UNALIGNED_ACCESSES_ENABLE      : integer := 0;
		ONLY_FULL_ACCESS_ENABLE        : integer := 0;
		BURST_WRAPPING_SUPPORT         : integer := 0;
		PROGRAMMABLE_BURST_ENABLE      : integer := 0;
		MAX_BURST_COUNT                : integer := 1;
		FIFO_SPEED_OPTIMIZATION        : integer := 1;
		STRIDE_WIDTH                   : integer := 1;
		ACTUAL_BYTES_TRANSFERRED_WIDTH : integer := 32
	);
	port (
		clk                : in  std_logic                      := '0';             --             Clock.clk
		reset              : in  std_logic                      := '0';             --       Clock_reset.reset
		master_address     : out std_logic_vector(31 downto 0);                     -- Data_Write_Master.address
		master_write       : out std_logic;                                         --                  .write
		master_byteenable  : out std_logic_vector(31 downto 0);                     --                  .byteenable
		master_writedata   : out std_logic_vector(255 downto 0);                    --                  .writedata
		master_waitrequest : in  std_logic                      := '0';             --                  .waitrequest
		snk_data           : in  std_logic_vector(255 downto 0) := (others => '0'); --         Data_Sink.data
		snk_valid          : in  std_logic                      := '0';             --                  .valid
		snk_ready          : out std_logic;                                         --                  .ready
		snk_sop            : in  std_logic                      := '0';             --                  .startofpacket
		snk_eop            : in  std_logic                      := '0';             --                  .endofpacket
		snk_empty          : in  std_logic_vector(4 downto 0)   := (others => '0'); --                  .empty
		snk_command_data   : in  std_logic_vector(255 downto 0) := (others => '0'); --      Command_Sink.data
		snk_command_valid  : in  std_logic                      := '0';             --                  .valid
		snk_command_ready  : out std_logic;                                         --                  .ready
		src_response_data  : out std_logic_vector(255 downto 0);                    --   Response_Source.data
		src_response_valid : out std_logic;                                         --                  .valid
		src_response_ready : in  std_logic                      := '0';             --                  .ready
		master_burstcount  : out std_logic_vector(0 downto 0);
		snk_error          : in  std_logic_vector(7 downto 0)   := (others => '0')
	);
end entity soc_system_dma_write_master_0;

architecture rtl of soc_system_dma_write_master_0 is
	component write_master is
		generic (
			DATA_WIDTH                     : integer := 32;
			LENGTH_WIDTH                   : integer := 32;
			FIFO_DEPTH                     : integer := 32;
			STRIDE_ENABLE                  : integer := 0;
			BURST_ENABLE                   : integer := 0;
			PACKET_ENABLE                  : integer := 0;
			ERROR_ENABLE                   : integer := 0;
			ERROR_WIDTH                    : integer := 8;
			BYTE_ENABLE_WIDTH              : integer := 4;
			BYTE_ENABLE_WIDTH_LOG2         : integer := 2;
			ADDRESS_WIDTH                  : integer := 32;
			FIFO_DEPTH_LOG2                : integer := 5;
			SYMBOL_WIDTH                   : integer := 8;
			NUMBER_OF_SYMBOLS              : integer := 4;
			NUMBER_OF_SYMBOLS_LOG2         : integer := 2;
			MAX_BURST_COUNT_WIDTH          : integer := 2;
			UNALIGNED_ACCESSES_ENABLE      : integer := 0;
			ONLY_FULL_ACCESS_ENABLE        : integer := 0;
			BURST_WRAPPING_SUPPORT         : integer := 1;
			PROGRAMMABLE_BURST_ENABLE      : integer := 0;
			MAX_BURST_COUNT                : integer := 2;
			FIFO_SPEED_OPTIMIZATION        : integer := 1;
			STRIDE_WIDTH                   : integer := 1;
			ACTUAL_BYTES_TRANSFERRED_WIDTH : integer := 32
		);
		port (
			clk                : in  std_logic                      := 'X';             -- clk
			reset              : in  std_logic                      := 'X';             -- reset
			master_address     : out std_logic_vector(31 downto 0);                     -- address
			master_write       : out std_logic;                                         -- write
			master_byteenable  : out std_logic_vector(31 downto 0);                     -- byteenable
			master_writedata   : out std_logic_vector(255 downto 0);                    -- writedata
			master_waitrequest : in  std_logic                      := 'X';             -- waitrequest
			snk_data           : in  std_logic_vector(255 downto 0) := (others => 'X'); -- data
			snk_valid          : in  std_logic                      := 'X';             -- valid
			snk_ready          : out std_logic;                                         -- ready
			snk_sop            : in  std_logic                      := 'X';             -- startofpacket
			snk_eop            : in  std_logic                      := 'X';             -- endofpacket
			snk_empty          : in  std_logic_vector(4 downto 0)   := (others => 'X'); -- empty
			snk_command_data   : in  std_logic_vector(255 downto 0) := (others => 'X'); -- data
			snk_command_valid  : in  std_logic                      := 'X';             -- valid
			snk_command_ready  : out std_logic;                                         -- ready
			src_response_data  : out std_logic_vector(255 downto 0);                    -- data
			src_response_valid : out std_logic;                                         -- valid
			src_response_ready : in  std_logic                      := 'X';             -- ready
			master_burstcount  : out std_logic_vector(0 downto 0);                      -- burstcount
			snk_error          : in  std_logic_vector(7 downto 0)   := (others => 'X')  -- error
		);
	end component write_master;

begin

	dma_write_master_0 : component write_master
		generic map (
			DATA_WIDTH                     => DATA_WIDTH,
			LENGTH_WIDTH                   => LENGTH_WIDTH,
			FIFO_DEPTH                     => FIFO_DEPTH,
			STRIDE_ENABLE                  => STRIDE_ENABLE,
			BURST_ENABLE                   => BURST_ENABLE,
			PACKET_ENABLE                  => PACKET_ENABLE,
			ERROR_ENABLE                   => ERROR_ENABLE,
			ERROR_WIDTH                    => ERROR_WIDTH,
			BYTE_ENABLE_WIDTH              => BYTE_ENABLE_WIDTH,
			BYTE_ENABLE_WIDTH_LOG2         => BYTE_ENABLE_WIDTH_LOG2,
			ADDRESS_WIDTH                  => ADDRESS_WIDTH,
			FIFO_DEPTH_LOG2                => FIFO_DEPTH_LOG2,
			SYMBOL_WIDTH                   => SYMBOL_WIDTH,
			NUMBER_OF_SYMBOLS              => NUMBER_OF_SYMBOLS,
			NUMBER_OF_SYMBOLS_LOG2         => NUMBER_OF_SYMBOLS_LOG2,
			MAX_BURST_COUNT_WIDTH          => MAX_BURST_COUNT_WIDTH,
			UNALIGNED_ACCESSES_ENABLE      => UNALIGNED_ACCESSES_ENABLE,
			ONLY_FULL_ACCESS_ENABLE        => ONLY_FULL_ACCESS_ENABLE,
			BURST_WRAPPING_SUPPORT         => BURST_WRAPPING_SUPPORT,
			PROGRAMMABLE_BURST_ENABLE      => PROGRAMMABLE_BURST_ENABLE,
			MAX_BURST_COUNT                => MAX_BURST_COUNT,
			FIFO_SPEED_OPTIMIZATION        => FIFO_SPEED_OPTIMIZATION,
			STRIDE_WIDTH                   => STRIDE_WIDTH,
			ACTUAL_BYTES_TRANSFERRED_WIDTH => ACTUAL_BYTES_TRANSFERRED_WIDTH
		)
		port map (
			clk                => clk,                --             Clock.clk
			reset              => reset,              --       Clock_reset.reset
			master_address     => master_address,     -- Data_Write_Master.address
			master_write       => master_write,       --                  .write
			master_byteenable  => master_byteenable,  --                  .byteenable
			master_writedata   => master_writedata,   --                  .writedata
			master_waitrequest => master_waitrequest, --                  .waitrequest
			snk_data           => snk_data,           --         Data_Sink.data
			snk_valid          => snk_valid,          --                  .valid
			snk_ready          => snk_ready,          --                  .ready
			snk_sop            => snk_sop,            --                  .startofpacket
			snk_eop            => snk_eop,            --                  .endofpacket
			snk_empty          => snk_empty,          --                  .empty
			snk_command_data   => snk_command_data,   --      Command_Sink.data
			snk_command_valid  => snk_command_valid,  --                  .valid
			snk_command_ready  => snk_command_ready,  --                  .ready
			src_response_data  => src_response_data,  --   Response_Source.data
			src_response_valid => src_response_valid, --                  .valid
			src_response_ready => src_response_ready, --                  .ready
			master_burstcount  => open,               --       (terminated)
			snk_error          => "00000000"          --       (terminated)
		);

end architecture rtl; -- of soc_system_dma_write_master_0
