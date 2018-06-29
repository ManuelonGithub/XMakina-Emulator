#pragma once

#include "XMakina_Emulator_entities.h"

#ifndef DEBUGGER_H
#define DEBUGGER_H

extern union XMakina_memory memory;
extern union XMakina_register_file register_file;
extern char program_name[MAX_PROG_NAME_SIZE];

#define BACK_TO_DEBUGGER_MENU 'Q'
#define MENU_HELP 'H'

enum DEBUGGER_MENU_OPTIONS {BREAKPOINT_MENU = 'B', RUN_PROGRAM = 'R', QUIT_PROGRAM = 'Q', REG_FILE_MENU = 'F', MEMORY_MENU = 'M', PROGRAM_STEP = '\0', SANITY_CHECK_MENU = 'S', LOAD_PROGRAM = 'L'};
enum BREAKPOINT_MENU {VIEW_CURRENT_BREAKPOINTS = 'V', SET_MEM_BREAKPOINT = 'M', SET_CLOCK_OFFSET_BREAKPOINT = 'O', PRIORITY_CHANGE_BREAKPOINT = 'P'};
enum VIEW_REG_FILE_MENU {CHANGE_REG_VALUE = 'C'};
enum VIEW_MEMORY_MENU {TOP_BOTTOM_LIMIT_MEM_VIEW = 'L', BASE_AND_RANGE_MEM_VIEW = 'R', CHANGE_MEM_VALUE = 'C'};
enum SANITY_CHECK_MENU {VIEW_CURRENT_SANITY_CHECK = 'S', CHANGE_SANITY_CHECK_VALUE = 'C'};

char debugger();

void breakpoint_menu();
void reg_file_menu();
void memory_menu();
void sanity_check_menu();

#endif // !DEBUGGER_H
