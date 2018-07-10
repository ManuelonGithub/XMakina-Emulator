
#include "stdafx.h"
#include "Single_Register_manipulation_and_initialization_instructions.h"
#include "Bus_Devices_Interrupt_operations.h"

char (*register_initialization_execution[]) (char, unsigned short) = { MOVL, MOVLZ, MOVH };

/*********************** Register Initialization **********************/
char MOVL(char dst_reg, unsigned short value)
{
	printf("Executing a MOVL instruction.\n");
	//value |= (REG_CON_table[REG][dst_reg] & 0xFF00);	//
	REG_CON_table[REG][dst_reg] = LOW_BYTE_OVERWRITE(REG_CON_table[REG][dst_reg], value);

	return PROCESS_SUCCESS;
}

char MOVLZ(char dst_reg, unsigned short value)
{
	printf("Executing a MOVLZ instruction.\n");
	REG_CON_table[REG][dst_reg] = value;

	return PROCESS_SUCCESS;
}

char MOVH(char dst_reg, unsigned short value)
{
	printf("Executing a MOVH instruction.\n");
	//value <<= 8;										//
	//value |= (REG_CON_table[REG][dst_reg] & 0x00FF);	//
	REG_CON_table[REG][dst_reg] = HIGH_BYTE_OVERWRITE(REG_CON_table[REG][dst_reg], value);

	return PROCESS_SUCCESS;
}


/*********************** Single Register Manipulation *****************/
char SRA(char word_byte_control, char dst_reg)
{
	unsigned short value = SINGLE_RIGHT_SHIFT(REG_CON_table[REG][dst_reg]);

	if (word_byte_control == BYTE) {
		value = LOW_BYTE_OVERWRITE(REG_CON_table[REG][dst_reg], BIT_CHANGE(value, 7, CLEAR));
	}

	REG_CON_table[REG][dst_reg] = value;

	return PROCESS_SUCCESS;
}

char RRC(char word_byte_control, char dst_reg)
{
	unsigned short future_carry = REG_CON_table[REG][dst_reg] & 0x1,
		value = SINGLE_RIGHT_SHIFT(REG_CON_table[REG][dst_reg]);

	if (word_byte_control == WORD) {
		BIT_CHANGE(value, 15, PSW_C);
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
