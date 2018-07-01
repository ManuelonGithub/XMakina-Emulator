/*
* X-Makina Emulator Project - debugger.cpp
* Debugger file contains all the functions that pertaining to the debugger program
*
* Programmer: Manuel Burnay
*
*
* Date created: 28/06/2018
*/

#include "stdafx.h"
#include "XMakina_Emulator_entities.h"
#include "Loader.h"
#include "debugger.h"


/* Debugger Main Menu function:
 * This function serves as the debugger's main menu,
 * as it offers the user several options for services the debugger offers.
 * Many of these options have their own menus.
 * To ease the clutter inherant to a console user interface, 
 * the menu doesn't show the options it can provide to the user, but instead it is behind a "help" option.
 */
char debugger_main_menu()
{
	char menu_option;

	while (1) {
		printf("\n=~=~=~=~=~ Xmakina Debugger: Main Menu ~=~=~=~=~=~=~=~=~=~=~\n\n");

		if (strlen(program_name) == 0) {
			printf("No program loaded.\n\n");
		}

		else {
			printf("%s program has been loaded.\n\n", program_name);
		}
		
		printf("MAIN MENU: Input your option (Input H for menu options):\t");
		scanf_s("%c", &menu_option, 1);
		getchar();							// This extra "getchar()" will be seen whenever there's an "scanf" in the code. 
											// This is because the system can register 2 enter key presses when the user inputs an option, which degrades the user interface experience.
		menu_option = toupper(menu_option);

		switch (menu_option)
		{
		case (BREAKPOINT_MENU):
			breakpoint_menu();
			break;

		case (RUN_PROGRAM):
			if (strlen(program_name) == 0) {
				printf("System hasn't loaded a program yet.\n");
			}
			else {
				return RUN_PROGRAM;
			}
			break;

		case (QUIT_PROGRAM):
			return QUIT_PROGRAM;
			break;

		case (REG_FILE_OPTIONS):
			reg_file_options();
			break;

		case (MEMORY_MENU):
			memory_menu();
			break;

		case (SANITY_CHECK_OPTIONS):
			sanity_check_options();
			break;

		case (LOAD_PROGRAM):
			loader();
			break;

		case (MENU_HELP):
			printf("\nMain Menu options:\n");
			printf("B = Breakpoint menu | G = \"Go\" or Run program  | Q = Quit program | R = Register File Menu\n");
			printf("M = Memory Menu     | S = Sanity check Options | L = Load program\n");
			break;

		default:
			printf("Not a valid menu option.\n");
			break;
		}
	}
}

/* Breakpoint Menu function:
 * This sub-menu offers the user options to set breakpoints for the breakpoint, 
 * and to view active breakpoints.
 */
void breakpoint_menu()
{
	char menu_option;
	printf("\n=~=~=~=~=~ Xmakina Debugger: Breakpoint Menu ~=~=~=~=~=~=~=~\n");

	printf("\nInput your option (Input H for menu options):\t");
	scanf_s("%c", &menu_option, 1);
	getchar();

	menu_option = toupper(menu_option);

	while (1) {
		switch (menu_option)
		{
		case (VIEW_CURRENT_BREAKPOINTS):

		case (SET_MEM_BREAKPOINT):

		case (SET_CLOCK_OFFSET_BREAKPOINT):

		case (PRIORITY_CHANGE_BREAKPOINT):

		case (BACK_TO_DEBUGGER_MENU):
			return;
			break;

		case (MENU_HELP):
			printf("\nBreakpoint Menu options:\n");
			printf("V = View Current Breakpoints   | M = Set Memory Breakpoint | O = Set Clock Offset Breakpoint\n");
			printf("P = Priority Change Breakpoint | Q = Back to Main Menu\n");
			break;

		default:
			printf("Not a valid option.\n");
			break;
		}
	}
}

/* Register File Options function:
 * Displays the register file and their contents.
 * Offers the user the ability to change the contents of a register.
 */
void reg_file_options()
{
	int i;
	char option;

	while (1) {
		printf("\n=~=~=~=~=~ Xmakina Debugger: Register File ~=~=~=~=~=~=~=~=~\n\n");

		for (i = 0; i < XMAKINA_CPU_REG_COUNT; i++) {
			printf("R%d : 0x%04x\n", i, register_file.reg[i]);
		}

		printf("\nInput 'C' to change a value of a register, 'Q' to go back to main menu.\n");
		printf("Input:\t");
		scanf_s("%c", &option, 1);
		getchar();

		option = toupper(option);

		switch (option)
		{
		case (CHANGE_REG_VALUE):
			change_reg_content();
			break;

		case (BACK_TO_DEBUGGER_MENU):
			return;
			break;

		default:
			printf("Not a valid option.\n");
			break;
		}
	}
}

/* change Register contents function:
 * Function that is called within the reg_file_options function.
 * It runs the user through the process of safely changing the contents of a register.
 */
void change_reg_content()
{
	unsigned int reg_num, new_reg_value;

	printf("Input the register number you want to change:\tR");
	scanf_s("%d", &reg_num);
	getchar();

	if (reg_num >= 0 && reg_num < XMAKINA_CPU_REG_COUNT) {
		printf("Input the new 16-bit hex value to be placed in R%d:\t0x", reg_num);
		scanf_s("%x", &new_reg_value);
		getchar();

		if (new_reg_value > MAX_16_BIT_VALUE) {
			printf("Invalid register value.\n");
		}
		else {
			register_file.reg[reg_num] = new_reg_value;
		}
	}
	else {
		printf("Invalid register number.\n");
	}
}

