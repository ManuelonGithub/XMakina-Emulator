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
#include "Two_Operand_instructions.h"


signed short Const_table[]{ 0, 1, 2, 4, 8, 32, 48, -1 };

char(*two_operand_execution[]) (char, char, char, char) = { ADD, ADDC, SUB, SUBC, DADD, CMP, XOR, AND, BIT, BIC, BIS, MOV, SWAP };


/* Arrithmetic function:
 * This function is used with all arrithmetic function (with exception of DADD).
 * It simply performs addition of two values, but that addition can be between positive or negative values,
 * thus a subtraction instruction can use this function simply by inverting the bits of the subtrahend.
 * This function is especially useful since this way only one PSW update algorithm is needed, 
 * since otherwise the algorithm would be different between subtraction and addition. 
 *
 * NOTE:
 * WORD is specifed in XMakina_Emulator_entities.h
 */
unsigned short arrithmetic(unsigned short src, unsigned short dst, unsigned short carry, char word_byte_ctrl)
{
	sys_reg.temp_reg_a.word = dst;
	sys_reg.temp_reg_b.word = src;

	if (word_byte_ctrl == WORD) {
		sys_reg.temp_reg_a.word = (sys_reg.temp_reg_b.word + sys_reg.temp_reg_b.word) + carry;
	}
	else {
		sys_reg.temp_reg_a.LSB = (sys_reg.temp_reg_b.LSB + sys_reg.temp_reg_b.LSB) + carry;
	}

	update_PSW(src, dst, sys_reg.temp_reg_a.word, word_byte_ctrl);

	return sys_reg.temp_reg_a.word;
}

/* Addition function:
 * Utilizes the arrithmetic function. 
 * Determines if the opcode is utilizing a constant or register, 
 * passes the right entity by value to the arrithmetic function
 * and doesn't pass any carry for the carry argument of the arrithmetic function.
 *
 * NOTE:
 * PROCESS_SUCCESS is specifed in XMakina_Emulator_entities.h
 */
char ADD(char REG_or_CON, char word_byte_ctrl, char source, char dst_reg)
{
	if (REG_or_CON == REGISTER) {
		reg_file.REG[dst_reg].word = 
			arrithmetic(reg_file.REG[source].word, reg_file.REG[dst_reg].word, NO_CARRY, word_byte_ctrl);
	}
	else {
		reg_file.REG[dst_reg].word =
			arrithmetic(Const_table[source], reg_file.REG[dst_reg].word, NO_CARRY, word_byte_ctrl);
	}

	return PROCESS_SUCCESS;
}

/* Addition with carry function:
 * Utilizes the arrithmetic function.
 * Determines if the opcode is utilizing a constant or register,
 * passes the right entity by value to the arrithmetic function,
 * as well as PSW.C as the carry argument of the arrithmetic function.
 * The result is stored in the destination register.
 *
 * NOTE:
 * PROCESS_SUCCESS is specifed in XMakina_Emulator_entities.h
 */
char ADDC(char REG_or_CON, char word_byte_ctrl, char source, char dst_reg)
{
	if (REG_or_CON == REGISTER) {
		reg_file.REG[dst_reg].word =
			arrithmetic(reg_file.REG[source].word, reg_file.REG[dst_reg].word, reg_file.PSW.C, word_byte_ctrl);
	}
	else {
		reg_file.REG[dst_reg].word =
			arrithmetic(Const_table[source], reg_file.REG[dst_reg].word, reg_file.PSW.C, word_byte_ctrl);
	}

	return PROCESS_SUCCESS;
}

/* Subtraction function:
 * Utilizes the arrithmetic function.
 * Determines if the opcode is utilizing a constant or register,
 * and passes the inverse/complement of the right entity by value to the arrithmetic function.
 * and passing SUBBING_TERM as the carry argument of the arrithmetic function,
 * which is needed to perform subtraction through addition.
 * The result is stored in the destination register.
 *
 * NOTE:
 * PROCESS_SUCCESS is specifed in XMakina_Emulator_entities.h
 */
char SUB(char REG_or_CON, char word_byte_ctrl, char source, char dst_reg)
{
	if (REG_or_CON == REGISTER) {
		reg_file.REG[dst_reg].word =
			arrithmetic(~reg_file.REG[source].word, reg_file.REG[dst_reg].word, SUBBING_TERM, word_byte_ctrl);
	}
	else {
		reg_file.REG[dst_reg].word =
			arrithmetic(~Const_table[source], reg_file.REG[dst_reg].word, SUBBING_TERM, word_byte_ctrl);
	}

	return PROCESS_SUCCESS;
}

