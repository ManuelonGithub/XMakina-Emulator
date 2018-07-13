
#include "stdafx.h"
#include "Two_Operand_instructions.h"

signed short Const_table[]{ 0, 1, 2, 4, 8, 32, 48, -1 };

char(*two_operand_execution[]) (char, char, char, char) = { ADD, ADDC, SUB, SUBC, DADD, CMP, XOR, AND, BIT, BIC, BIS, MOV, SWAP };

unsigned short arrithmetic(unsigned short src, unsigned short dst, unsigned short carry, char word_byte_ctrl)
{
	register_format temp_res, temp_src;

	temp_res.word = dst;
	temp_src.word = src;

	if (word_byte_ctrl == WORD) {
		temp_res.word = (temp_res.word + temp_src.word) + carry;
	}
	else {
		temp_res.LSB = (temp_res.LSB + temp_src.LSB) + carry;
	}

	update_PSW(src, dst, temp_res.word, word_byte_ctrl);

	return temp_res.word;
}

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

char SUB(char REG_or_CON, char word_byte_ctrl, char source, char dst_reg)
{
	if (REG_or_CON == REGISTER) {
		reg_file.REG[dst_reg].word =
			arrithmetic(~reg_file.REG[source].word, reg_file.REG[dst_reg].word, NO_CARRY, word_byte_ctrl);
	}
	else {
		reg_file.REG[dst_reg].word =
			arrithmetic(~Const_table[source], reg_file.REG[dst_reg].word, NO_CARRY, word_byte_ctrl);
	}

	return PROCESS_SUCCESS;
}

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

char DADD(char REG_or_CON, char word_byte_ctrl, char source, char dst_reg)
{

	return PROCESS_SUCCESS;
}

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

char XOR(char REG_or_CON, char word_byte_ctrl, char source, char dst_reg)
{
	register_format temp_res, temp_src;

	temp_src.word = (REG_or_CON == REGISTER) ? reg_file.REG[source].word : Const_table[source];

	if (word_byte_ctrl == WORD) {
		temp_res.word = reg_file.REG[dst_reg].word ^ temp_src.word;
	}
	else {
		temp_res.LSB = reg_file.REG[dst_reg].LSB ^ temp_src.LSB;
	}

	update_PSW(temp_src.word, reg_file.REG[dst_reg].word, temp_res.word, word_byte_ctrl);

	reg_file.REG[dst_reg] = temp_res;

	return PROCESS_SUCCESS;
}

char AND(char REG_or_CON, char word_byte_ctrl, char source, char dst_reg)
{
	register_format temp_res, temp_src;

	temp_src.word = (REG_or_CON == REGISTER) ? reg_file.REG[source].word : Const_table[source];

	if (word_byte_ctrl == WORD) {
		temp_res.word = reg_file.REG[dst_reg].word & temp_src.word;
	}
	else {
		temp_res.LSB = reg_file.REG[dst_reg].LSB & temp_src.LSB;
	}

	update_PSW(temp_src.word, reg_file.REG[dst_reg].word, temp_res.word, word_byte_ctrl);

	reg_file.REG[dst_reg] = temp_res;

	return PROCESS_SUCCESS;
}

char BIT(char REG_or_CON, char word_byte_ctrl, char source, char dst_reg)
{
	register_format temp_res, temp_src;

	temp_src.word = (REG_or_CON == REGISTER) ? reg_file.REG[source].word : Const_table[source];

	if (word_byte_ctrl == WORD) {
		temp_res.word = reg_file.REG[dst_reg].word & temp_src.word;
	}
	else {
		temp_res.LSB = reg_file.REG[dst_reg].LSB & temp_src.LSB;
	}

	update_PSW(temp_src.word, reg_file.REG[dst_reg].word, temp_res.word, word_byte_ctrl);

	return PROCESS_SUCCESS;
}

char BIC(char REG_or_CON, char word_byte_ctrl, char source, char dst_reg)
{
	register_format temp_src;

	temp_src.word = (REG_or_CON == REGISTER) ? reg_file.REG[source].word : Const_table[source];

	if (word_byte_ctrl == WORD) {
		reg_file.REG[dst_reg].word &= ~temp_src.word;
		reg_file.PSW.N = BIT_CHECK(reg_file.REG[dst_reg].word, WORD_MSBi);
	}
	else {
		reg_file.REG[dst_reg].LSB &= ~temp_src.LSB;
		reg_file.PSW.N = BIT_CHECK(reg_file.REG[dst_reg].LSB, BYTE_MSBi);
	}

	reg_file.PSW.Z = (reg_file.REG[dst_reg].word == 0);

	return PROCESS_SUCCESS;
}

char BIS(char REG_or_CON, char word_byte_ctrl, char source, char dst_reg)
{
	register_format temp_src;

	temp_src.word = (REG_or_CON == REGISTER) ? reg_file.REG[source].word : Const_table[source];

	if (word_byte_ctrl == WORD) {
		reg_file.REG[dst_reg].word |= temp_src.word;
		reg_file.PSW.N = BIT_CHECK(reg_file.REG[dst_reg].word, WORD_MSBi);
	}
	else {
		reg_file.REG[dst_reg].LSB |= temp_src.LSB;
		reg_file.PSW.N = BIT_CHECK(reg_file.REG[dst_reg].LSB, BYTE_MSBi);
	}

	reg_file.PSW.Z = (reg_file.REG[dst_reg].word == 0);

	return PROCESS_SUCCESS;
}

char MOV(char REG_or_CON, char word_byte_ctrl, char source, char dst_reg)
{
	register_format temp_src;
	temp_src.word = (REG_or_CON == REGISTER) ? reg_file.REG[source].word : Const_table[source];

	if (word_byte_ctrl == WORD) {
		reg_file.REG[dst_reg].word = temp_src.word;
	}
	else {
		reg_file.REG[dst_reg].LSB = temp_src.LSB;
	}

	return PROCESS_SUCCESS;
}

char SWAP(char REG_or_CON, char word_byte_ctrl, char source, char dst_reg)
{
	register_format temp;
	temp.word = reg_file.REG[dst_reg].word;
	reg_file.REG[dst_reg].word = reg_file.REG[source].word;
	reg_file.REG[source].word = temp.word;

	return PROCESS_SUCCESS;
}
