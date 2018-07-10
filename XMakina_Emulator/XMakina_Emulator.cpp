/*
* X-Makina Emulator Project - main
* Main body initializes most system global variables
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


char program_name[MAX_PROG_NAME_SIZE];
unsigned int System_clk = 0;

union XMakina_memory memory;
unsigned short MAR;
unsigned short MBR;
union XMakina_instruction_set instruction;

char system_status = '\0';

void default_program();
void test_program();

int main(int argc, char * argv[])
{
	default_program();

	//test_program();


	getchar();

	return 0;
}

void default_program()
{

	program_name[0] = '\0';	// Initializing the program name to contain nothing in order for some prompts in the debugger to work as intended
	char run_status;
	char decoded_inst_type = 0;

	while (system_status != QUIT_EMULATOR) {
		system_status = debugger_main_menu();

		switch (system_status)
		{
		case (RUN_PROGRAM):
			run_status = fetch();
			if (run_status == PROCESS_FAILURE) {
				printf("Problem has occurred during the Fetch process. Invalid value in PC (= 0x%04X).\n", PC);
				printf("IX = %04X\n", IX);
				break;
			}

			decoded_inst_type = decode();
			if (decoded_inst_type == INVALID_INST) {
				printf("Problem has occurred during the Decode process. Invalid value in IX (= 0x%04X).\n", IX);
				break;
			}

			run_status = execute(decoded_inst_type);
			if (run_status == INVALID_INST) {
				printf("Invalid data has been attempted to be executed. Data fetched for this cycle = 0x%04X.\n", IX);
				break;
			}
			//device_management();
			debugger_tiggers();

		default:
			break;
		}
	}
}

void test_program()
{
	//unsigned char a = 100, b = 100, c, d = -264;
	//int e;
	//e = (a + b);
	//c = a + b;
	//printf("%x %x %x\n", e, c, d);

	//unsigned char a = 31;
	//char b;

	//b = a;

	//printf("a = %x, b = %x\n", a, b);

	unsigned short a = 0xFFFF, b = BIT_CHECK(a, 15);
	printf("a = %02x, b = %02x\n", a, b);

	BIT_CHANGE(a, 15, 0);
	b = BIT_CHECK(a, 15);

	printf("a = %02x, b = %02x\n", a, b);
}