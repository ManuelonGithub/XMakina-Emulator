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
			return PROCESS_FAILURE;
		}
	}
	else {
		system_registers.MAR = register_file.ID.PC;
		bus(system_registers.MAR, &system_registers.MBR, WORD, READ);
		system_registers.IX = system_registers.MBR;
		register_file.ID.PC += 2;
	}

	return PROCESS_SUCCESS;
}

char decode()
{
	char category = INST_CATEGORY(system_registers.IX);
	// BRANCHING, ALU, MEM_ACCESS_REG_INIT, MEM_ACCESS_REL
	switch (category)
	{
	case (BRANCHING):

	case (ALU):

	case (MEM_ACCESS_AND_REG_INIT):

	case (MEM_ACCESS_REL):

	default:
		break;
	}

	return INVALID_INST;
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

void bus(unsigned short MAR, unsigned short * MBR, char byte_word_control, char read_write_control)
{
	if (system_registers.MAR < DEVICE_MEMORY_SPACE) {
		device_memory_access(MAR, MBR, byte_word_control, read_write_control);
	}
	else {
		if (read_write_control == READ) {
			*MBR = (byte_word_control == WORD) ? memory.word[MAR >> 1] : memory.byte[MAR];
		}
		else {
			if (byte_word_control == WORD) {
				memory.word[MAR >> 1] = *MBR;
			}
			else {
				memory.byte[MAR] = (unsigned char)* MBR;
			}
		}
	}
}

void device_memory_access(unsigned short MAR, unsigned short * MBR, char byte_word_control, char read_write_control)
{
}

void interrupt_return_management()
{
}
