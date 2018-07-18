/*
 * X-Makina Emulator Project - debugger.cpp
 * Debugger file contains all the functions that pertaining to the debugger program
 *
 * Programmer: Manuel Burnay
 *
 * Rev 1.0: Instructions work as intended and have been properly documented.
 *
 * Date created: 28/06/2018
 */

#include "stdafx.h"
#include "XMakina_Emulator_entities.h"
#include "debugger.h"
#include "Loader.h"
#include "CPU_operations.h"
#include "Bus_Devices_Interrupt_operations.h"

debugger_breakpoints breakpoints;

/* Debugger Main Menu function:
 * This function serves as the debugger's main menu,
 * as it offers the user several options for services the debugger offers.
 * Many of these options have their own menus.
 * To ease the clutter inherant to a console user interface, 
 * the menu doesn't show the options it can provide to the user, but instead it is behind a "help" option.
 */
void debugger_main_menu()
{
	char menu_option;

	while (1) {
		printf("\n=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~ Xmakina Debugger: Main Menu ~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=\n\n");

		if (strlen(emulation.program_name) == 0) {
			printf("No program loaded.\n\n");
		}
		else {
			printf("%s program has been loaded.\n\n", emulation.program_name);
		}

		menu_option = 0;
		
		printf("Input your option (Input H for menu options):\t");
		scanf_s(" %c", &menu_option, 1);
		menu_option = toupper(menu_option);

		printf("\n\n");

		switch (menu_option)
		{
		case (BREAKPOINT_MENU):
			breakpoint_menu();
			break;

		case (RUN_PROGRAM):
			if (strlen(emulation.program_name) == 0) {
				printf("System hasn't loaded a program yet.\n");
			}
			else {
				emulation.system_status = RUN_PROGRAM;
				return;
			}
			break;

		case (CLOSE_PROGRAM):
			close_program();
			break;

		case (QUIT_EMULATOR):
			emulation.system_status = QUIT_EMULATOR;
			return;
			break;

		case (REG_FILE_OPTIONS):
			reg_file_options();
			break;

		case (MEMORY_MENU):
			memory_menu();
			break;

		case (LOAD_PROGRAM):
			loader();
			break;

		case (LOAD_DEVICE_FILE):
			device_init();
			break;

		case (INST_OPCODE_TEST):
			test_inst_opcode();
			break;

		case (MENU_HELP):
			printf("Main Menu options:\n");
			printf("B = Breakpoint menu | G = \"Go\" or Run program  | Q = Quit program | R = Register File Menu\n");
			printf("M = Memory Menu     | S = Sanity check Options | L = Load file | X = Close Porgram\n");
			printf("I = Instruction Opcode testing\n\n");
			break;

		default:
			printf("Not a valid menu option.\n");
			break;
		}

		printf("\n");
	}
}

/* Breakpoint Menu function:
 * This sub-menu shows the user the current status of its available breakpoints,
 * and offers the user options to set breakpoints for the program.
 */
void breakpoint_menu()
{
	char menu_option;

	while (1) {
		printf("\n=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~ Xmakina Debugger: Breakpoints ~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=\n\n");

		view_current_breakpoints();

		printf("\nInput your option (Input H for menu options):\t");
		scanf_s(" %c", &menu_option, 1);
		menu_option = toupper(menu_option);

		switch (menu_option)
		{
		case (SET_MEM_BREAKPOINT):
			set_mem_breakpoint();
			break;

		case (SET_CPU_CYCLE_BREAKPOINT):
			set_cpu_cycle_breakpoint();
			break;

		case (BRANCHING_BREAKPOINT):
			breakpoints.branch = (breakpoints.branch == OFF) ? ON : OFF;
			breakpoints.PC_check = reg_file.PC.word;
			break;

		case (PRIORITY_CHANGE_BREAKPOINT):
			toggle_priority_change_breakpoint();
			break;

		case (PROGRAM_STEP):
			breakpoints.step = (breakpoints.step == OFF) ? ON : OFF;
			break;

		case (BACK_TO_DEBUGGER_MENU):
			return;
			break;

		case (MENU_HELP):
			printf("\nBreakpoint Menu options:\n\n");
			printf("M = Set Memory Breakpoint\n");
			printf("The program will break once the PC counter matches a memory addess set by the user.\n");
			printf("When a match occurs, the memory breakpoint will reset to being off.\n\n");

			printf("O = Set cpu cycle Breakpoint\n");
			printf("Cpu cycle breakpoint will break the program once the emulation\n");
			printf("has run the amount of cpu cycles set by the user..\n");
			printf("When a match occurs, the memory breakpoint will reset to being off.\n\n");

			printf("B = Toggle Branching Breakpoint\n");
			printf("If enabled, program will break when a branching jump has been detected.\n");
			printf("This breakpoint setting will stay on until toggled otherwise.\n\n");

			printf("P = Toggle Priority Change Breakpoint\n");
			printf("If enabled,the system will memorize the current priority,\n");
			printf("and will make the program will break when a priority change is noticed.\n");
			printf("This breakpoint setting will stay on until toggled otherwise.\n\n");

			printf("S = Toggle program step\n");
			printf("If enabled, program will break every cpu cycle.\n");
			printf("This breakpoint setting will stay on until toggled otherwise.\n\n");

			printf("Q = Back to Main Menu\n\n");
			break;

		default:
			printf("Not a valid option.\n");
			break;
		}
	}
}

