#include "stdafx.h"
#include "Memory_access_instructions.h"
#include "Bus_Devices_Interrupt_operations.h"

char (*direct_memory_access_execution[]) (char, char, char, char) = { LD, ST };
char (*relative_memory_access_execution[]) (signed short, char, char, char) = { LDR, STR };

/************************ Direct Memory Accessing *********************/
char LD(char action, char word_byte_control, char src_reg, char dst_reg)
{
	printf("Executing a LD instruction.\n");

	switch (action)
	{
	case (NO_ACTION):
		MAR = REG_CON_table[REG][src_reg];
		break;

	case (POST_INCREMEMT):
		MAR = REG_CON_table[REG][src_reg];
		REG_CON_table[REG][src_reg] += (PC_WORD_STEP - word_byte_control);
		break;

	case (POST_DECREMENT):
		MAR = REG_CON_table[REG][src_reg];
		REG_CON_table[REG][src_reg] -= (PC_WORD_STEP - word_byte_control);
		break;

	case (PRE_INCREMENT):
		REG_CON_table[REG][src_reg] += (PC_WORD_STEP - word_byte_control);
		MAR = REG_CON_table[REG][src_reg];
		break;

	case (PRE_DECREMENT):
		REG_CON_table[REG][src_reg] -= (PC_WORD_STEP - word_byte_control);
		MAR = REG_CON_table[REG][src_reg];
		break;

	default:
		return PROCESS_FAILURE;
		break;
	}

	bus(MAR, &MBR, word_byte_control, READ);
	REG_CON_table[REG][dst_reg] = (word_byte_control == WORD) ? MBR : (REG_CON_table[REG][dst_reg] & HIGH_BYTE_MASK) | (MBR & LOW_BYTE_MASK);

	return PROCESS_SUCCESS;
}


/************************ Relative Memory Accessing *******************/
char ST(char action, char word_byte_control, char src_reg, char dst_reg)
{
	printf("Executing a ST instruction.\n");

	switch (action)
	{
	case (NO_ACTION):
		MAR = REG_CON_table[REG][dst_reg];
		break;

	case (POST_INCREMEMT):
		MAR = REG_CON_table[REG][dst_reg];
		REG_CON_table[REG][dst_reg] += (PC_WORD_STEP - word_byte_control);
		break;

	case (POST_DECREMENT):
		MAR = REG_CON_table[REG][dst_reg];
		REG_CON_table[REG][dst_reg] -= (PC_WORD_STEP - word_byte_control);
		break;

	case (PRE_INCREMENT):
		REG_CON_table[REG][dst_reg] += (PC_WORD_STEP - word_byte_control);
		MAR = REG_CON_table[REG][dst_reg];
		break;

	case (PRE_DECREMENT):
		REG_CON_table[REG][dst_reg] -= (PC_WORD_STEP - word_byte_control);
		MAR = REG_CON_table[REG][dst_reg];
		break;

	default:
		return PROCESS_FAILURE;
		break;
	}

	MBR = REG_CON_table[REG][src_reg];
	bus(MAR, &MBR, word_byte_control, WRITE);

	return PROCESS_SUCCESS;
}

char LDR(signed short offset, char word_byte_control, char src_reg, char dst_reg)
{
	MAR = (REG_CON_table[REG][src_reg] + offset);
	bus(MAR, &MBR, word_byte_control, READ);
	bus(MAR, &MBR, word_byte_control, WRITE);
	REG_CON_table[REG][dst_reg] = (word_byte_control == WORD) ? MBR : (REG_CON_table[REG][dst_reg] & HIGH_BYTE_MASK) | (MBR & LOW_BYTE_MASK);

	return PROCESS_SUCCESS;
}

char STR(signed short offset, char word_byte_control, char src_reg, char dst_reg)
{
	MAR = (REG_CON_table[REG][dst_reg] + offset);
	MBR = REG_CON_table[REG][src_reg];
	bus(MAR, &MBR, word_byte_control, WRITE);

	return PROCESS_SUCCESS;
}