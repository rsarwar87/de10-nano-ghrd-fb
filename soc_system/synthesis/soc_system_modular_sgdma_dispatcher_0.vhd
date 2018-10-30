-- soc_system_modular_sgdma_dispatcher_0.vhd

-- Generated using ACDS version 18.1 625

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity soc_system_modular_sgdma_dispatcher_0 is
	generic (
		MODE                        : integer := 2;
		RESPONSE_PORT               : integer := 2;
		DESCRIPTOR_INTERFACE        : integer := 0;
		DESCRIPTOR_FIFO_DEPTH       : integer := 128;
		ENHANCED_FEATURES           : integer := 0;
		DESCRIPTOR_WIDTH            : integer := 128;
		DESCRIPTOR_BYTEENABLE_WIDTH : integer := 16
	);
	port (
		clk                     : in  std_logic                      := '0';             --                clock.clk
		reset                   : in  std_logic                      := '0';             --          clock_reset.reset
		csr_writedata           : in  std_logic_vector(31 downto 0)  := (others => '0'); --                  CSR.writedata
		csr_write               : in  std_logic                      := '0';             --                     .write
		csr_byteenable          : in  std_logic_vector(3 downto 0)   := (others => '0'); --                     .byteenable
		csr_readdata            : out std_logic_vector(31 downto 0);                     --                     .readdata
		csr_read                : in  std_logic                      := '0';             --                     .read
		csr_address             : in  std_logic_vector(2 downto 0)   := (others => '0'); --                     .address
		descriptor_write        : in  std_logic                      := '0';             --     Descriptor_Slave.write
		descriptor_waitrequest  : out std_logic;                                         --                     .waitrequest
		descriptor_writedata    : in  std_logic_vector(127 downto 0) := (others => '0'); --                     .writedata
		descriptor_byteenable   : in  std_logic_vector(15 downto 0)  := (others => '0'); --                     .byteenable
		src_write_master_data   : out std_logic_vector(255 downto 0);                    -- Write_Command_Source.data
		src_write_master_valid  : out std_logic;                                         --                     .valid
		src_write_master_ready  : in  std_logic                      := '0';             --                     .ready
		snk_write_master_data   : in  std_logic_vector(255 downto 0) := (others => '0'); --  Write_Response_Sink.data
		snk_write_master_valid  : in  std_logic                      := '0';             --                     .valid
		snk_write_master_ready  : out std_logic;                                         --                     .ready
		csr_irq                 : out std_logic;                                         --              csr_irq.irq
		mm_response_address     : in  std_logic                      := '0';
		mm_response_byteenable  : in  std_logic_vector(3 downto 0)   := (others => '0');
		mm_response_read        : in  std_logic                      := '0';
		mm_response_readdata    : out std_logic_vector(31 downto 0);
		mm_response_waitrequest : out std_logic;
		snk_descriptor_data     : in  std_logic_vector(127 downto 0) := (others => '0');
		snk_descriptor_ready    : out std_logic;
		snk_descriptor_valid    : in  std_logic                      := '0';
		snk_read_master_data    : in  std_logic_vector(255 downto 0) := (others => '0');
		snk_read_master_ready   : out std_logic;
		snk_read_master_valid   : in  std_logic                      := '0';
		src_read_master_data    : out std_logic_vector(255 downto 0);
		src_read_master_ready   : in  std_logic                      := '0';
		src_read_master_valid   : out std_logic;
		src_response_data       : out std_logic_vector(255 downto 0);
		src_response_ready      : in  std_logic                      := '0';
		src_response_valid      : out std_logic
	);
end entity soc_system_modular_sgdma_dispatcher_0;

