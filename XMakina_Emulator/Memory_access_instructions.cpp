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
		sys_reg.MAR = reg_file.REG[src_reg];
		break;

	case (POST_INCREMEMT):
		sys_reg.MAR = reg_file.REG[src_reg];
		reg_file.REG[src_reg] += (PC_WORD_STEP - word_byte_control);
		break;

	case (POST_DECREMENT):
		sys_reg.MAR = reg_file.REG[src_reg];
		reg_file.REG[src_reg] -= (PC_WORD_STEP - word_byte_control);
		break;

	case (PRE_INCREMENT):
		reg_file.REG[src_reg] += (PC_WORD_STEP - word_byte_control);
		sys_reg.MAR = reg_file.REG[src_reg];
		break;

	case (PRE_DECREMENT):
		reg_file.REG[src_reg] -= (PC_WORD_STEP - word_byte_control);
		sys_reg.MAR = reg_file.REG[src_reg];
		break;

	default:
		return PROCESS_FAILURE;
		break;
	}

	bus(sys_reg.MAR, &sys_reg.MBR, word_byte_control, READ);
	reg_file.REG[dst_reg] = (word_byte_control == WORD) ? sys_reg.MBR : (reg_file.REG[dst_reg] & HIGH_BYTE_MASK) | (sys_reg.MBR & LOW_BYTE_MASK);	//

	return PROCESS_SUCCESS;
}


/************************ Relative Memory Accessing *******************/
char ST(char action, char word_byte_control, char src_reg, char dst_reg)
{
	printf("Executing a ST instruction.\n");

	switch (action)
	{
	case (NO_ACTION):
		sys_reg.MAR = reg_file.REG[dst_reg];
		break;

	case (POST_INCREMEMT):
		sys_reg.MAR = reg_file.REG[dst_reg];
		reg_file.REG[dst_reg] += (PC_WORD_STEP - word_byte_control);
		break;

	case (POST_DECREMENT):
		sys_reg.MAR = reg_file.REG[dst_reg];
		reg_file.REG[dst_reg] -= (PC_WORD_STEP - word_byte_control);
		break;

	case (PRE_INCREMENT):
		reg_file.REG[dst_reg] += (PC_WORD_STEP - word_byte_control);
		sys_reg.MAR = reg_file.REG[dst_reg];
		break;

	case (PRE_DECREMENT):
		reg_file.REG[dst_reg] -= (PC_WORD_STEP - word_byte_control);
		sys_reg.MAR = reg_file.REG[dst_reg];
		break;

	default:
		return PROCESS_FAILURE;
		break;
	}

	sys_reg.MBR = reg_file.REG[src_reg];
	bus(sys_reg.MAR, &sys_reg.MBR, word_byte_control, WRITE);

	return PROCESS_SUCCESS;
}

char LDR(signed short offset, char word_byte_control, char src_reg, char dst_reg)
{
	sys_reg.MAR = (reg_file.REG[src_reg] + offset);
	bus(sys_reg.MAR, &sys_reg.MBR, word_byte_control, READ);
	reg_file.REG[dst_reg] = (word_byte_control == WORD) ? sys_reg.MBR : (reg_file.REG[dst_reg] & HIGH_BYTE_MASK) | (sys_reg.MBR & LOW_BYTE_MASK);	//

	return PROCESS_SUCCESS;
}

char STR(signed short offset, char word_byte_control, char src_reg, char dst_reg)
{
	sys_reg.MAR = (reg_file.REG[dst_reg] + offset);
	sys_reg.MBR = reg_file.REG[src_reg];
	bus(sys_reg.MAR, &sys_reg.MBR, word_byte_control, WRITE);

	return PROCESS_SUCCESS;
}