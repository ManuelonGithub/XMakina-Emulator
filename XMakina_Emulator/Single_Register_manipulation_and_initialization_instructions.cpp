
#include "stdafx.h"
#include "Single_Register_manipulation_and_initialization_instructions.h"
#include "Bus_Devices_Interrupt_operations.h"

char (*register_initialization_execution[]) (char, unsigned short) = { MOVL, MOVLZ, MOVH };

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
	reg_file.PSW.C = WORD_LSBi(reg_file.REG[dst_reg].LSB);

	if (word_byte_control == WORD) {
		reg_file.REG[dst_reg].word >>= 1;
	}
	else {
		reg_file.REG[dst_reg].LSB >>= 1;
	}

	return PROCESS_SUCCESS;
}

char RRC(char word_byte_control, char dst_reg)
{
	reg_file.PSW.unused_bits = WORD_LSBi(reg_file.REG[dst_reg].LSB);	// Using the unused space in the PSW to temporarely store the value of the LSBi, 
																		// so it doesn't get lost during the bit rotation.
	if (word_byte_control == WORD) {
		reg_file.REG[dst_reg].word >>= 1;
		BIT_CHANGE(reg_file.REG[dst_reg].word, 15, reg_file.PSW.C);
	}
	else {
		reg_file.REG[dst_reg].LSB >>= 1;
		BIT_CHANGE(reg_file.REG[dst_reg].LSB, 7, reg_file.PSW.C);
	}

	reg_file.PSW.C = reg_file.PSW.unused_bits;

	return PROCESS_SUCCESS;
}

char SWPB(char word_byte_control, char dst_reg)		// This code might be a bit ugly, but it's an interesting process that allows values to be swapped without needing a temporary value holder.
{
	//unsigned short temp = reg_file.REG[dst_reg];

	return PROCESS_SUCCESS;
}

char SXT(char word_byte_control, char dst_reg)
{


	return PROCESS_SUCCESS;
}