architecture rtl of soc_system_modular_sgdma_dispatcher_0 is
	component dispatcher is
		generic (
			MODE                        : integer := 0;
			RESPONSE_PORT               : integer := 0;
			DESCRIPTOR_INTERFACE        : integer := 0;
			DESCRIPTOR_FIFO_DEPTH       : integer := 128;
			ENHANCED_FEATURES           : integer := 1;
			DESCRIPTOR_WIDTH            : integer := 256;
			DESCRIPTOR_BYTEENABLE_WIDTH : integer := 32
		);
		port (
			clk                     : in  std_logic                      := 'X';             -- clk
			reset                   : in  std_logic                      := 'X';             -- reset
			csr_writedata           : in  std_logic_vector(31 downto 0)  := (others => 'X'); -- writedata
			csr_write               : in  std_logic                      := 'X';             -- write
			csr_byteenable          : in  std_logic_vector(3 downto 0)   := (others => 'X'); -- byteenable
			csr_readdata            : out std_logic_vector(31 downto 0);                     -- readdata
			csr_read                : in  std_logic                      := 'X';             -- read
			csr_address             : in  std_logic_vector(2 downto 0)   := (others => 'X'); -- address
			descriptor_write        : in  std_logic                      := 'X';             -- write
			descriptor_waitrequest  : out std_logic;                                         -- waitrequest
			descriptor_writedata    : in  std_logic_vector(127 downto 0) := (others => 'X'); -- writedata
			descriptor_byteenable   : in  std_logic_vector(15 downto 0)  := (others => 'X'); -- byteenable
			src_write_master_data   : out std_logic_vector(255 downto 0);                    -- data
			src_write_master_valid  : out std_logic;                                         -- valid
			src_write_master_ready  : in  std_logic                      := 'X';             -- ready
			snk_write_master_data   : in  std_logic_vector(255 downto 0) := (others => 'X'); -- data
			snk_write_master_valid  : in  std_logic                      := 'X';             -- valid
			snk_write_master_ready  : out std_logic;                                         -- ready
			csr_irq                 : out std_logic;                                         -- irq
			src_response_data       : out std_logic_vector(255 downto 0);                    -- data
			src_response_valid      : out std_logic;                                         -- valid
			src_response_ready      : in  std_logic                      := 'X';             -- ready
			snk_descriptor_data     : in  std_logic_vector(127 downto 0) := (others => 'X'); -- data
			snk_descriptor_valid    : in  std_logic                      := 'X';             -- valid
			snk_descriptor_ready    : out std_logic;                                         -- ready
			mm_response_waitrequest : out std_logic;                                         -- waitrequest
			mm_response_byteenable  : in  std_logic_vector(3 downto 0)   := (others => 'X'); -- byteenable
			mm_response_address     : in  std_logic                      := 'X';             -- address
			mm_response_readdata    : out std_logic_vector(31 downto 0);                     -- readdata
			mm_response_read        : in  std_logic                      := 'X';             -- read
			src_read_master_data    : out std_logic_vector(255 downto 0);                    -- data
			src_read_master_valid   : out std_logic;                                         -- valid
			src_read_master_ready   : in  std_logic                      := 'X';             -- ready
			snk_read_master_data    : in  std_logic_vector(255 downto 0) := (others => 'X'); -- data
			snk_read_master_valid   : in  std_logic                      := 'X';             -- valid
			snk_read_master_ready   : out std_logic                                          -- ready
		);
	end component dispatcher;