/* View Current Breakpoints function:
 * Displays the current status of the breakpoints offered by the debugger.
 */
void view_current_breakpoints()
{
	printf("Memory Breakpoint: \t\t");
	if ((signed short)breakpoints.memory != OFF) {
		printf("0x%04X ", breakpoints.memory);
	}
	else {
		printf("Not enabled.");
	}

	printf("\ncpu cycle Breakpoint: \t");
	if ((signed int)breakpoints.cpu_cycle != OFF) {
		printf("%d cpu cycles", breakpoints.cpu_cycle);
	}
	else {
		printf("Not enabled.");
	}

	printf("\nBranching Breakpoint: \t\t");
	if (breakpoints.branch == ON) {
		printf("Enabled.");
	}
	else {
		printf("Not enabled.");
	}

	printf("\nPriority change breakpoint: \t");
	if ((signed char)breakpoints.priority == OFF) {
		printf("Not enabled.");
	}
	else {
		printf("Enabled.");
	}

	printf("\nProgram stepping: \t\t");
	if (breakpoints.step == ON) {
		printf("Enabled.\n");
	}
	else {
		printf("Not enabled.\n");
	}
}

/* Set Memory Breakpoint function:
 * Runs the user through the process of safely setting/clearing the memory breakpoint.
 */
void set_mem_breakpoint()
{
	unsigned int mem_brkpt;

	printf("\n\nSetting a new memory breakpoint will overwrite the previous set memory breakpoint.\n");
	printf("Inputting an odd value clears the memory breakpoint.\n\n");
	printf("Input the hex value for the memory breakpoint:\t0x");
	scanf_s("%x", &mem_brkpt);

	if ((mem_brkpt % 2) != 0) {		// this operation determines if the value is odd
		breakpoints.memory = OFF;
		printf("\n\n");
	}
	else if (mem_brkpt >= MAX_16_BIT_VALUE) {
		printf("Invalid memory value, cancelling process.\n\n");
	}
	else {
		breakpoints.memory = mem_brkpt;
		printf("\n\n");
	}
}

/* Set CPU Cycle Breakpoint function:
 * Runs the user through the process of safely setting/clearing the cpu cycle breakpoint.
 */
void set_cpu_cycle_breakpoint()
{
	int clk_brkpt;

	printf("\n\nSetting a new cpu cycle breakpoint will overwrite the previous set cpu cycle breakpoint.\n");
	printf("Inputting a negative value clears the cpu cycle breakpoint.\n\n");
	printf("Input the integer value for the cpu cycle breakpoint:\t");
	scanf_s("%d", &clk_brkpt);

	if (clk_brkpt < 0) {
		breakpoints.cpu_cycle = OFF;
		printf("\n\n\n");
	}
	else {
		breakpoints.cpu_cycle = clk_brkpt;
		breakpoints.cycle_count = 1;			// Count set to one, otherwise the program will only break at cpu_cycle + 1
	}
}

/* Toggle Priority Change Breakpoint:
 * Goes through the process of properly toggling on/off the priority change breakpoint.
 */
void toggle_priority_change_breakpoint()
{
	if ((signed char)breakpoints.priority == OFF) {		// The condition operand must typecasted, otherwise the comparison won't be equal when it should (OFF = -1)
		breakpoints.priority = reg_file.PSW.PRIORITY;
	}
	else {
		breakpoints.priority = OFF;
	}
}

/* Register File Options function:
 * Displays the register file and their contents.
 * Offers the user the ability to change the contents of a register.
 *
 * NOTE:
 * XMAKINA_CPU_REG_COUNT is defined in XMakina_Emulator_entities.h
 */
