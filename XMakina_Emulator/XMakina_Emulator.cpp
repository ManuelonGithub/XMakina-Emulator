/*
 * X-Makina Emulator Project - XMakina_Emulator.cpp
 * Contains the Main body and the ctrl C signal catcher.
 *
 * Programmer: Manuel Burnay
 *
 * Rev 1.0: Instructions work as intended and have been properly documented.
 *
 * Date created: 26/06/2018
 */

// NOTE: All C standard libraries used are specified in stdafx.h

#include "stdafx.h"
#include "XMakina_Emulator_entities.h"
#include "Loader.h"
#include "debugger.h"
#include "CPU_operations.h"
#include "Memory_access_handling.h"
#include "Device_Handling.h"

void test_program();
void ctrl_c_signal_detection();

/* Main:
 * Initializes the ctrl C signal catcher, 
 * and starts the XMakina emulation.
 * Once the user is done, it safely closes all entities of the emulation.
 */
int main(int argc, char * argv[])
{
	signal(SIGINT, (_crt_signal_t) ctrl_c_signal_detection);

	//test_program();

	clear_emulation_properties();	// Resetting the emulation properties to ensure they have predictable initial conditions
	debugger_main_menu();			// The emulation starts with the debugger opening. From there the user can load the desired program
									// And enable any breakpoints to allow the emulation to go back to the debugger.
	while (emulation.system_status != 'Q')
	{
		CPU_cycle();
		device_management();

		cache_options.comparison_clk += emulation.run_clk;
		debugger_triggers();

		emulation.run_clk = 0;	// System has a cpu cycle run clock that runs parallel to the system clock.
	}							// It is mainly used to manage the processing time of the output devices.

	close_device_files();
	printf("XMakina emulation has been shut down. Press ENTER to exit. . . .");
	getchar();
	getchar();

	return 0;
}

/* Ctrl C signal detection function:
 * It's called when the system has detected the Ctrl C signal.
 * It simply enables a flag that gets detected by the debugger_triggers function,
 * which will cause the debugger to open.
 *
 * NOTE: TRUE is specifed in XMakina_Emulator_entities.h
 */
void ctrl_c_signal_detection()
{
	emulation.ctrl_C_detected = TRUE;
	signal(SIGINT, (_crt_signal_t)ctrl_c_signal_detection);	// Re-enabling the system to look for the signal.
}

void test_program()
{
	cache_line cache;

	cache.contents.word = 0x1234;

	printf("%02X %02X\n", cache.contents.byte[0], cache.contents.byte[1]);
}