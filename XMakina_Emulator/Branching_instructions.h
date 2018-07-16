/*
 * X-Makina Emulator Project - Branching_instructions.h
 * Contains all the function prototypes and definitions used when the CPU cycle is executing a Branching instruction
 *
 * In order to keep the files at a minimum, while keeping everything organized, 
 * Branching with link and conditional branching are both covered in this file.
 *
 * Programmer: Manuel Burnay
 *
 * Rev 1.0: Instructions work as intended and have been properly documented.
 *
 * Date created: 10/07/2018
 */

#include "XMakina_Emulator_entities.h"

#pragma once
#ifndef BRANCHING_INSTRUCTIONS_H
#define BRANCHING_INSTRUCTIONS_H


extern XMakina_register_file reg_file;

/* Branching with Link intruction:
*		- Offset: holds the "word-jump" added to the PC counter.
*		- "Instruction Category signature": Bit that allows the decoding circuitry to distinguish
*			between instruction types that share the same instruction category.
*		- Instruction category: The two MSBits of the instruction opcode.
*			Instructions within the same category don't necessarely share anything in common,
*			it's just a way to efficiently decode the instruction opcode.
*/
typedef struct branch_with_link_instruction {
	signed short offset : 13;
	unsigned short inst_type_signature : 1;
	unsigned short inst_category : 2;
} branch_with_link_instruction;

/* Conditional Branching intruction:
*		- Offset: holds the "word-jump" added to the PC counter.
*		- "Intruction code": Bit(s) that allows the decode/execution circuitry to 
*			distinguish between instructions within the same instrucition type
*		- "Instruction Category signature": Bit that allows the decoding circuitry to 
*			distinguish between instruction types that share the same instruction category.
*		- Instruction category: The two MSBits of the instruction opcode.
*			Instructions within the same category don't necessarely share anything in common,
*			it's just a way to efficiently decode the instruction opcode.
*/
typedef struct conditional_branch_instruction {
	signed short offset : 10;
	unsigned short inst_code : 3;
	unsigned short inst_type_signature : 1;
	unsigned short inst_category : 2;
} conditional_branch_instruction;

// Branching with link
char BL(signed short label);

// Conditional Branching
char BEQ_BZ(signed short label);
char BNE_BNZ(signed short label);
char BC_BHS(signed short label);
char BNC_BLO(signed short label);
char BN(signed short label);
char BGE(signed short label);
char BLT(signed short label);
char BAL(signed short label);


#endif // !BRANCHING_INSTRUCTIONS_H
