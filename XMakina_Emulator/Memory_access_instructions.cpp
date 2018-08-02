/*
 * X-Makina Emulator Project - Memory_access_instructions.cpp
 * Contains all the function prototypes and definitions used when the CPU cycle is executing a Memory access instruction
 *
 * In order to keep the files at a minimum, while keeping everything organized,
 * Both direct and relative memory addressing are covered in this file.
 *
 * Programmer: Manuel Burnay
 *
 * Rev 1.0: Instructions work as intended and have been properly documented.
 *
 * Date created: 10/07/2018
 */

#include "stdafx.h"
#include "Memory_access_instructions.h"
#include "Memory_access_handling.h"

char (*direct_memory_access_execution[]) (char, char, char, char) = { LD, ST };
char (*relative_memory_access_execution[]) (signed short, char, char, char) = { LDR, STR };

/* General notes:
 * Like it isS mention in XMakina's ISA, the destination register specifies where
 * data goes to, and source register specifies where it originates from.
 * Due to this definition it can be understood why the registers are used inversly between
 * Load and Store instructions.
 */


/************************ Direct Memory Accessing *********************/

/* Notes on LD/ST exectution:
 * In my implementation of the instructions, I use a switch case statement over the *action*
 * the instruction needs to commit.
 * The truth table between the PRPO/DEC/INC was analyzed agaisnt their resulting action,
 * and the following was noticed:
 *   PRPO  DEC  INC  ACTION
 *    0     0    0     (0) -> No action taken
 *    0     0    1     (1) -> Post increment
 *    0     1    0     (2) -> Post decrement
 *    1     0    1     (5) -> Pre increment
 *    1     1    0     (6) -> Pre decrement 
 *    Every other combination is invalid
 *
 * NOTE:
 * WORD_STEP, PROCESS_FAILURE, PROCESS_SUCCESS is defined in XMakina_Emulator_entities.h
 * READ, WRITE and WORD are defined in Bus_Devices_Interrupt_operations.h
 */


/* Load instruction:
 * Loads a destination register's content with a word/byte of memory,
 * whose address is specified by the contents of the source register.
 * Requires an action value, that will determine how to modify the source register.
 */
char LD(char action, char word_byte_control, char src_reg, char dst_reg)
{
	//printf("Executing a LD instruction.\n");

	switch (action)
	{
	case (NO_ACTION):
		sys_reg.MAR = reg_file.REG[src_reg].word;
		break;

	case (POST_INCREMEMT):
		sys_reg.MAR = reg_file.REG[src_reg].word;
		reg_file.REG[src_reg].word += (WORD_STEP - word_byte_control);	// This little operation causes the register to
		break;															// change by 2 if the instruction is addressing a whole word (.W)
																		// or by only 1 if the instruction is addressing a single byte (.B) (where word_byte_control = 1)
	case (POST_DECREMENT):
		sys_reg.MAR = reg_file.REG[src_reg].word;
		reg_file.REG[src_reg].word -= (WORD_STEP - word_byte_control);
		break;

	case (PRE_INCREMENT):
		reg_file.REG[src_reg].word += (WORD_STEP - word_byte_control);
		sys_reg.MAR = reg_file.REG[src_reg].word;
		break;

	case (PRE_DECREMENT):
		reg_file.REG[src_reg].word -= (WORD_STEP - word_byte_control);
		sys_reg.MAR = reg_file.REG[src_reg].word;
		break;

	default:
		return PROCESS_FAILURE;		// The reason all instruction executions return a flag. Plus it's returning a PROCESS_SUCCESS flag brings a nice conclusion to the cycle.
		break;
	}

	bus(word_byte_control, READ);

	if (word_byte_control == WORD) {
		reg_file.REG[dst_reg].word = sys_reg.MBR;
	}
	else {
		reg_file.REG[dst_reg].LSB = sys_reg.MBR;
	}

	return PROCESS_SUCCESS;
}

/* Store instruction:
 * Store a sources register's content with a word/byte of memory,
 * whose address is specified by the contents of the destination register.
 * Requires an action value, that will determine how to modify the destination register.
 */
char ST(char action, char word_byte_control, char src_reg, char dst_reg)
{
	//printf("Executing a ST instruction.\n");

	switch (action)
	{
	case (NO_ACTION):
		sys_reg.MAR = reg_file.REG[dst_reg].word;
		break;

	case (POST_INCREMEMT):
		sys_reg.MAR = reg_file.REG[dst_reg].word;
		reg_file.REG[dst_reg].word += (WORD_STEP - word_byte_control);
		break;

	case (POST_DECREMENT):
		sys_reg.MAR = reg_file.REG[dst_reg].word;
		reg_file.REG[dst_reg].word -= (WORD_STEP - word_byte_control);
		break;

	case (PRE_INCREMENT):
		reg_file.REG[dst_reg].word += (WORD_STEP - word_byte_control);
		sys_reg.MAR = reg_file.REG[dst_reg].word;
		break;

	case (PRE_DECREMENT):
		reg_file.REG[dst_reg].word -= (WORD_STEP - word_byte_control);
		sys_reg.MAR = reg_file.REG[dst_reg].word;
		break;

	default:
		return PROCESS_FAILURE;
		break;
	}

	sys_reg.MBR = reg_file.REG[src_reg].word;
	bus(word_byte_control, WRITE);

	return PROCESS_SUCCESS;
}


/************************ Relative Memory Accessing *******************/

/* Like it was mentioned in the branching instructions, there was no need to 
 * sign extend the offset value, because passing the offset by value to the function
 * and transferring over the value from a 6-bit unsigned entity to a 16-bit signed entity
 * caused the sign extension to occur.
 */

/* Load relative function:
 * Uses the value of the source register and of a 6-bit offset to obtain
 * the effective address that will be used to load the destination register for a 
 * word or byte.
 */
char LDR(signed short offset, char word_byte_control, char src_reg, char dst_reg)
{
	//printf("Executing a LDR instruction.\n");

	sys_reg.MAR = (reg_file.REG[src_reg].word + offset);
	bus(word_byte_control, READ);

	if (word_byte_control == WORD) {
		reg_file.REG[dst_reg].word = sys_reg.MBR;
	}
	else {
		reg_file.REG[dst_reg].LSB = sys_reg.MBR;
	}

	return PROCESS_SUCCESS;
}

/* Store relative function:
 * Uses the value of the destination register and of a 6-bit offset to obtain
 * the effective address that will be used to store the contents of
 * the source register into memory (storing either the whole word or just the LSB).
 */
char STR(signed short offset, char word_byte_control, char src_reg, char dst_reg)
{
	//printf("Executing a STR instruction.\n");

	sys_reg.MAR = (reg_file.REG[dst_reg].word + offset);
	sys_reg.MBR = reg_file.REG[src_reg].word;
	bus(word_byte_control, WRITE);

	return PROCESS_SUCCESS;
}