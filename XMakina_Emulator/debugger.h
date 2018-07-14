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
	SANITY_CHECK_OPTIONS = 'S', LOAD_PROGRAM = 'L', INST_OPCODE_TEST = 'I'
};
enum BREAKPOINT_MENU { 
	VIEW_CURRENT_BREAKPOINTS = 'V', SET_MEM_BREAKPOINT = 'M', 
	SET_CLOCK_OFFSET_BREAKPOINT = 'O', PRIORITY_CHANGE_BREAKPOINT = 'P' 
};
enum VIEW_REG_FILE_OPTIONS { CHANGE_REG_VALUE = 'C' };
enum VIEW_MEMORY_MENU { TOP_BOTTOM_LIMIT_MEM_VIEW = 'L', BASE_AND_OFFSET_MEM_VIEW = 'O', CHANGE_MEM_VALUE = 'C' };
enum SANITY_CHECK_OPTIONS { VIEW_CURRENT_SANITY_CHECK = 'S', CHANGE_SANITY_CHECK_VALUE = 'C' };
enum COMMON_MENU_OPTIONS { BACK_TO_DEBUGGER_MENU = 'Q', MENU_HELP = 'H' };
enum DEBUGGER_FEATURES_STATE {OFF = -1};

typedef struct debugger_breakpoints {
	unsigned short memory[MEM_BREAKPOINT_SIZE];
	unsigned int clock_offset;
	unsigned char priority;
	unsigned char step;
} debugger_breakpoints;

char debugger_main_menu();

void breakpoint_menu();

void reg_file_options();
void change_reg_content();

void memory_menu();
void top_bottom_memory_view();
void change_mem_content();
//void base_offset_memory_view();

void sanity_check_options();

void close_program();

void test_inst_opcode();

void debugger_triggers();

#endif // !DEBUGGER_H