begin

	mode_check : if MODE /= 2 generate
		assert false report "Supplied generics do not match expected generics" severity Failure;
	end generate;

	response_port_check : if RESPONSE_PORT /= 2 generate
		assert false report "Supplied generics do not match expected generics" severity Failure;
	end generate;

	descriptor_interface_check : if DESCRIPTOR_INTERFACE /= 0 generate
		assert false report "Supplied generics do not match expected generics" severity Failure;
	end generate;

	descriptor_fifo_depth_check : if DESCRIPTOR_FIFO_DEPTH /= 128 generate
		assert false report "Supplied generics do not match expected generics" severity Failure;
	end generate;

	enhanced_features_check : if ENHANCED_FEATURES /= 0 generate
		assert false report "Supplied generics do not match expected generics" severity Failure;
	end generate;

	descriptor_width_check : if DESCRIPTOR_WIDTH /= 128 generate
		assert false report "Supplied generics do not match expected generics" severity Failure;
	end generate;

	descriptor_byteenable_width_check : if DESCRIPTOR_BYTEENABLE_WIDTH /= 16 generate
		assert false report "Supplied generics do not match expected generics" severity Failure;
	end generate;

	modular_sgdma_dispatcher_0 : component dispatcher
		generic map (
			MODE                        => 2,
			RESPONSE_PORT               => 2,
			DESCRIPTOR_INTERFACE        => 0,
			DESCRIPTOR_FIFO_DEPTH       => 128,
			ENHANCED_FEATURES           => 0,
			DESCRIPTOR_WIDTH            => 128,
			DESCRIPTOR_BYTEENABLE_WIDTH => 16
		)
		port map (
			clk                     => clk,                                                                                                                                                                                                                                                                --                clock.clk
			reset                   => reset,                                                                                                                                                                                                                                                              --          clock_reset.reset
			csr_writedata           => csr_writedata,                                                                                                                                                                                                                                                      --                  CSR.writedata
			csr_write               => csr_write,                                                                                                                                                                                                                                                          --                     .write
			csr_byteenable          => csr_byteenable,                                                                                                                                                                                                                                                     --                     .byteenable
			csr_readdata            => csr_readdata,                                                                                                                                                                                                                                                       --                     .readdata
			csr_read                => csr_read,                                                                                                                                                                                                                                                           --                     .read
			csr_address             => csr_address,                                                                                                                                                                                                                                                        --                     .address
			descriptor_write        => descriptor_write,                                                                                                                                                                                                                                                   --     Descriptor_Slave.write
			descriptor_waitrequest  => descriptor_waitrequest,                                                                                                                                                                                                                                             --                     .waitrequest
			descriptor_writedata    => descriptor_writedata,                                                                                                                                                                                                                                               --                     .writedata
			descriptor_byteenable   => descriptor_byteenable,                                                                                                                                                                                                                                              --                     .byteenable
			src_write_master_data   => src_write_master_data,                                                                                                                                                                                                                                              -- Write_Command_Source.data
			src_write_master_valid  => src_write_master_valid,                                                                                                                                                                                                                                             --                     .valid
			src_write_master_ready  => src_write_master_ready,                                                                                                                                                                                                                                             --                     .ready
			snk_write_master_data   => snk_write_master_data,                                                                                                                                                                                                                                              --  Write_Response_Sink.data
			snk_write_master_valid  => snk_write_master_valid,                                                                                                                                                                                                                                             --                     .valid
			snk_write_master_ready  => snk_write_master_ready,                                                                                                                                                                                                                                             --                     .ready
			csr_irq                 => csr_irq,                                                                                                                                                                                                                                                            --              csr_irq.irq
			src_response_data       => open,                                                                                                                                                                                                                                                               --          (terminated)
			src_response_valid      => open,                                                                                                                                                                                                                                                               --          (terminated)
			src_response_ready      => '0',                                                                                                                                                                                                                                                                --          (terminated)
			snk_descriptor_data     => "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",                                                                                                                                 --          (terminated)
			snk_descriptor_valid    => '0',                                                                                                                                                                                                                                                                --          (terminated)
			snk_descriptor_ready    => open,                                                                                                                                                                                                                                                               --          (terminated)
			mm_response_waitrequest => open,                                                                                                                                                                                                                                                               --          (terminated)
			mm_response_byteenable  => "0000",                                                                                                                                                                                                                                                             --          (terminated)
			mm_response_address     => '0',                                                                                                                                                                                                                                                                --          (terminated)
			mm_response_readdata    => open,                                                                                                                                                                                                                                                               --          (terminated)
			mm_response_read        => '0',                                                                                                                                                                                                                                                                --          (terminated)
			src_read_master_data    => open,                                                                                                                                                                                                                                                               --          (terminated)
			src_read_master_valid   => open,                                                                                                                                                                                                                                                               --          (terminated)
			src_read_master_ready   => '0',                                                                                                                                                                                                                                                                --          (terminated)
			snk_read_master_data    => "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000", --          (terminated)
			snk_read_master_valid   => '0',                                                                                                                                                                                                                                                                --          (terminated)
			snk_read_master_ready   => open                                                                                                                                                                                                                                                                --          (terminated)
		);

end architecture rtl; -- of soc_system_modular_sgdma_dispatcher_0
