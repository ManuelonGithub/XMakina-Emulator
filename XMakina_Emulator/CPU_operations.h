/*
* X-Makina Emulator Project - CPU_operations.h
* Debugger header contains all the function prototypes and definitions used during the XMakine machine emulation
*
* Programmer: Manuel Burnay
*
*
* Date created: 30/06/2018
*/

#include "XMakina_Emulator_entities.h"
#include "Branching_instructions.h"
#include "Single_Register_manipulation_and_initialization_instructions.h"
#include "Memory_access_instructions.h"

#pragma once
#ifndef CPU_OPERATIONS_H
#define CPU_OPERATIONS_H

#define INST_CATEGORY(INST) ((INST & 0xC000) >> 14)

#define BRANCH_INST_TYPE_SIGNATURE(INST) ((INST & 0x2000) >> 13)
#define BRANCHING_INST_CODE(INST) ((INST & 0x1C00) >> 10)
#define BRANCHING_OFFSET_WORD(OFFSET) ((OFFSET << 1))
#define ALU_INST_TYPE_SIGNATURE(INST) ((INST & 0x100) >> 8)
#define ALU_INST_CODE(INST) ((INST & 0x1E00) >> 0)
#define MEM_ACCESS_AND_REG_INIT_INST_TYPE(INST) ((INST & 0x3800) >> 11)
#define REG_INIT_INST_CODE_START 2
#define RELATIVE_MEMORY_ACCESS_INST_TYPE(INST) ((INST & 0x2000) >> 13)

enum INST_CATEGORY {BRANCHING, ALU, MEM_ACCESS_AND_REG_INIT, MEM_ACCESS_REL};
enum INST_TYPES { INVALID_INST = -1, BRANCH_WITH_LINK_INST, CONDITIONAL_BRANCH_INST, TWO_OPERAND_INST, 
					SINGLE_REGISTER_INST, DIRECT_MEMORY_ACCESS_INST, REGISTER_INITIALIZATION_INST, RELATIVE_MEMORY_ACCESS_INST};

extern unsigned int System_clk;

extern union XMakina_memory memory;
extern unsigned short MAR;
extern unsigned short MBR;
extern union XMakina_instruction_set instruction;
extern unsigned short REG_CON_table[REG_OR_CON][XMAKINA_CPU_REG_COUNT];

union XMakina_instruction_set {
	unsigned short opcode;
	struct branch_with_link_instruction subr_br;
	struct conditional_branch_instruction cond_br;
	struct two_operand_instruction two_operand;
	struct single_register_instruction single_reg;
	struct direct_memory_access_instruction dir_mem_access;
	struct register_initialization_instruction reg_init;
	struct relative_memory_access_instruction rel_mem_access;
};

extern char (*conditional_branching_execution[]) (signed short);
extern char (*register_initialization_execution[]) (char, unsigned short);
extern char (*direct_memory_access_execution[]) (char, char, char, char);
extern char (*relative_memory_access_execution[]) (signed short, char, char, char);

char fetch();
char decode();
char execute(char instruction_type);

void debugger_tiggers();

#endif // !CPU_OPERATIONS_H

