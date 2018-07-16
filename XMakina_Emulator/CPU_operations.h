/*
 * X-Makina Emulator Project - CPU_operations.h
 * Contains the function prototypes and definitions used during a XMakine machine CPU cycle
 * (except the execution for the instructions, they have been split between multiple files to keep the code organized)
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
#include "Two_Operand_instructions.h"

#pragma once
#ifndef CPU_OPERATIONS_H
#define CPU_OPERATIONS_H

#define INST_CATEGORY(INST) ((INST & 0xC000) >> 14)

#define IX_BRANCH_SIG (sys_reg.IX.bits.multi_purporse >> 2)
#define IX_MEM_ACCESS_REG_INIT_INST_CODE sys_reg.IX.bits.multi_purporse
#define REG_INIT_INST_CODE_START 2
#define SINGLE_REG_INST_CODE_LIMIT 3
#define TWO_OP_INST_CODE_LIMIT 12

#define BRANCHING_OFFSET_WORD(OFFSET) ((OFFSET << 1))

enum INST_CATEGORY { BRANCHING, ALU, MEM_ACCESS_AND_REG_INIT, MEM_ACCESS_REL };
enum INST_TYPES { 
	INVALID_INST = -1, BRANCH_WITH_LINK_INST, CONDITIONAL_BRANCH_INST, 
	TWO_OPERAND_INST, SINGLE_REGISTER_INST, DIRECT_MEMORY_ACCESS_INST, 
	REGISTER_INITIALIZATION_INST, RELATIVE_MEMORY_ACCESS_INST
};

extern XMakina_memory memory;
extern XMakina_register_file reg_file;
extern System_registers sys_reg;
extern Emulation_properties emulation;

/* XMakina_instruction_set pointer union:
 *		- This union minimizes the  necessary bitwise operations during the execute process.
 *		- By containing the bit-formats of the whole XMakina ISA, 
 *		- The system is able to gather all the necessary information with simple conditional logic.
 *		- Union elements:
 *			- "opcode": pointer used to link to the whole instruction word to this union;
 *			- "br_link': Branching with Link instruction type bit format structure. 
 *				Definition found in Branching_instructions.h .
 *			- "cond_br": Conditional Branching instruction type bit format structure.
 *				Definition found in Branching_instructions.h .
 *			- "two_op": Two-Operand instruction type bit format structure. 
 *				Definition found in Two_Operand_instructions.h .
 *			- "single-reg": Single Register instruction type bit format structure. 
 *				Definition found in Memory_access_instructions.h .
 *			- "DMA": Direct Memory Access instruction type bit format structure. 
 *				Definition found in Single_Register_manipulation_and_initialization_instructions.h .
 *			- "reg_init": Register Initialization instruction type bit format structure.
 *				Definition found in Single_Register_manipulation_and_initialization_instructions.h .
 *			- "RMA": Relative Memory Access instruction type bit format structure.
 *				Definition found in Memory_access_instructions.h .
 */
typedef union XMakina_instruction_set {
	unsigned short * opcode;
	branch_with_link_instruction * br_link;
	conditional_branch_instruction * cond_br;
	two_operand_instruction * two_op;
	single_register_instruction * single_reg;
	direct_memory_access_instruction * DMA;
	register_initialization_instruction * reg_init;
	relative_memory_access_instruction * RMA;
} XMakina_instruction_set;

extern char (*conditional_branching_execution[]) (signed short);
extern char (*register_initialization_execution[]) (char, unsigned short);
extern char (*direct_memory_access_execution[]) (char, char, char, char);
extern char (*relative_memory_access_execution[]) (signed short, char, char, char);
extern char (*single_register_execution[]) (char, char);
char(*two_operand_execution[]) (char, char, char, char);;

void CPU_cycle();
void fetch();
void decode();
void execute();

#endif // !CPU_OPERATIONS_H

