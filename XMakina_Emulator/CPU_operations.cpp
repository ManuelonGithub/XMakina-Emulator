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

char (*conditional_branching[]) (signed short) = { BEQ_BZ, BNE_BNZ, BC_BHS, BNC_BLO, BN, BGE, BLT, BAL };
char (*register_initialization[]) (unsigned short, unsigned short) = { MOVL, MOVLZ, MOVH };
char(*direct_memory_access[]) (unsigned short, unsigned short, unsigned short, unsigned short) = { LD, ST };

char fetch()
{
	if (PC % 2 != 0) {
		if (PC == LAST_BYTE) {
			interrupt_return_process();
		}
		else {
			return PROCESS_FAILURE;
		}
	}
	else {
		MAR = PC;
		bus(MAR, &MBR, WORD, READ);
		IX = MBR;
		PC += WORD_STEP;
	}

	return PROCESS_SUCCESS;
}

char decode()
{
	char category = INST_CATEGORY(IX);

	switch (INST_CATEGORY(IX))
	{
	case (BRANCHING):
		if (BRANCH_INST_TYPE_SIGNATURE(IX) == 0) {
			return BRANCH_WITH_LINK_INST;
		}
		else {
			return CONDITIONAL_BRANCH_INST;
		}
		break;

	case (ALU):
		if (ALU_INST_TYPE_SIGNATURE(IX) == 0) {
			return TWO_OPERAND_INST;
		}
		else {
			return SINGLE_REGISTER_INST;
		}
		break;

	case (MEM_ACCESS_AND_REG_INIT):
		if (MEM_ACCESS_AND_REG_INIT_INST_TYPE(IX) < REG_INIT_INST_CODE_START) {
			return DIRECT_MEMORY_ACCESS_INST;
		}
		else {
			return REGISTER_INITIALIZATION_INST;
		}
		break;

	case (MEM_ACCESS_REL):
		return RELATIVE_MEMORY_ACCESS_INST;
		break;

	default:
		return INVALID_INST;
		break;
	}
}

char execute(char instruction_type)
{
	switch (instruction_type)
	{
	case (BRANCH_WITH_LINK_INST):
		return BL(BRANCHING_OFFSET_WORD(instruction.subr_br.offset));
		break;

	case (CONDITIONAL_BRANCH_INST):
		return (*conditional_branching[instruction.cond_br.inst_code]) (BRANCHING_OFFSET_WORD(instruction.cond_br.offset));
		break;

	case (TWO_OPERAND_INST):
		printf("Executing a two-operand instruction.\n");
		return INVALID_INST;
		break;

	case (SINGLE_REGISTER_INST):
		printf("Executing a single-register instruction.\n");
		return INVALID_INST;
		break;

	case (REGISTER_INITIALIZATION_INST):
		if ((instruction.reg_init.inst_code - REG_INIT_INST_CODE_START) > REG_INIT_INST_CODE_START) {
			return INVALID_INST;
		}
		else {
			return (*register_initialization[(instruction.reg_init.inst_code - REG_INIT_INST_CODE_START)]) (instruction.reg_init.destination_reg, instruction.reg_init.value);
		}
		break;

	case (DIRECT_MEMORY_ACCESS_INST):
		return (*direct_memory_access[instruction.dir_mem_access.inst_code]) (instruction.dir_mem_access.action, 
			instruction.dir_mem_access.word_byte_control, instruction.dir_mem_access.source, instruction.dir_mem_access.destination_reg);
		break;

	case (RELATIVE_MEMORY_ACCESS_INST):
		printf("Executing a relative memory access instruction.\n");
		return INVALID_INST;
		break;

	default:
		return INVALID_INST;
		break;
	}
}

char BL(signed short label)
{
	printf("Executing a BL instruction.\n");
	LR = PC;
	PC += label;
	return PROCESS_SUCCESS;
}

char BEQ_BZ(signed short label)
{
	printf("Executing a BEQ/BZ instruction.\n");
	PC += (PSW_Z == true) ? label : NO_INC;
	return PROCESS_SUCCESS;
}

char BNE_BNZ(signed short label)
{
	printf("Executing a BNE/BNZ instruction.\n");
	PC += (PSW_Z == false) ? label : NO_INC;
	return PROCESS_SUCCESS;
}

char BC_BHS(signed short label)
{
	printf("Executing a BC/BHS instruction.\n");
	PC += (PSW_C == true) ? label : NO_INC;
	return PROCESS_SUCCESS;
}

char BNC_BLO(signed short label)
{
	printf("Executing a BNC/BLO instruction.\n");
	PC += (PSW_C == false) ? label : NO_INC;
	return PROCESS_SUCCESS;
}

char BN(signed short label)
{
	printf("Executing a BN instruction.\n");
	PC += (PSW_Z == true) ? label : WORD_STEP;
	return PROCESS_SUCCESS;
}

char BGE(signed short label)
{
	printf("Executing a BGE instruction.\n");
	PC += ((PSW_Z | PSW_V) == true) ? label : NO_INC;
	return PROCESS_SUCCESS;
}