void reg_file_options()
{
	int i;
	char option;

	while (1) {
		printf("\n=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~ Xmakina Debugger: Register File ~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=\n\n");

		/* The three instances of the reg_file.word allows the system to show the contents of each register 
		   in ways that might be useful to the user (Hex, unsigned, and signed) */
		for (i = 0; i < XMAKINA_CPU_REG_COUNT; i++) {
			printf("R%d : 0x%04X  (%d or %d)\n", i, reg_file.REG[i].word, reg_file.REG[i].word, (signed short)reg_file.REG[i].word);
		}

		printf("\nInput 'C' to change a value of a register, 'X' to clear the register file, and 'Q' to go back to main menu.\n");
		printf("Input:\t");
		scanf_s(" %c", &option, 1);
		option = toupper(option);

		printf("\n");

		switch (option)
		{
		case (CHANGE_REG_VALUE):
			change_reg_content();
			break;
		case (CLEAR_ALL_REGS):
			printf("This option clears the contents in the register file. Are you sure you want to wish to continue? (Y/N)\t");
			scanf_s(" %c", &option, 1);
			option = toupper(option);

			switch (option)
			{
			case ('Y'):
				clear_reg_file();
				break;

			default:
				printf("Register file clearing process has been cancelled.\n\n");
				break;
			}

			break;

		case (BACK_TO_DEBUGGER_MENU):
			return;
			break;

		default:
			printf("Not a valid option.\n");
			break;
		}

		printf("\n");
	}
}

/* change Register contents function:
 * Function that is called within the reg_file_options function.
 * It runs the user through the process of safely changing the contents of a register.
 *
 * NOTE:
 * XMAKINA_CPU_REG_COUNT is defined in XMakina_Emulator_entities.h
 */
void change_reg_content()
{
	unsigned int reg_num, new_reg_value;

	printf("Input the register number you want to change:\tR");
	scanf_s(" %d", &reg_num);

	if (reg_num >= 0 && reg_num < XMAKINA_CPU_REG_COUNT) {
		printf("Input the new 16-bit hex value to be placed in R%d:\t0x", reg_num);
		scanf_s(" %x", &new_reg_value);

		if (new_reg_value >= MAX_16_BIT_VALUE) {
			printf("Value is too large to be placed in a register.\n");
		}
		else {
			reg_file.REG[reg_num].word = new_reg_value;
		}
	}
	else {
		printf("Invalid register number.\n");
	}

	printf("\n");
}

/* Clear Register File function:
 * Clears the contents of the whole register file.
 * Useful function when closing/restarting a program or after testing instruction opcodes.
 */
void clear_reg_file()
{
	printf("Clearing register file . . . . .  ");
	memset(reg_file.REG, 0, sizeof(reg_file.REG));				// function safely sets the contents of the memory space allocated to reg_file to 0
	printf("Register File has been successfully cleared.\n\n");
}

/* Memory menu function:
 * This sub-menu gives the user the option to view the contents of several memory locations at a time.
 * It also provides the user with the ability to change the contents of a memory locaiton. 
 */
