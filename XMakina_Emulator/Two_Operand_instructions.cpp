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
	sys_reg.temp_reg_A.word = dst;
	sys_reg.temp_reg_B.word = src;

	if (word_byte_ctrl == WORD) {
		sys_reg.temp_reg_A.word += sys_reg.temp_reg_B.word + carry;
	}
	else {
		sys_reg.temp_reg_A.LSB += sys_reg.temp_reg_B.LSB + carry;
	}

	update_PSW(src, dst, sys_reg.temp_reg_A.word, word_byte_ctrl);

	return sys_reg.temp_reg_A.word;
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
	printf("Executing an ADD instruction.\n");

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
	printf("Executing an ADDC instruction.\n");

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
	printf("Executing a SUB instruction.\n");

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
	printf("Executing a SUBC instruction.\n");

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

/* Binary Coded Decimal addition:
 * Performs BCD addition between two 4-bit entities, referred to as nibbles. 
 * It also takes into account a carry value.
 * Each nibble is contained as a base-10 value (0..9), 
 * and if the addition of the three arguments (nibbles 1 & 2 and the carry)
 * is higher or equal to 10 (BCD_VALUE_LIMIT), the carry is asserted and 10 is subtracted from the result.
 */
unsigned short bcd_add(unsigned short nibble_1, unsigned short nibble_2, unsigned short *carry)
{
	unsigned char res; // result of addition

	res = nibble_1 + nibble_2 + *carry;
	if (res >= BCD_VALUE_LIMIT)
	{
		res -= BCD_VALUE_LIMIT;
		*carry = 1;
	}
	else
		*carry = 0;

	return res;
}

/* Decimal Addition function:
 * Function performs Decimal Addition with carry between two 16-bit values.
 * The two 16-bit values are split between 4 nibbles of 4 bits each, that aren't allowed to have a value higher than 9 (10 causes a carry).
 * Each nibbles is added together, checked that they are not higher, 
 *
 * NOTE:
 * WORD and PROCESS_SUCCESS are specifed in XMakina_Emulator_entities.h
 */
