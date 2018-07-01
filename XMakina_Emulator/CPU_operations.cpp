/*
* X-Makina Emulator Project - CPU_operations.cpp
* CPU_operations file contains all the functions that pertain to the XMakina machine emulation
*
* Programmer: Manuel Burnay
*
*
* Date created: 30/06/2018
*/

#include "stdafx.h"
#include "XMakina_Emulator_entities.h"
#include "CPU_operations.h"

char fetch()
{
	if (register_file.ID.PC % 2 != 0) {
		if (register_file.ID.PC == LAST_BYTE) {
			interrupt_return_management();
		}
		else {
			printf("Invalid value inside of Program Counter. (PC = 0x%04X)\n", register_file.ID.PC);
			return;
		}
	}
	else {
		system_registers.MAR = register_file.ID.PC;
		bus(WORD, READ);
		system_registers.IX = system_registers.MBR;
	}
}

void decode()
{
}

void execute()
{
}

void device_management()
{
}

void debugger_tiggers()
{
}

void bus(char byte_word_control, char read_write_control)
{
}

void interrupt_return_management()
{
}