/* Subtraction with carry function:
 * Utilizes the arrithmetic function.
 * Determines if the opcode is utilizing a constant or register,
 * and passes the inverse/complement of the right entity by value to the arrithmetic function.
 * and passing PSW.C as the carry argument of the arrithmetic function.
 * The result is stored in the destination register.
 *
 * NOTE:
 * PROCESS_SUCCESS is specifed in XMakina_Emulator_entities.h
 */
char SUBC(char REG_or_CON, char word_byte_ctrl, char source, char dst_reg)
{
	if (REG_or_CON == REGISTER) {
		reg_file.REG[dst_reg].word =
			arrithmetic(~reg_file.REG[source].word, reg_file.REG[dst_reg].word, reg_file.PSW.C, word_byte_ctrl);
	}
	else {
		reg_file.REG[dst_reg].word =
			arrithmetic(~Const_table[source], reg_file.REG[dst_reg].word, reg_file.PSW.C, word_byte_ctrl);
	}

	return PROCESS_SUCCESS;
}

/* Decimal Addition function:
 *
 *
 * NOTE:
 * PROCESS_SUCCESS is specifed in XMakina_Emulator_entities.h
 */
char DADD(char REG_or_CON, char word_byte_ctrl, char source, char dst_reg)
{

	return PROCESS_SUCCESS;
}

/* Comparison function:
 * Still utilizes the arrithmetic function as a subtraction,
 * but purely for its effects on the PSW.
 * The result of the arrithmetic function isn't stored anywhere.
 *
 * NOTE:
 * PROCESS_SUCCESS is specifed in XMakina_Emulator_entities.h
 */
char CMP(char REG_or_CON, char word_byte_ctrl, char source, char dst_reg)
{
	if (REG_or_CON == REGISTER) {
		arrithmetic(~reg_file.REG[source].word, reg_file.REG[dst_reg].word, NO_CARRY, word_byte_ctrl);
	}
	else {
		arrithmetic(~Const_table[source], reg_file.REG[dst_reg].word, NO_CARRY, word_byte_ctrl);
	}

	return PROCESS_SUCCESS;
}

/* XOR function:
 * Performs a bitwise XOR operation between the value of the destination register
 * and the value specified by the source.
 * The result is stored in the destination register.
 * Uses the update_PSW algorithm.
 *
 * NOTE:
 * WORD and PROCESS_SUCCESS are specifed in XMakina_Emulator_entities.h
 */
char XOR(char REG_or_CON, char word_byte_ctrl, char source, char dst_reg)
{
	sys_reg.temp_reg_b.word = (REG_or_CON == REGISTER) ? reg_file.REG[source].word : Const_table[source];

	if (word_byte_ctrl == WORD) {
		sys_reg.temp_reg_a.word = reg_file.REG[dst_reg].word ^ sys_reg.temp_reg_b.word;
	}
	else {
		sys_reg.temp_reg_a.LSB = reg_file.REG[dst_reg].LSB ^ sys_reg.temp_reg_b.LSB;
	}

	update_PSW(sys_reg.temp_reg_b.word, reg_file.REG[dst_reg].word, sys_reg.temp_reg_a.word, word_byte_ctrl);

	reg_file.REG[dst_reg] = sys_reg.temp_reg_a;

	return PROCESS_SUCCESS;
}

/* AND function:
 * Performs a bitwise AND operation between the value of the destination register
 * and the value specified by the source. 
 * The result is stored in the destination register.
 * Uses the update_PSW algorithm.
 *
 * NOTE:
 * WORD and PROCESS_SUCCESS are specifed in XMakina_Emulator_entities.h
 */
char AND(char REG_or_CON, char word_byte_ctrl, char source, char dst_reg)
{
	sys_reg.temp_reg_b.word = (REG_or_CON == REGISTER) ? reg_file.REG[source].word : Const_table[source];

	if (word_byte_ctrl == WORD) {
		sys_reg.temp_reg_a.word = reg_file.REG[dst_reg].word & sys_reg.temp_reg_b.word;
	}
	else {
		sys_reg.temp_reg_a.LSB = reg_file.REG[dst_reg].LSB & sys_reg.temp_reg_b.LSB;
	}

	update_PSW(sys_reg.temp_reg_b.word, reg_file.REG[dst_reg].word, sys_reg.temp_reg_a.word, word_byte_ctrl);

	reg_file.REG[dst_reg] = sys_reg.temp_reg_a;

	return PROCESS_SUCCESS;
}

/* Bit test function:
 * Performs a bitwise AND operation between the value of the destination register
 * and the value specified by the source. 
 * The result ism't stored anywhere, 
 * the operation is done simply for its effects on the PSW.
 * Uses the update_PSW algorithm.
 *
 * NOTE:
 * WORD and PROCESS_SUCCESS are specifed in XMakina_Emulator_entities.h
 */
