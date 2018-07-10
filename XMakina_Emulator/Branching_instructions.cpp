#include "stdafx.h"
#include "Branching_instructions.h"

char (*conditional_branching_execution[]) (signed short) = { BEQ_BZ, BNE_BNZ, BC_BHS, BNC_BLO, BN, BGE, BLT, BAL };

/****************** Branching with Link Instruction ******************/
char BL(signed short label)
{
	printf("Executing a BL instruction.\n");
	LR = PC;
	PC += label;
	return PROCESS_SUCCESS;
}


/***************** Conditional Branching Instructions ****************/
char BEQ_BZ(signed short label)
{
	printf("Executing a BEQ/BZ instruction.\n");
	PC += (PSW_Z == 1) ? label : PC_NO_INC;
	return PROCESS_SUCCESS;
}

char BNE_BNZ(signed short label)
{
	printf("Executing a BNE/BNZ instruction.\n");
	PC += (PSW_Z == 0) ? label : PC_NO_INC;
	return PROCESS_SUCCESS;
}

char BC_BHS(signed short label)
{
	printf("Executing a BC/BHS instruction.\n");
	PC += (PSW_C == 1) ? label : PC_NO_INC;
	return PROCESS_SUCCESS;
}

char BNC_BLO(signed short label)
{
	printf("Executing a BNC/BLO instruction.\n");
	PC += (PSW_C == 0) ? label : PC_NO_INC;
	return PROCESS_SUCCESS;
}

char BN(signed short label)
{
	printf("Executing a BN instruction.\n");
	PC += (PSW_Z == 1) ? label : PC_WORD_STEP;
	return PROCESS_SUCCESS;
}

char BGE(signed short label)
{
	printf("Executing a BGE instruction.\n");
	PC += ((PSW_Z ^ PSW_V) == 0) ? label : PC_NO_INC;
	return PROCESS_SUCCESS;
}

char BLT(signed short label)
{
	printf("Executing a BLT instruction.\n");
	PC += ((PSW_Z ^ PSW_V) == 1) ? label : PC_NO_INC;
	return PROCESS_SUCCESS;
}

char BAL(signed short label)
{
	printf("Executing a BAL instruction.\n");
	PC += label;
	return PROCESS_SUCCESS;
}