
#include "stdafx.h"
#include "Single_Register_manipulation_and_initialization_instructions.h"
#include "Bus_Devices_Interrupt_operations.h"

char (*register_initialization_execution[]) (char, unsigned short) = { MOVL, MOVLZ, MOVH };
char(*single_register_execution[]) (char, char) = { SRA, RRC, SWPB, SXT };

/*********************** Register Initialization **********************/
char MOVL(char dst_reg, unsigned short value)
{
	printf("Executing a MOVL instruction.\n");
	reg_file.REG[dst_reg].LSB = value;

	return PROCESS_SUCCESS;
}

char MOVLZ(char dst_reg, unsigned short value)
{
	printf("Executing a MOVLZ instruction.\n");
	reg_file.REG[dst_reg].word = value;

	return PROCESS_SUCCESS;
}

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

	register_format temp_res = reg_file.REG[dst_reg];

	if (word_byte_control == WORD) {
		SINGLE_RIGHT_SHIFT(temp_res.word);
		BIT_CHANGE(temp_res.word, WORD_MSBi, (BIT_CHECK(reg_file.REG[dst_reg].word, WORD_MSBi)));
	}
	else {
		SINGLE_RIGHT_SHIFT(temp_res.LSB);
		BIT_CHANGE(temp_res.LSB, BYTE_MSBi, (BIT_CHECK(reg_file.REG[dst_reg].LSB, BYTE_MSBi)));
	}
	
	update_PSW(reg_file.REG[dst_reg].word, reg_file.REG[dst_reg].word, temp_res.word, word_byte_control);
	reg_file.PSW.C = BIT_CHECK(reg_file.REG[dst_reg].LSB, LSBi);

	reg_file.REG[dst_reg] = temp_res;

	return PROCESS_SUCCESS;
}

char RRC(char word_byte_control, char dst_reg)
{
	printf("Executing a RRC instruction.\n");

	register_format temp_res = reg_file.REG[dst_reg];

	if (word_byte_control == WORD) {
		SINGLE_RIGHT_SHIFT(temp_res.word);
		BIT_CHANGE(temp_res.word, WORD_MSBi, reg_file.PSW.C);
	}
	else {
		SINGLE_RIGHT_SHIFT(temp_res.LSB);
		BIT_CHANGE(temp_res.LSB, BYTE_MSBi, reg_file.PSW.C);
	}

	update_PSW(reg_file.REG[dst_reg].word, reg_file.REG[dst_reg].word, temp_res.word, word_byte_control);
	reg_file.PSW.C = BIT_CHECK(reg_file.REG[dst_reg].LSB, LSBi);

	reg_file.REG[dst_reg] = temp_res;

	return PROCESS_SUCCESS;
}

char SWPB(char word_byte_control, char dst_reg)
{
	printf("Executing a SWPB instruction.\n");

	register_format temp_res = reg_file.REG[dst_reg];

	reg_file.REG[dst_reg].MSB = temp_res.LSB;
	reg_file.REG[dst_reg].LSB = temp_res.MSB;
	
	return PROCESS_SUCCESS;
}

//char SWPB(char word_byte_control, char dst_reg)					// Using XOR swapping algorithm. 
//{																	// Doesn't use temporary registers. 
//	if (reg_file.REG[dst_reg].MSB != reg_file.REG[dst_reg].LSB) {	// Could be benificial to XMakina, were the # of registers is limited.
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

	register_format temp_res = reg_file.REG[dst_reg];

	temp_res.MSB = (LOW_BYTE_MASK & BIT_CHECK(reg_file.REG[dst_reg].LSB, BYTE_MSBi));
	
	update_PSW(reg_file.REG[dst_reg].word, reg_file.REG[dst_reg].word, temp_res.word, word_byte_control);

	reg_file.REG[dst_reg] = temp_res;

	return PROCESS_SUCCESS;
}
