set_global_assignment -entity "clock_gen" -library "lib_clock_gen" -name IP_TOOL_NAME "altera_pll"
set_global_assignment -entity "clock_gen" -library "lib_clock_gen" -name IP_TOOL_VERSION "18.1"
set_global_assignment -entity "clock_gen" -library "lib_clock_gen" -name IP_TOOL_ENV "mwpim"
set_global_assignment -library "lib_clock_gen" -name SPD_FILE [file join $::quartus(sip_path) "clock_gen.spd"]

set_global_assignment -library "lib_clock_gen" -name MISC_FILE [file join $::quartus(sip_path) "clock_gen_sim/clock_gen.vho"]
