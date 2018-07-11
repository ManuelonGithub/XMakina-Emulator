
#include "stdafx.h"
#include "Single_Register_manipulation_and_initialization_instructions.h"
#include "Bus_Devices_Interrupt_operations.h"

char (*register_initialization_execution[]) (char, unsigned short) = { MOVL, MOVLZ, MOVH };

/*********************** Register Initialization **********************/
char MOVL(char dst_reg, unsigned short value)
{
	printf("Executing a MOVL instruction.\n");
	reg_file.REG[dst_reg] = LOW_BYTE_OVERWRITE(reg_file.REG[dst_reg], value);

	return PROCESS_SUCCESS;
}

char MOVLZ(char dst_reg, unsigned short value)
{
	printf("Executing a MOVLZ instruction.\n");
	reg_file.REG[dst_reg] = value;

	return PROCESS_SUCCESS;
}

char MOVH(char dst_reg, unsigned short value)
{
	printf("Executing a MOVH instruction.\n");
	value = HIGH_BYTE_SHIFT(value);
	reg_file.REG[dst_reg] = HIGH_BYTE_OVERWRITE(reg_file.REG[dst_reg], value);

	return PROCESS_SUCCESS;
}


/*********************** Single Register Manipulation *****************/
char SRA(char word_byte_control, char dst_reg)
{
	unsigned short value = SINGLE_RIGHT_SHIFT(reg_file.REG[dst_reg]);

	if (word_byte_control == BYTE) {
		value = LOW_BYTE_OVERWRITE(reg_file.REG[dst_reg], BIT_CHANGE(value, 7, CLEAR));
	}

	reg_file.REG[dst_reg] = value;

	return PROCESS_SUCCESS;
}

char RRC(char word_byte_control, char dst_reg)
{
	unsigned short future_carry = reg_file.REG[dst_reg] & 0x1,
		value = SINGLE_RIGHT_SHIFT(reg_file.REG[dst_reg]);

	if (word_byte_control == WORD) {
		BIT_CHANGE(value, 15, reg_file.PSW.C);
	}
	/*else {
		value = LOW_BYTE_OVERWRITE(REG_CON_table[REG][dst_reg], (value );
	}
*/
	return PROCESS_SUCCESS;
}

char SWPB(char word_byte_control, char dst_reg)
{


	return PROCESS_SUCCESS;
}

char SXT(char word_byte_control, char dst_reg)
{


	return PROCESS_SUCCESS;
}
