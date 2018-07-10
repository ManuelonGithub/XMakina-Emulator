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


char (*conditional_branching_execution[]) (signed short) = { BEQ_BZ, BNE_BNZ, BC_BHS, BNC_BLO, BN, BGE, BLT, BAL };
char (*register_initialization_execution[]) (char, unsigned short) = { MOVL, MOVLZ, MOVH };
char(*direct_memory_access_execution[]) (char, char, char, char) = { LD, ST };

/* Notes on CPU Operation:
 * PROCESS_FAILURE/PROCESS_SUCCESS are general output flags used through the several steps of a CPU cycle.
 * This is my solution to keep the machine operating in a predictable manner, and not let it deviate to unpredicable behaviour.
 * Examples of these are when the fetch process finds an odd number in the PC counter, or when non-instruction data gets fetched and attempted to be decoded or even executed.
 * (if the bit format matches one of the instruction types, that incorrect data can go up to the execution phase)
 * Specially with the use of arrays of function pointers, it's important to only allow the emulation to work with data it's prepared for.
 * This also is used to notify the user of a problem within the program, or perhaps of the emulation.
 */


/* Fetch function:
 * Emulates part of the CPU cycle that fetches instruction data from memory (whose address is taken from the Program Counter)
 * And places the instruction word in the instruction register, to be decoded and then executed.
 */
char fetch()
{
	if (PC % 2 != 0) {					// Program counter should not have an odd value
		if (PC == LAST_BYTE) {
			interrupt_return_process();	// This is the only exception to the rule
		}
		else {
			return PROCESS_FAILURE;		// If the exception isn't met, the cycle is stopped and the user is notified of the error
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


/* Decode function:
 * Emulates part of the CPU cycle that decodes the instruction data to figure out what instruction needs to be executed, and the operands involved in the execution.
 * This decoder is quite simple, as it only decodes up to the type of instruction is being execution, and not the particular instruction itself.
 * This is because from the instruction type, the particular instruction can be easily found within the instruction code (inst_code),
 * which relates a value to each instruction of the instruction type. 
 * 
 * Notes:
 * There are 4 instruction categories. An instruction category denotes the value in the 2 MSBs (bit 15 and 14) of the instruction word.
 * Finding the category of the instruction is the first step of the decoder.
 * There are 7 instruction types. An instruction type groups instructions that have either an identical or extremely similar operand format.
 * When naming the bits in the bit-format of each instruction type, 
 * "signature bit" is used when describing a bit that allows the process to easily distinguish between two instruction types contained in the same instruction category.
 */
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


/* Execute function:
 * Emulates part of the CPU cycle that executes the instruction decoded, 
 * using the operands laid out in the instruction word as described in the instruction type bit-format described in XMakina_Emulator_entities.h
 * Having sole Execution function allows the CPU cycle process to be laid out nicely, \
 * with the "fetch -> decode -> execute" sequence being present the main (where the machine's cycle is performed).
 * The execution function also deals with filtering out any possible non-instruction data 
 * that might've slipped through the decoder, due to having a similar bit-format.
 */
char execute(char instruction_type)
{
	switch (instruction_type)
	{
	case (BRANCH_WITH_LINK_INST):
		return BL(BRANCHING_OFFSET_WORD(instruction.subr_br.offset));
		break;

	case (CONDITIONAL_BRANCH_INST):
		return (*conditional_branching_execution[instruction.cond_br.inst_code]) (BRANCHING_OFFSET_WORD(instruction.cond_br.offset));
		break;

	case (TWO_OPERAND_INST):
		printf("Executing a two-operand instruction.\n");
		return INVALID_INST;
		break;

	case (SINGLE_REGISTER_INST):
		printf("Executing a single-register instruction.\n");
		return INVALID_INST;
		break;
																										// Register initialization instruction codes start go from 2 to 4,
	case (REGISTER_INITIALIZATION_INST):																// so there needs to be a check if the code goes above (meaning invalid data slipped through the decoder)
		if ((instruction.reg_init.inst_code - REG_INIT_INST_CODE_START) > REG_INIT_INST_CODE_START) {	// Inst_code gets subtracted by 2 to shift the code from 2...4, to 0...2,
			return INVALID_INST;																		// which lets the function pointer array avoid having a complicated solution, 
		}																								// and allows the code to re-use an existing and fitting defined value (REG_INIT_INST_CODE_START)
		else {																							
			return (*register_initialization_execution[(instruction.reg_init.inst_code - REG_INIT_INST_CODE_START)]) (instruction.reg_init.destination_reg, instruction.reg_init.value);
		}
		break;

	case (DIRECT_MEMORY_ACCESS_INST):
		return (*direct_memory_access_execution[instruction.dir_mem_access.inst_code]) (instruction.dir_mem_access.action, 
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

/****************** Branching with Link Instruction ******************/
char BL(signed short label)
{
	printf("Executing a BL instruction.\n");
	LR = PC;
	PC += label;
	return PROCESS_SUCCESS;
}


/***************** Conditional Branching Instructions ****************/
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


/*********************** Register Initialization **********************/
char MOVL(char dst_reg, unsigned short value)
{
	printf("Executing a MOVL instruction.\n");
	//value |= (REG_CON_table[REG][dst_reg] & 0xFF00);	//
	REG_CON_table[REG][dst_reg] = (REG_CON_table[REG][dst_reg] & HIGH_BYTE_MASK) | (value & LOW_BYTE_MASK);
	return PROCESS_SUCCESS;
}

char MOVLZ(char dst_reg, unsigned short value)
{
	printf("Executing a MOVLZ instruction.\n");
	REG_CON_table[REG][dst_reg] = value;
	return PROCESS_SUCCESS;
}

char MOVH(char dst_reg, unsigned short value)
{
	printf("Executing a MOVH instruction.\n");
	//value <<= 8;										//
	//value |= (REG_CON_table[REG][dst_reg] & 0x00FF);	//
	REG_CON_table[REG][dst_reg] = (REG_CON_table[REG][dst_reg] & LOW_BYTE_MASK) | (HIGH_BYTE_SHIFT(value) & HIGH_BYTE_MASK);
	return PROCESS_SUCCESS;
}


/************************ Direct Memory Accessing *********************/
char LD(char action, char word_byte_control, char src_reg, char dst_reg)
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

char ST(char action, char word_byte_control, char src_reg, char dst_reg)
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

char LDR(signed short offset, char word_byte_control, char src_reg, char dst_reg)
{
	return 0;
}

char STR(signed short offset, char word_byte_control, char src_reg, char dst_reg)
{
	return 0;
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

void device_memory_access(unsigned short MAR, unsigned short * MBR, char word_byte_control, char read_write_control)
{
}

void interrupt_return_process()
{
}
