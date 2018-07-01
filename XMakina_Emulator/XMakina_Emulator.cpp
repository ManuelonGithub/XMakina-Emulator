/*
* X-Makina Emulator Project - main
* Main body checks for a file to be opened,
* starts the loader program
* and calls the debugger manu once the loading process has complete.
*
* Programmer: Manuel Burnay
*
*
* Date created: 26/06/2018
*/

#include "stdafx.h"
#include "XMakina_Emulator_entities.h"
#include "Loader.h"
#include "debugger.h"
#include "CPU_operations.h"

union XMakina_memory memory;
union XMakina_register_file register_file;
struct XMakina_system_registers system_registers;
char program_name[MAX_PROG_NAME_SIZE];

int main(int argc, char * argv[])
{
	program_name[0] = '\0';	// Initializing the program name to contain nothing in order for some prompts in the debugger to work as intended
	char debugger_out = '\0';

	while (debugger_out != 'Q') {
		debugger_out = debugger_main_menu();

		switch (debugger_out)
		{
		case (RUN_PROGRAM):
			if (fetch() == PROCESS_FAILURE) {
				break;
			}

			decode();


			execute();
			device_management();
			debugger_tiggers();

		default:
			break;
		}
	}

	getchar();

	return 0;
}