char BIT(char REG_or_CON, char word_byte_ctrl, char source, char dst_reg)
{
	sys_reg.temp_reg_b.word = (REG_or_CON == REGISTER) ? reg_file.REG[source].word : Const_table[source];

	if (word_byte_ctrl == WORD) {
		sys_reg.temp_reg_a.word = reg_file.REG[dst_reg].word & sys_reg.temp_reg_b.word;
	}
	else {
		sys_reg.temp_reg_a.LSB = reg_file.REG[dst_reg].LSB & sys_reg.temp_reg_b.LSB;
	}

	update_PSW(sys_reg.temp_reg_b.word, reg_file.REG[dst_reg].word, sys_reg.temp_reg_a.word, word_byte_ctrl);

	return PROCESS_SUCCESS;
}

/* Bit clear function:
 * Function clears the bits in destination register that are specified
 * by the value of the source.
 * It only effects the N and Z flag of PSW, so they are updated without using the algorithm in update_PSW.
 *
 * NOTE:
 * WORD, BIT_CHECK, WORD_MSBi, BYTE_MSBi and PROCESS_SUCCESS are specifed in XMakina_Emulator_entities.h
 */
char BIC(char REG_or_CON, char word_byte_ctrl, char source, char dst_reg)
{
	sys_reg.temp_reg_b.word = (REG_or_CON == REGISTER) ? reg_file.REG[source].word : Const_table[source];

	if (word_byte_ctrl == WORD) {
		reg_file.REG[dst_reg].word &= ~sys_reg.temp_reg_b.word;
		reg_file.PSW.N = BIT_CHECK(reg_file.REG[dst_reg].word, WORD_MSBi);
	}
	else {
		reg_file.REG[dst_reg].LSB &= ~sys_reg.temp_reg_b.LSB;
		reg_file.PSW.N = BIT_CHECK(reg_file.REG[dst_reg].LSB, BYTE_MSBi);
	}

	reg_file.PSW.Z = (reg_file.REG[dst_reg].word == 0);

	return PROCESS_SUCCESS;
}

/* Bit set function:
 * Function sets the bits in destination register that are specified
 * by the value of the source.
 * It only effects the N and Z flag of PSW, so they are updated without using the algorithm in update_PSW.
 *
 * NOTE:
 * WORD, BIT_CHECK, WORD_MSBi, BYTE_MSBi and PROCESS_SUCCESS are specifed in XMakina_Emulator_entities.h
 */
char BIS(char REG_or_CON, char word_byte_ctrl, char source, char dst_reg)
{
	sys_reg.temp_reg_b.word = (REG_or_CON == REGISTER) ? reg_file.REG[source].word : Const_table[source];

	if (word_byte_ctrl == WORD) {
		reg_file.REG[dst_reg].word |= sys_reg.temp_reg_b.word;
		reg_file.PSW.N = BIT_CHECK(reg_file.REG[dst_reg].word, WORD_MSBi);
	}
	else {
		reg_file.REG[dst_reg].LSB |= sys_reg.temp_reg_b.LSB;
		reg_file.PSW.N = BIT_CHECK(reg_file.REG[dst_reg].LSB, BYTE_MSBi);
	}

	reg_file.PSW.Z = (reg_file.REG[dst_reg].word == 0);

	return PROCESS_SUCCESS;
}

/* Move function:
 * Moves the value of the word/byte that is specified by the source to the destination register.
 * It has no effect on the PSW.
 *
 * NOTE:
 * WORD and PROCESS_SUCCESS are specifed in XMakina_Emulator_entities.h
 */
char MOV(char REG_or_CON, char word_byte_ctrl, char source, char dst_reg)
{
	sys_reg.temp_reg_b.word = (REG_or_CON == REGISTER) ? reg_file.REG[source].word : Const_table[source];

	if (word_byte_ctrl == WORD) {
		reg_file.REG[dst_reg].word = sys_reg.temp_reg_b.word;
	}
	else {
		reg_file.REG[dst_reg].LSB = sys_reg.temp_reg_b.LSB;
	}

	return PROCESS_SUCCESS;
}

/* Swap function:
 * Swaps the values of the source and destination register with each other.
 * temp <- dst, dst <- src, src <- temp.
 * It has no effect on the PSW.
 *
 * NOTE:
 * PROCESS_SUCCESS is specifed in XMakina_Emulator_entities.h
 */
char SWAP(char REG_or_CON, char word_byte_ctrl, char source, char dst_reg)
{
	sys_reg.temp_reg_a.word = reg_file.REG[dst_reg].word;
	reg_file.REG[dst_reg].word = reg_file.REG[source].word;
	reg_file.REG[source].word = sys_reg.temp_reg_a.word;

	return PROCESS_SUCCESS;
}
