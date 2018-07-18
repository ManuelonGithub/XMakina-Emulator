/*
 * X-Makina Emulator Project - Single_Register_manipulation_and_initialization_instructions.cpp
 * Contains all the functions used when the CPU cycle is executing
 * either a single register instruction or a register initialization instruction.
 *
 * In order to keep the files at a minimum, while keeping everything organized,
 * Both single register and register initialization instructions are covered in this file.
 *
 * Programmer: Manuel Burnay
 *
 * Rev 1.0: Instructions work as intended and have been properly documented.
 *
 * Date created: 10/07/2018
 *
 * General file note: Unless specified, any definitions/macros should be found the file's respective header.
 */

#include "stdafx.h"
#include "Single_Register_manipulation_and_initialization_instructions.h"
#include "Bus_Devices_Interrupt_operations.h"

char (*register_initialization_execution[]) (char, unsigned short) = { MOVL, MOVLZ, MOVH };
char (*single_register_execution[]) (char, char) = { SRA, RRC, SWPB, SXT };

// NOTE: PROCESS_SUCCESS is defined in XMakina_Emulator_entities.h

/*********************** Register Initialization **********************/

/* Move-to-low instruction:
 * Very simple instruction where the value in the opcode is placed
 * in the low byte of the destination register, 
 * without interfering with the register's high byte.
 */
char MOVL(char dst_reg, unsigned short value)
{
	printf("Executing a MOVL instruction.\n");
	reg_file.REG[dst_reg].LSB = value;

	return PROCESS_SUCCESS;
}

/* Move-to-low and zero instruction:
 * Very simple instruction where the value in the opcode is placed
 * in the destination register, overwritting the high byte with 0's.
 */
char MOVLZ(char dst_reg, unsigned short value)
{
	printf("Executing a MOVLZ instruction.\n");
	reg_file.REG[dst_reg].word = value;			

	return PROCESS_SUCCESS;
}


/* Move-to-high instruction:
 * Very simple instruction where the value in the opcode is placed
 * in the high byte of the destination register, 
 * without interfering with the register's low byte.
 */
char MOVH(char dst_reg, unsigned short value)
{
	printf("Executing a MOVH instruction.\n");
	reg_file.REG[dst_reg].MSB = value;

	return PROCESS_SUCCESS;
}


/*********************** Single Register Manipulation *****************/

/* Shift Right Arithmetic function:
 * Function shifts the word or LSB of the destination register arrithmetically.
 * That means that the sign of the number (bit 15 or 7) is perserved, 
 * and the LSBi goes to the carry flag.
 * This effectively divides the number by 2, 
 * and performing the instruction of the same value enough times with result on the value being either 0 or -1.
 * SRA instruction affects the PSW using the update_PSW function algorithm,
 * using the initial value of the destination register as both the source and destination register.
 * 
 * NOTE:
 * BIT_CHANGE, WORD_MSBi, BYTE_MSBi, LSBi and BIT_CHECK are defined in XMakina_Emulator_entities.h
 */
char SRA(char word_byte_control, char dst_reg)
{
	printf("Executing a SRA instruction.\n");

	sys_reg.temp_reg_A = reg_file.REG[dst_reg];	// Using the system's hidden register so the update_PSW function can be used.

	if (word_byte_control == WORD) {
		SINGLE_RIGHT_SHIFT(sys_reg.temp_reg_A.word);
		BIT_CHANGE(sys_reg.temp_reg_A.word, WORD_MSBi, (BIT_CHECK(reg_file.REG[dst_reg].word, WORD_MSBi)));
	}
	else {
		SINGLE_RIGHT_SHIFT(sys_reg.temp_reg_A.LSB);
		BIT_CHANGE(sys_reg.temp_reg_A.LSB, BYTE_MSBi, (BIT_CHECK(reg_file.REG[dst_reg].LSB, BYTE_MSBi)));
	}
	
	update_PSW(reg_file.REG[dst_reg].word, reg_file.REG[dst_reg].word, sys_reg.temp_reg_A.word, word_byte_control);
	reg_file.PSW.C = BIT_CHECK(reg_file.REG[dst_reg].LSB, LSBi);	// Update_PSW does not transfer the dst_reg's LSB to the PSW.C, so must be done here

	reg_file.REG[dst_reg] = sys_reg.temp_reg_A;

	return PROCESS_SUCCESS;
}

