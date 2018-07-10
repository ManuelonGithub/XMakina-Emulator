
#include "XMakina_Emulator_entities.h"

#pragma once
#ifndef BRANCHING_INSTRUCTIONS_H
#define BRANCHING_INSTRUCTIONS_H


extern union XMakina_memory memory;
extern unsigned short MAR;
extern unsigned short MBR;
extern unsigned short REG_CON_table[REG_OR_CON][XMAKINA_CPU_REG_COUNT];

struct branch_with_link_instruction {
	signed short offset : 13;
	unsigned short inst_type_signature : 1;
	unsigned short inst_category : 2;
};

struct conditional_branch_instruction {
	signed short offset : 10;
	unsigned short inst_code : 3;
	unsigned short inst_type_signature : 1;
	unsigned short inst_category : 2;
};

char BL(signed short label);

char BEQ_BZ(signed short label);
char BNE_BNZ(signed short label);
char BC_BHS(signed short label);
char BNC_BLO(signed short label);
char BN(signed short label);
char BGE(signed short label);
char BLT(signed short label);
char BAL(signed short label);


#endif // !BRANCHING_INSTRUCTIONS_H