char BLT(signed short label)
{
	printf("Executing a BLT instruction.\n");
	PC += ((PSW_Z | PSW_V) == true) ? label : NO_INC;
	return PROCESS_SUCCESS;
}

char BAL(signed short label)
{
	printf("Executing a BAL instruction.\n");
	PC += label;
	return PROCESS_SUCCESS;
}

char MOVL(unsigned short dst_reg, unsigned short value)
{
	printf("Executing a MOVL instruction.\n");
	//value |= (REG_CON_table[REG][dst_reg] & 0xFF00);	//
	REG_CON_table[REG][dst_reg] = (REG_CON_table[REG][dst_reg] & HIGH_BYTE_MASK) | (value & LOW_BYTE_MASK);
	return PROCESS_SUCCESS;
}

char MOVLZ(unsigned short dst_reg, unsigned short value)
{
	printf("Executing a MOVLZ instruction.\n");
	REG_CON_table[REG][dst_reg] = value;
	return PROCESS_SUCCESS;
}

char MOVH(unsigned short dst_reg, unsigned short value)
{
	printf("Executing a MOVH instruction.\n");
	//value <<= 8;										//
	//value |= (REG_CON_table[REG][dst_reg] & 0x00FF);	//
	REG_CON_table[REG][dst_reg] = (REG_CON_table[REG][dst_reg] & LOW_BYTE_MASK) | (HIGH_BYTE_SHIFT(value) & HIGH_BYTE_MASK);
	return PROCESS_SUCCESS;
}

char LD(unsigned short action, unsigned short word_byte_control, unsigned short src_reg, unsigned short dst_reg)
{
	printf("Executing a LD instruction.\n");

	switch (action)
	{
	case (NO_ACTION):
		MAR = REG_CON_table[REG][src_reg];
		break;

	case (POST_INCREMEMT):
		MAR = REG_CON_table[REG][src_reg];
		REG_CON_table[REG][src_reg] += (WORD_STEP - word_byte_control);
		break;

	case (POST_DECREMENT):
		MAR = REG_CON_table[REG][src_reg];
		REG_CON_table[REG][src_reg] -= (WORD_STEP - word_byte_control);
		break;

	case (PRE_INCREMENT):
		REG_CON_table[REG][src_reg] += (WORD_STEP - word_byte_control);
		MAR = REG_CON_table[REG][src_reg];
		break;

	case (PRE_DECREMENT):
		REG_CON_table[REG][src_reg] -= (WORD_STEP - word_byte_control);
		MAR = REG_CON_table[REG][src_reg];
		break;

	default:
		return PROCESS_FAILURE;
		break;
	}

	bus(MAR, &MBR, word_byte_control, READ);
	REG_CON_table[REG][dst_reg] = (word_byte_control == WORD) ? MBR : (REG_CON_table[REG][dst_reg] & LOW_BYTE_MASK) | (MBR & LOW_BYTE_MASK);

	return PROCESS_SUCCESS;
}

char ST(unsigned short action, unsigned short word_byte_control, unsigned short src_reg, unsigned short dst_reg)
{
	printf("Executing a ST instruction.\n");

	switch (action)
	{
	case (NO_ACTION):
		MAR = REG_CON_table[REG][dst_reg];
		break;

	case (POST_INCREMEMT):
		MAR = REG_CON_table[REG][dst_reg];
		REG_CON_table[REG][dst_reg] += (WORD_STEP - word_byte_control);
		break;

	case (POST_DECREMENT):
		MAR = REG_CON_table[REG][dst_reg];
		REG_CON_table[REG][dst_reg] -= (WORD_STEP - word_byte_control);
		break;

	case (PRE_INCREMENT):
		REG_CON_table[REG][dst_reg] += (WORD_STEP - word_byte_control);
		MAR = REG_CON_table[REG][dst_reg];
		break;

	case (PRE_DECREMENT):
		REG_CON_table[REG][dst_reg] -= (WORD_STEP - word_byte_control);
		MAR = REG_CON_table[REG][dst_reg];
		break;

	default:
		return PROCESS_FAILURE;
		break;
	}

	MBR = REG_CON_table[REG][src_reg];
	bus(MAR, &MBR, word_byte_control, WRITE);

	return PROCESS_SUCCESS;
}

void device_management()
{
}

void debugger_tiggers()
{
}

void bus(unsigned short MAR, unsigned short * MBR, char word_byte_control, char read_write_control)
{
	if (MAR < DEVICE_MEMORY_SPACE) {
		device_memory_access(MAR, MBR, word_byte_control, read_write_control);
	}
	else {
		if (read_write_control == READ) {
			*MBR = (word_byte_control == WORD) ? memory.word[WORD_ADDR_CONV(MAR)] : memory.byte[MAR];
		}
		else {
			if (word_byte_control == WORD) {
				memory.word[WORD_ADDR_CONV(MAR)] = *MBR;
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

void interrupt_return_process()
{
}