/* Memory menu function:
 * This sub-menu gives the user the option to view the contents of several memory locations at a time.
 * It also provides the user with the ability to change the contents of a memory locaiton. 
 */
void memory_menu()
{
	char menu_option;

	while (1) {
		printf("\n=~=~=~=~=~ Xmakina Debugger: Memory Menu ~=~=~=~=~=~=~=~=~=~\n");

		printf("\nMEMORY MENU: Input your option (Input H for menu options):\t");
		scanf_s("%c", &menu_option, 1);
		getchar();

		menu_option = toupper(menu_option);

		switch (menu_option)
		{
		case (TOP_BOTTOM_LIMIT_MEM_VIEW):
			top_bottom_memory_view();
			break;

		//case (BASE_AND_OFFSET_MEM_VIEW):
		//	base_offset_memory_view();
		//	break;

		case (CHANGE_MEM_VALUE):
			change_mem_content();
			break;

		case (BACK_TO_DEBUGGER_MENU):
			return;
			break;

		case (MENU_HELP):
			printf("Memory Menu options:\n");
			printf("L = View memory contents based on a lower and upper memory address limit.\n");
			printf("O = View memory contents based on a base address and an integer offset.\n");
			printf("C = Change the contents of a memory location.\n");
			printf("Q = Go back to Main Menu.\n");
			break;

		default:
			printf("Not a valid option.\n");
			break;
		}
	}
}

/* Top/Bottom memory view function:
 * Function is called within the memory_menu function.
 * It allows the user to view the machine's memory based on a bottom and top address boundary.
 */
void top_bottom_memory_view()
{
	int i;
	unsigned int bottom_lim, top_lim;

	printf("\nInput the hex value of the lower bound address:\t0x");
	scanf_s("%x", &bottom_lim);
	getchar();

	if (bottom_lim < MEM_SIZE_BYTES) {
		printf("Input the hex value of the upper bound address:\t0x");
		scanf_s("%x", &top_lim);
		getchar();

		if (top_lim > MEM_SIZE_BYTES || top_lim < bottom_lim) {
			printf("Invalid upper limit.\n");
		}
		else {
			printf("\nMemory contents from 0x%04X to 0x%04X:\n\n", bottom_lim, top_lim);
			for (i = bottom_lim; i <= top_lim; i++) {
				printf("Address: 0x%04X  ->  Contents: 0x%02X\n", i, memory.byte[i]);
			}
		}
	}
}

/* Change memory content function:
 * Function is called within the memory_menu function.
 * It runs the user through the process of safely changing the contents of a memory location.
 */
void change_mem_content()
{
	unsigned int mem_loc, new_mem_value;

	printf("Input the 16-bit memory location you want to change:\t0x");
	scanf_s("%d", &mem_loc);
	getchar();

	if (mem_loc >= 0 && mem_loc < MEM_SIZE_BYTES) {
		printf("Input the new 8-bit hex number to be placed in 0x%04X:\t0x", mem_loc);
		scanf_s("%x", &new_mem_value);
		getchar();

		if (new_mem_value > MAX_8_BIT_VALUE) {
			printf("Invalid register value.\n");
		}
		else {
			memory.byte[mem_loc] = new_mem_value;
		}
	}
	else {
		printf("Invalid register number.\n");
	}
}

//void base_offset_memory_view()
//{
//	unsigned int base_mem_loc;
//	int offset, i;
//
//	printf("\nInput the hex value of the base address:\t0x");
//	scanf_s("%x", &base_mem_loc);
//	getchar();
//
//	printf("Input the integer value of the offset:\t");
//	scanf_s("%d", &offset);
//	getchar();
//
//	if ((base_mem_loc + offset) < MEM_SIZE_BYTES) {
//		printf("\nMemory contents from 0x%04X to 0x%04X:\n\n", base_mem_loc, (base_mem_loc + offset);
//		for (i = bottom_lim; i <= top_lim; i++) {
//			printf("Address: 0x%04X  ->  Contents: 0x%02X\n", i, memory.byte[i]);
//		}
//	}
//}

//enum SANITY_CHECK_OPTIONS { VIEW_CURRENT_SANITY_CHECK = 'S', CHANGE_SANITY_CHECK_VALUE = 'C' };

void sanity_check_options()
{
	char menu_option;
	printf("\n=~=~=~=~=~ Xmakina Debugger: Sanity Check Options ~=~=~=~=~=\n");

	printf("Input your option (Input H for menu options):\t");
	scanf_s("%c", &menu_option, 1);
	getchar();

	menu_option = toupper(menu_option);

	while (1) {
		switch (menu_option)
		{
		case (TOP_BOTTOM_LIMIT_MEM_VIEW):

		case (BASE_AND_OFFSET_MEM_VIEW):

		case (CHANGE_MEM_VALUE):

		case (BACK_TO_DEBUGGER_MENU):
			return;
			break;

		case (MENU_HELP):
			printf("Debugger Menu options:\n");
			printf("B = Breakpoint menu | R = Run program | Q = Quit program | F = Register File Menu\n");
			printf("M = Memory Menu     | S = Sanity check Options           | L = Load program\n");
			printf("Not inputting an option and pressing ENTER will step through the program.\n\n");
			break;

		default:
			printf("Not a valid option.\n");
			break;
		}
	}
}