char DADD(char REG_or_CON, char word_byte_ctrl, char source, char dst_reg)
{
	printf("Executing a DADD instruction.\n");

	unsigned short carry = reg_file.PSW.C;	// Cannot pass a bit field by reference, so a new variable must be created.

	sys_reg.temp_reg_A.word = reg_file.REG[dst_reg].word;
	sys_reg.temp_reg_B.word = (REG_or_CON == REGISTER) ? reg_file.REG[source].word : Const_table[source];

	sys_reg.temp_reg_A.BCD.nib0 = bcd_add(sys_reg.temp_reg_B.BCD.nib0, sys_reg.temp_reg_A.BCD.nib0, &carry);
	sys_reg.temp_reg_A.BCD.nib1 = bcd_add(sys_reg.temp_reg_B.BCD.nib1, sys_reg.temp_reg_A.BCD.nib1, &carry);

	if (word_byte_ctrl == WORD) {
		sys_reg.temp_reg_A.BCD.nib2 = bcd_add(sys_reg.temp_reg_B.BCD.nib2, sys_reg.temp_reg_A.BCD.nib2, &carry);
		sys_reg.temp_reg_A.BCD.nib3 = bcd_add(sys_reg.temp_reg_B.BCD.nib3, sys_reg.temp_reg_A.BCD.nib3, &carry);
	}

	reg_file.PSW.C = carry;	// The final carry value is transfered to PSW.C

	reg_file.REG[dst_reg] = sys_reg.temp_reg_A;

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
	printf("Executing a CMP instruction.\n");

	if (REG_or_CON == REGISTER) {
		arrithmetic(~reg_file.REG[source].word, reg_file.REG[dst_reg].word, SUBBING_TERM, word_byte_ctrl);
	}
	else {
		arrithmetic(~Const_table[source], reg_file.REG[dst_reg].word, SUBBING_TERM, word_byte_ctrl);
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
	printf("Executing a XOR instruction.\n");

	sys_reg.temp_reg_B.word = (REG_or_CON == REGISTER) ? reg_file.REG[source].word : Const_table[source];

	if (word_byte_ctrl == WORD) {
		sys_reg.temp_reg_A.word = reg_file.REG[dst_reg].word ^ sys_reg.temp_reg_B.word;
	}
	else {
		sys_reg.temp_reg_A.LSB = reg_file.REG[dst_reg].LSB ^ sys_reg.temp_reg_B.LSB;
	}

	update_PSW(sys_reg.temp_reg_B.word, reg_file.REG[dst_reg].word, sys_reg.temp_reg_A.word, word_byte_ctrl);

	reg_file.REG[dst_reg] = sys_reg.temp_reg_A;

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
	printf("Executing an AND instruction.\n");

	sys_reg.temp_reg_A = reg_file.REG[dst_reg];
	sys_reg.temp_reg_B.word = (REG_or_CON == REGISTER) ? reg_file.REG[source].word : Const_table[source];

	if (word_byte_ctrl == WORD) {
		sys_reg.temp_reg_A.word &= sys_reg.temp_reg_B.word;
	}
	else {
		sys_reg.temp_reg_A.LSB &= sys_reg.temp_reg_B.LSB;
	}

	update_PSW(sys_reg.temp_reg_B.word, reg_file.REG[dst_reg].word, sys_reg.temp_reg_A.word, word_byte_ctrl);

	reg_file.REG[dst_reg] = sys_reg.temp_reg_A;

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
	printf("Executing a BIT instruction.\n");

	sys_reg.temp_reg_B.word = (REG_or_CON == REGISTER) ? reg_file.REG[source].word : Const_table[source];

	if (word_byte_ctrl == WORD) {
		sys_reg.temp_reg_A.word = reg_file.REG[dst_reg].word & sys_reg.temp_reg_B.word;
	}
	else {
		sys_reg.temp_reg_A.LSB = reg_file.REG[dst_reg].LSB & sys_reg.temp_reg_B.LSB;
	}

	update_PSW(sys_reg.temp_reg_B.word, reg_file.REG[dst_reg].word, sys_reg.temp_reg_A.word, word_byte_ctrl);

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
	printf("Executing a BIC instruction.\n");

	sys_reg.temp_reg_B.word = (REG_or_CON == REGISTER) ? reg_file.REG[source].word : Const_table[source];

	if (word_byte_ctrl == WORD) {
		reg_file.REG[dst_reg].word &= ~sys_reg.temp_reg_B.word;
		reg_file.PSW.N = BIT_CHECK(reg_file.REG[dst_reg].word, WORD_MSBi);
	}
	else {
		reg_file.REG[dst_reg].LSB &= ~sys_reg.temp_reg_B.LSB;
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
	printf("Executing a BIS instruction.\n");

	sys_reg.temp_reg_B.word = (REG_or_CON == REGISTER) ? reg_file.REG[source].word : Const_table[source];

	if (word_byte_ctrl == WORD) {
		reg_file.REG[dst_reg].word |= sys_reg.temp_reg_B.word;
		reg_file.PSW.N = BIT_CHECK(reg_file.REG[dst_reg].word, WORD_MSBi);
	}
	else {
		reg_file.REG[dst_reg].LSB |= sys_reg.temp_reg_B.LSB;
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
	printf("Executing a MOV instruction.\n");

	sys_reg.temp_reg_B.word = (REG_or_CON == REGISTER) ? reg_file.REG[source].word : Const_table[source];

	if (word_byte_ctrl == WORD) {
		reg_file.REG[dst_reg].word = sys_reg.temp_reg_B.word;
	}
	else {
		reg_file.REG[dst_reg].LSB = sys_reg.temp_reg_B.LSB;
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
	printf("Executing a SWAP instruction.\n");

	sys_reg.temp_reg_A.word = reg_file.REG[dst_reg].word;
	reg_file.REG[dst_reg].word = reg_file.REG[source].word;
	reg_file.REG[source].word = sys_reg.temp_reg_A.word;

	return PROCESS_SUCCESS;
}
