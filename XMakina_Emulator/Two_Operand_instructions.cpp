
#include "Two_Operand_instructions.h"

signed short Const_table[]{ 0, 1, 2, 4, 8, 32, 48, -1 };

char ADD(char REG_or_CON, char word_byte_ctrl, char source, char dst_reg)
{
	int temp = (REG_or_CON == REGISTER) ? reg_file.REG[source].word : Const_table[source];

	if (word_byte_ctrl == WORD) {
		reg_file.PSW.V = ~(WORD_MSBi(reg_file.REG[dst_reg].word) ^ WORD_MSBi(temp));
		temp += reg_file.REG[dst_reg].word;
		reg_file.REG[dst_reg].word = temp;
		reg_file.PSW.C = BIT_CHECK(temp, WORD_OP_CARRY_BIT);
	}
	else {
		reg_file.PSW.V = ~(BYTE_MSBi(reg_file.REG[dst_reg].word) ^ BYTE_MSBi(temp));
		temp = (char)temp + reg_file.REG[dst_reg].LSB;
		reg_file.REG[dst_reg].LSB = temp;
		reg_file.PSW.C = BIT_CHECK(temp, BYTE_OP_CARRY_BIT);
	}



	return PROCESS_SUCCESS;
}

char ADDC(char REG_or_CON, char word_byte_ctrl, char source, char dst_reg)
{

	return PROCESS_SUCCESS;
}

char SUB(char REG_or_CON, char word_byte_ctrl, char source, char dst_reg)
{

	return PROCESS_SUCCESS;
}

char SUBC(char REG_or_CON, char word_byte_ctrl, char source, char dst_reg)
{

	return PROCESS_SUCCESS;
}

char DADD(char REG_or_CON, char word_byte_ctrl, char source, char dst_reg)
{

	return PROCESS_SUCCESS;
}

char CMP(char REG_or_CON, char word_byte_ctrl, char source, char dst_reg)
{

	return PROCESS_SUCCESS;
}

char XOR(char REG_or_CON, char word_byte_ctrl, char source, char dst_reg)
{
	return PROCESS_SUCCESS;
}

char AND(char REG_or_CON, char word_byte_ctrl, char source, char dst_reg)
{
	return 0;
}

char BIT(char REG_or_CON, char word_byte_ctrl, char source, char dst_reg)
{
	return 0;
}

char BIC(char REG_or_CON, char word_byte_ctrl, char source, char dst_reg)
{
	return 0;
}

char BIS(char REG_or_CON, char word_byte_ctrl, char source, char dst_reg)
{
	return 0;
}

char MOV(char REG_or_CON, char word_byte_ctrl, char source, char dst_reg)
{
	return 0;
}

char SWAP(char REG_or_CON, char word_byte_ctrl, char source, char dst_reg)
{
	return 0;
}
