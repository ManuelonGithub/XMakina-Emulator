/*
* X-Makina Emulator Project - Debugger.h
* Debugger header contains all the function prototypes and definitions that the debugger program utilizes
*
* Programmer: Manuel Burnay
*
*
* Date created: 26/06/2018
*/

#include "XMakina_Emulator_entities.h"

#pragma once
#ifndef DEBUGGER_H
#define DEBUGGER_H

extern XMakina_memory memory;
extern XMakina_register_file reg_file;
extern System_registers sys_reg;
extern Emulation_properties emulation;
extern union XMakina_instruction_set inst_set;


#define MAX_16_BIT_VALUE (1 << 16)
#define MAX_8_BIT_VALUE (1 << 8)
#define MEM_BREAKPOINT_SIZE 32

enum DEBUGGER_MENU_OPTIONS {
	BREAKPOINT_MENU = 'B', RUN_PROGRAM = 'G', CLOSE_PROGRAM = 'X', 
	QUIT_EMULATOR = 'Q', REG_FILE_OPTIONS = 'R', MEMORY_MENU = 'M', 
	LOAD_PROGRAM = 'L', LOAD_DEVICE_FILE = 'D', INST_OPCODE_TEST = 'I'
};
enum BREAKPOINT_MENU { 
	SET_MEM_BREAKPOINT = 'M', BRANCHING_BREAKPOINT = 'B',
	SET_CLOCK_CYCLE_BREAKPOINT = 'C', PRIORITY_CHANGE_BREAKPOINT = 'P', 
	PROGRAM_STEP = 'S'
};

enum VIEW_REG_FILE_OPTIONS { CHANGE_REG_VALUE = 'C', CLEAR_ALL_REGS = 'X' };
enum VIEW_MEMORY_MENU { TOP_BOTTOM_LIMIT_MEM_VIEW = 'L', BASE_AND_OFFSET_MEM_VIEW = 'O', CHANGE_MEM_VALUE = 'C' };
enum COMMON_MENU_OPTIONS { BACK_TO_DEBUGGER_MENU = 'Q', MENU_HELP = 'H' };
enum DEBUGGER_FEATURES_STATE {OFF = -1, ON = 1};

typedef struct debugger_breakpoints {
	unsigned short memory;
	unsigned int clock_cycle;
	unsigned int cycle_count;
	signed char branch;
	unsigned short PC_check;
	unsigned char priority;
	signed char step;
} debugger_breakpoints;

void debugger_main_menu();

void breakpoint_menu();
void view_current_breakpoints();
void set_mem_breakpoint();
void set_clock_cycle_breakpoint();
void toggle_priority_change_breakpoint();

void reg_file_options();
void change_reg_content();
void clear_all_reg_values();

void memory_menu();
void top_bottom_memory_view();
void change_mem_content();
void base_offset_memory_view();

void close_program();

void test_inst_opcode();

void debugger_triggers();

void clear_emulation_properties();

#endif // !DEBUGGER_H