/* Rotate Right through Carry function:
 * Function shifts the word or LSB of the destination register, 
 * transfering the value of the carry to the MSBi of the operation, 
 * and the value of the LSBi is tranfered to the carry.
 * SRA instruction affects the PSW using the update_PSW function algorithm,
 * using the initial value of the destination register as both the source and destination register.
 *
 * NOTE:
 * BIT_CHANGE, WORD_MSBi, BYTE_MSBi, LSBi and BIT_CHECK are defined in XMakina_Emulator_entities.h
 */
char RRC(char word_byte_control, char dst_reg)
{
	printf("Executing a RRC instruction.\n");

	sys_reg.temp_reg_A = reg_file.REG[dst_reg];	// Using the system's hidden register so the update_PSW function can be used.

	if (word_byte_control == WORD) {
		SINGLE_RIGHT_SHIFT(sys_reg.temp_reg_A.word);
		BIT_CHANGE(sys_reg.temp_reg_A.word, WORD_MSBi, reg_file.PSW.C);
	}
	else {
		SINGLE_RIGHT_SHIFT(sys_reg.temp_reg_A.LSB);
		BIT_CHANGE(sys_reg.temp_reg_A.LSB, BYTE_MSBi, reg_file.PSW.C);
	}

	update_PSW(reg_file.REG[dst_reg].word, reg_file.REG[dst_reg].word, sys_reg.temp_reg_A.word, word_byte_control);
	reg_file.PSW.C = BIT_CHECK(reg_file.REG[dst_reg].LSB, LSBi);	// Update_PSW does not transfer the dst_reg's LSB to the PSW.C, so must be done here

	reg_file.REG[dst_reg] = sys_reg.temp_reg_A;

	return PROCESS_SUCCESS;
}

/* Swap bytes function:
 * Function swaps the MSB and LSB of the destination register.
 * This version uses the system's hidden temporary register.
 * It has no effect on the PSW.
 */
char SWPB(char word_byte_control, char dst_reg)
{
	printf("Executing a SWPB instruction.\n");

	sys_reg.temp_reg_A = reg_file.REG[dst_reg];

	reg_file.REG[dst_reg].MSB = sys_reg.temp_reg_A.LSB;
	reg_file.REG[dst_reg].LSB = sys_reg.temp_reg_A.MSB;
	
	return PROCESS_SUCCESS;
}

//char SWPB(char word_byte_control, char dst_reg)					// Using XOR swapping algorithm. 
//{																	// Doesn't use temporary registers, but requires a lot more operations.
//	if (reg_file.REG[dst_reg].MSB != reg_file.REG[dst_reg].LSB) {	// Could be benificial to XMakina, where the # of registers is limited.
//		reg_file.REG[dst_reg].MSB ^= reg_file.REG[dst_reg].LSB;
//		reg_file.REG[dst_reg].LSB ^= reg_file.REG[dst_reg].MSB;
//		reg_file.REG[dst_reg].MSB ^= reg_file.REG[dst_reg].LSB;
//	}
//
//	return PROCESS_SUCCESS;
//}

/* Sign extension function:
 * Function extends the sign bit of the register's LSB to the whole word.
 * SXT instruction affects the PSW using the update_PSW function algorithm,
 * using the initial value of the destination register as both the source and destination register.
 *
 * NOTE:
 * BIT_CHECK, BYTE_MSBi and LOW_BYTE_MASK are defined in XMakina_Emulator_entities.h
 */
char SXT(char word_byte_control, char dst_reg)
{
	printf("Executing a SXT instruction.\n");

	sys_reg.temp_reg_A = reg_file.REG[dst_reg];															// BIT_CHECK retreives bit 7's value of destination register
	sys_reg.temp_reg_A.MSB = (BIT_CHECK(reg_file.REG[dst_reg].LSB, BYTE_MSBi) == 1) ? LOW_BYTE_MASK : 0;	// LOW_BYTE_MASK = 0xFF, so if bit 7 = 1 (negative #), MSB = 0xFF, otherwise MSB = 0;
																										
	update_PSW(reg_file.REG[dst_reg].word, reg_file.REG[dst_reg].word, sys_reg.temp_reg_A.word, word_byte_control);

	reg_file.REG[dst_reg] = sys_reg.temp_reg_A;

	return PROCESS_SUCCESS;
}