void memory_menu()
{
	char menu_option;

	while (1) {
		printf("\n=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~ Xmakina Debugger: Memory  ~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=\n\n");

		printf("Input your option (Input H for menu options):\t");
		scanf_s(" %c", &menu_option, 1);
		menu_option = toupper(menu_option);

		printf("\n");

		switch (menu_option)
		{
		case (TOP_BOTTOM_LIMIT_MEM_VIEW):
			top_bottom_memory_view();
			break;

		case (BASE_AND_OFFSET_MEM_VIEW):
			base_offset_memory_view();
			break;

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

		printf("\n");
	}
}

/* Top-Bottom memory view function:
 * Function is called within the memory_menu function.
 * It allows the user to view the machine's memory based on a bottom and top address boundary.
 *
 * NOTE:
 * MEM_SIZE_BYTES is defined in XMakina_Emulator_entities.h
 */
void top_bottom_memory_view()
{
	unsigned int i, bottom_lim, top_lim;

	printf("\nInput the hex value of the lower bound address:\t0x");
	scanf_s(" %x", &bottom_lim);

	if (bottom_lim < MEM_SIZE_BYTES) {
		printf("Input the hex value of the upper bound address:\t0x");
		scanf_s(" %x", &top_lim);

		if (top_lim >= MEM_SIZE_BYTES || top_lim < bottom_lim) {
			printf("Invalid upper limit.\n");
		}
		else {
			printf("\nMemory contents from 0x%04X to 0x%04X:\n\n", bottom_lim, top_lim);
			printf("\t--------------------------\n");
			printf("\t|  Address  |  Contents  |\n");
			printf("\t--------------------------\n");
			for (i = bottom_lim; i <= top_lim; i++) {
				printf("\t|  0x%04X   |    0x%02X    |\n", i, memory.byte[i]);
			}
			printf("\t--------------------------\n\n");
		}
	}
}

/* Change memory content function:
 * Function is called within the memory_menu function.
 * It runs the user through the process of safely changing the contents of a memory location.
 *
 * NOTE:
 * MEM_SIZE_BYTES is defined in XMakina_Emulator_entities.h
 */
void change_mem_content()
{
	unsigned int mem_loc, new_mem_value;

	printf("Input the 16-bit memory location you want to change:\t0x");
	scanf_s(" %d", &mem_loc);

	if (mem_loc >= 0 && mem_loc < MEM_SIZE_BYTES) {
		printf("Input the new 8-bit hex number to be placed in 0x%04X:\t0x", mem_loc);
		scanf_s(" %x", &new_mem_value);

		if (new_mem_value >= MAX_8_BIT_VALUE) {
			printf("Value is too large to be placed in memory.\n");
		}
		else {
			memory.byte[mem_loc] = new_mem_value;
		}
	}
	else {
		printf("Invalid memory location.\n");
	}

	printf("\n");
}

/* Base-Offset memory view function:
 * Function is called within the memory_menu function.
 * It allows the user to view the machine's memory based on a base and a +/- range.
 * Here to add more functionality if the user needs it.
 *
 * For future work, the memory view functions can be altered to also be allowed to 
 * change the contents of the memory they're addressing, or quick erasing of chunks of memory,
 * or any other use a user might think of.
 *
 * NOTE:
 * MEM_SIZE_BYTES is defined in XMakina_Emulator_entities.h
 */
void base_offset_memory_view()
{
	unsigned int base_mem_loc, bottom_lim, top_lim, i;
	int offset;

	printf("\nInput the hex value of the base address:\t0x");
	scanf_s("%x", &base_mem_loc);
	getchar();

	printf("Input the integer value (+/-) of the offset:\t");
	scanf_s("%d", &offset);
	getchar();

	if ((base_mem_loc + offset) < MEM_SIZE_BYTES) {
		if ((base_mem_loc + offset) < base_mem_loc) {
			top_lim = base_mem_loc;
			bottom_lim = (base_mem_loc + offset);
		}
		else {
			top_lim = (base_mem_loc + offset);
			bottom_lim = base_mem_loc;
		}

		printf("\nMemory contents from 0x%04X to 0x%04X:\n\n", bottom_lim, top_lim);
		printf("\t--------------------------\n");
		printf("\t|  Address  |  Contents  |\n");
		printf("\t--------------------------\n");
		for (i = bottom_lim; i <= top_lim; i++) {
			printf("\t|  0x%04X   |    0x%02X    |\n", i, memory.byte[i]);
		}
		printf("\t--------------------------\n\n");
	}
}


/* Close Program process:
 * This allows the user to erase the contents of the XMakina memory,
 * which results in the erasure of the loaded program.
 * The user can use this feature to load a new program without needing to exit out of the emulation.
 */
void close_program()
{
	printf("\n=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~ Xmakina Debugger: Close Program ~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=\n\n");

	char user_response;
	printf("This option clears the contents and properties of the current XMakina emulation.\n Are you sure you want to wish to close %s? (Y/N)\t", emulation.program_name);
	scanf_s(" %c", &user_response, 1);
	user_response = toupper(user_response);

	switch (user_response)
	{
	case ('Y'):
		printf("Closing program . . . . .  ");
		memset(memory.byte, 0, sizeof(memory.byte));
		memset(emulation.program_name, '\0', sizeof(emulation.program_name));	// Using this function here as well due to some problems where re-loading the emulation with another program
		printf("Program has been successfully closed.\n\n");					// caused the previous' program name to bleed into the supposed current program name.
																				// Clearing the whole memory space for program name ensures this program doesn't occur.
		clear_emulation_properties();	

		printf("Do you also wish to clear the register file? (Y/N)\t");
		scanf_s(" %c", &user_response, 1);
		user_response = toupper(user_response);

		if (user_response == 'Y') {
			clear_reg_file();
		}

		break;

	case ('N'):
		printf("Program closing has been cancelled.\n\n");
		break;

	default:
		printf("Invalid input. Program closing has been cancelled.\n\n");
		break;
	}
}

/* Test Instruction Opcode function:
 * This function allows the user to run individual instruction opcodes.
 * Great for testing the emulation or just seeing how it operates without needing to write a whole program.
 * It is a very bare-bones function, where it just utilizes the existing processes in the CPU cycle, \
 * and so will modify the contents of both memory and register file.
 *
 * NOTE:
 * INVALID_INST is defined in CPU_operations.h
 */
void test_inst_opcode()
{
	printf("\n=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~ Xmakina Debugger: Instruction Opcode testing  ~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=\n\n");

	char user_response;
	unsigned int inst;

	printf("This option allows you to test a single instruction opcode.\n");
	printf("It WILL modify the contents in memory and in the register file.\n");
	printf("\nDo you wish to continue? (Y/N)\t");
	scanf_s(" %c", &user_response, 1);
	user_response = toupper(user_response);

	switch (user_response)
	{
	case ('Y'):
		printf("\nPlease enter instruction opcode:\t");
		scanf_s("%x", &inst);

		/***************************************************** Mini CPU cycle *****************************************************/
		inst_set.opcode = &sys_reg.IX.word;
		sys_reg.IX.word = inst;

		decode();
		if (emulation.current_cycle_status == INVALID_INST) {
			printf("Problem has occurred during the Decode process. Invalid data was fetched (= 0x%04X).\n", sys_reg.IX.word);
			break;
		}

		execute();
		if (emulation.current_cycle_status == INVALID_INST) {
			printf("Invalid data has been attempted to be executed. Invalid data fetched = 0x%04X.\n", sys_reg.IX.word);
		}
		break;

	case ('N'):
		printf("\nInstruction opcode testing has been cancelled.\n\n");
		break;

	default:
		printf("\nInvalid input. Instruction opcode testing has been cancelled.\n\n");
		break;
	}
}

/* Debugger Triggers function:
 * This function performs all the conditional checks and upkeeping when it comes to the debugger breakpoints.
 *
 * NOTE:
 * WORD_STEP, TRUE and FALSE is defined in XMakina_Emulator_entities.h
 */
void debugger_triggers()
{
	if (breakpoints.memory == reg_file.PC.word) {	// Memory breakpoint check
		breakpoints.memory = OFF;
		debugger_main_menu();
	}

	else if (breakpoints.cpu_cycle != OFF) {					// Cpu cycle breakpoint check
		if (breakpoints.cycle_count >= breakpoints.cpu_cycle) {
			breakpoints.cpu_cycle = OFF;
			breakpoints.cycle_count = 0;
			debugger_main_menu();
		}
		else {
			breakpoints.cycle_count++;
		}
	}

	else if (breakpoints.branch == ON) {								// Branch breakpoint check
		if (breakpoints.PC_check != (reg_file.PC.word - WORD_STEP)) {
			debugger_main_menu();
		}
		else {
			breakpoints.PC_check = reg_file.PC.word;
		}
	}

	// Priority breakpoint check
	else if ((signed char)breakpoints.priority != OFF && breakpoints.priority != reg_file.PSW.PRIORITY) {
		debugger_main_menu();
	}

	else if (breakpoints.step == ON) {	// Program stepping check
		debugger_main_menu();
	}

	else if (emulation.ctrl_C_detected == TRUE) {	// Ctrl+C detection check
		printf("\n\n^C has been detected!\n\n");
		emulation.ctrl_C_detected = FALSE;
		debugger_main_menu();
	}
}

/* Clear emulation properties function:
 * used starting up the emulation or closing down a program.
 * ensures the system has a clean slate, and that all properties are initialized properly
 * when starting the machine/new program
 *
 * NOTE:
 * FALSE is defined in XMakina_Emulator_entities.h
 */
void clear_emulation_properties()
{
	emulation.program_name[0] = '\0';
	emulation.system_status = ON;
	emulation.sys_clk = 0;
	emulation.run_clk = 0;
	emulation.ctrl_C_detected = FALSE;

	breakpoints.branch = OFF;
	breakpoints.PC_check = 0;
	breakpoints.cpu_cycle = OFF;
	breakpoints.cycle_count = 0;
	breakpoints.memory = OFF;
	breakpoints.priority = OFF;
	breakpoints.step = OFF;
}

