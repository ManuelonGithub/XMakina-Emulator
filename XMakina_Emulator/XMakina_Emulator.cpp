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
	//program_name[0] = '\0';	// Initializing the program name to contain nothing in order for some prompts in the debugger to work as intended
	//char system_status = '\0';
	//char decoded_inst_type = 0;

	//while (system_status != 'Q') {
	//	system_status = debugger_main_menu();

	//	switch (system_status)
	//	{
	//	case (RUN_PROGRAM):
	//		system_status = fetch();

	//		if (system_status == PROCESS_FAILURE) {
	//			printf("Problem has occurred during the Fetch process. Invalid value in PC (= 0x%04X).\n", register_file.ID.PC);
	//			//printf("IX = %04X\n", system_registers.IX);
	//			break;
	//		}

	//		decoded_inst_type = decode();

	//		if (decoded_inst_type == INVALID_INST) {
	//			printf("Problem has occurred during the Decode process. Invalid value in IX (= 0x%04X).\n", system_registers.IX);
	//		}
	//		execute();
	//		device_management();
	//		debugger_tiggers();

	//	default:
	//		break;
	//	}
	//}

	printf("%04X\n", inst);

	getchar();

	return 0;
}
