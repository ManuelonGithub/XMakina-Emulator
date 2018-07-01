/*
* X-Makina Emulator Project - CPU_operations.h
* Debugger header contains all the function prototypes and definitions used during the XMakine machine emulation
*
* Programmer: Manuel Burnay
*
*
* Date created: 30/06/2018
*/

#include "XMakina_Emulator_entities.h"

#pragma once
#ifndef CPU_OPERATIONS_H
#define CPU_OPERATIONS_H

#define PROCESS_FAILURE 0

enum BUS_CONTROLS {BYTE = 0, WORD = 1, READ = 0, WRITE = 1};

extern union XMakina_memory memory;
extern union XMakina_register_file register_file;
extern struct XMakina_system_registers system_registers;

char fetch();
void decode();
void execute();
void device_management();
void debugger_tiggers();

void bus(char byte_word_control, char read_write_control);

void interrupt_return_management();

#endif // !CPU_OPERATIONS_H

