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

extern union XMakina_memory memory;
extern union XMakina_register_file register_file;
extern char program_name[MAX_PROG_NAME_SIZE];

#define MAX_16_BIT_VALUE (1 << 16)
#define MAX_8_BIT_VALUE (1 << 16)

enum DEBUGGER_MENU_OPTIONS {BREAKPOINT_MENU = 'B', RUN_PROGRAM = 'G', QUIT_PROGRAM = 'Q', REG_FILE_OPTIONS = 'R', MEMORY_MENU = 'M', SANITY_CHECK_OPTIONS = 'S', LOAD_PROGRAM = 'L'};
enum BREAKPOINT_MENU {VIEW_CURRENT_BREAKPOINTS = 'V', SET_MEM_BREAKPOINT = 'M', SET_CLOCK_OFFSET_BREAKPOINT = 'O', PRIORITY_CHANGE_BREAKPOINT = 'P'};
enum VIEW_REG_FILE_OPTIONS {CHANGE_REG_VALUE = 'C'};
enum VIEW_MEMORY_MENU {TOP_BOTTOM_LIMIT_MEM_VIEW = 'L', BASE_AND_OFFSET_MEM_VIEW = 'O', CHANGE_MEM_VALUE = 'C'};
enum SANITY_CHECK_OPTIONS {VIEW_CURRENT_SANITY_CHECK = 'S', CHANGE_SANITY_CHECK_VALUE = 'C'};
enum COMMON_MENU_OPTIONS {BACK_TO_DEBUGGER_MENU = 'Q', MENU_HELP = 'H'};

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

#endif // !DEBUGGER_H
