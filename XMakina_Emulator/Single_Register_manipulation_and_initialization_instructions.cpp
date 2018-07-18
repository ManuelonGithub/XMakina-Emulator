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
*/

#include "stdafx.h"
#include "Single_Register_manipulation_and_initialization_instructions.h"
#include "Bus_Devices_Interrupt_operations.h"

char (*register_initialization_execution[]) (char, unsigned short) = { MOVL, MOVLZ, MOVH };
char (*single_register_execution[]) (char, char) = { SRA, RRC, SWPB, SXT };


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
char SRA(char word_byte_control, char dst_reg)
{
	printf("Executing a SRA instruction.\n");

	sys_reg.temp_reg = reg_file.REG[dst_reg];

	if (word_byte_control == WORD) {
		SINGLE_RIGHT_SHIFT(sys_reg.temp_reg.word);
		BIT_CHANGE(sys_reg.temp_reg.word, WORD_MSBi, (BIT_CHECK(reg_file.REG[dst_reg].word, WORD_MSBi)));
	}
	else {
		SINGLE_RIGHT_SHIFT(sys_reg.temp_reg.LSB);
		BIT_CHANGE(sys_reg.temp_reg.LSB, BYTE_MSBi, (BIT_CHECK(reg_file.REG[dst_reg].LSB, BYTE_MSBi)));
	}
	
	update_PSW(reg_file.REG[dst_reg].word, reg_file.REG[dst_reg].word, sys_reg.temp_reg.word, word_byte_control);
	reg_file.PSW.C = BIT_CHECK(reg_file.REG[dst_reg].LSB, LSBi);

	reg_file.REG[dst_reg] = sys_reg.temp_reg;

	return PROCESS_SUCCESS;
}

char RRC(char word_byte_control, char dst_reg)
{
	printf("Executing a RRC instruction.\n");

	sys_reg.temp_reg = reg_file.REG[dst_reg];

	if (word_byte_control == WORD) {
		SINGLE_RIGHT_SHIFT(sys_reg.temp_reg.word);
		BIT_CHANGE(sys_reg.temp_reg.word, WORD_MSBi, reg_file.PSW.C);
	}
	else {
		SINGLE_RIGHT_SHIFT(sys_reg.temp_reg.LSB);
		BIT_CHANGE(sys_reg.temp_reg.LSB, BYTE_MSBi, reg_file.PSW.C);
	}

	update_PSW(reg_file.REG[dst_reg].word, reg_file.REG[dst_reg].word, sys_reg.temp_reg.word, word_byte_control);
	reg_file.PSW.C = BIT_CHECK(reg_file.REG[dst_reg].LSB, LSBi);

	reg_file.REG[dst_reg] = sys_reg.temp_reg;

	return PROCESS_SUCCESS;
}

char SWPB(char word_byte_control, char dst_reg)
{
	printf("Executing a SWPB instruction.\n");

	sys_reg.temp_reg = reg_file.REG[dst_reg];

	reg_file.REG[dst_reg].MSB = sys_reg.temp_reg.LSB;
	reg_file.REG[dst_reg].LSB = sys_reg.temp_reg.MSB;
	
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

char SXT(char word_byte_control, char dst_reg)
{
	printf("Executing a SXT instruction.\n");

	sys_reg.temp_reg = reg_file.REG[dst_reg];
	sys_reg.temp_reg.MSB = (LOW_BYTE_MASK & BIT_CHECK(reg_file.REG[dst_reg].LSB, BYTE_MSBi));	// This performs an and between 0xFF and the sign bit of the register's LSB,
																								// causing the result to be 0 if the sign bit is 0, and 1 if the sign bit is 1
	update_PSW(reg_file.REG[dst_reg].word, reg_file.REG[dst_reg].word, sys_reg.temp_reg.word, word_byte_control);

	reg_file.REG[dst_reg] = sys_reg.temp_reg;

	return PROCESS_SUCCESS;
}
