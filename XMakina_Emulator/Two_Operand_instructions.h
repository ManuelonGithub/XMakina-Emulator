/*
* X-Makina Emulator Project - Two_Operand_instructions.h
* Contains all the function prototypes and definitions used 
* when the CPU cycle is executing a Two-Operand instruction.
*
* Programmer: Manuel Burnay
*
* Rev 1.0: Instructions work as intended and have been properly documented.
*
* Date created: 11/07/2018
*/

#include "XMakina_Emulator_entities.h"

#pragma once
#ifndef TWO_OPERAND_INSTRUCTIONS_H
#define TWO_OPERAND_INSTRUCTIONS_H

#define REGISTER 0
#define CONSTANT 1
#define WORD_OP_CARRY_BIT 17	// The carry bit in a 16-bit arrithmetic operation
#define BYTE_OP_CARRY_BIT 8		// The carry bit in an 8-bit arrithmetic operation
#define NO_CARRY 0
#define SUBBING_TERM 1

extern XMakina_register_file reg_file;
extern System_registers sys_reg;

/* Two-operand intruction:
*		- "Destination Register": 3 bits used to determine the destination register used in the execution.
*		- "source": 3 bits used to determine the source used in the execution.
*			A two-operand source can either be a register or a value from the constant value table
*		- "Word-Byte control": Signals the process to either address the whole word of the operands, or just the Low Byte
*		- "Register-Constant control": Unique to two-operand.
*			Signals the process to either use the register file or the constant value table for the source value.
*		- "Instruction Category signature": Bit that allows the decoding circuitry to
*			distinguish between instruction types that share the same instruction category.
*		- "Intruction code": Bit(s) that allows the decode/execution circuitry
*			to distinguish between instructions within the same instrucition type
*		- Instruction category: The two MSBi's of the instruction opcode.
*			Instructions within the same category don't necessarely share anything in common,
*			it's just a way to efficiently decode the instruction opcode.
*/
typedef struct two_operand_instruction {
	unsigned short dst_reg : 3;
	unsigned short source : 3;
	unsigned short W_B_ctrl : 1;
	unsigned short REG_CON_ctrl : 1;
	unsigned short inst_type_signature : 1;
	unsigned short inst_code : 4;
	unsigned short unused_bits : 1;			// Bit(s) that aren't used to describe any relevant information in the instruction opcode
	unsigned short inst_category : 2;
} two_operand_instruction;

unsigned short arrithmetic(unsigned short src, unsigned short dst, unsigned short carry, char word_byte_ctrl);

char ADD(char REG_or_CON, char word_byte_ctrl, char source, char dst_reg);
char ADDC(char REG_or_CON, char word_byte_ctrl, char source, char dst_reg);
char SUB(char REG_or_CON, char word_byte_ctrl, char source, char dst_reg);
char SUBC(char REG_or_CON, char word_byte_ctrl, char source, char dst_reg);
char DADD(char REG_or_CON, char word_byte_ctrl, char source, char dst_reg);
char CMP(char REG_or_CON, char word_byte_ctrl, char source, char dst_reg);
char XOR(char REG_or_CON, char word_byte_ctrl, char source, char dst_reg);
char AND(char REG_or_CON, char word_byte_ctrl, char source, char dst_reg);
char BIT(char REG_or_CON, char word_byte_ctrl, char source, char dst_reg);
char BIC(char REG_or_CON, char word_byte_ctrl, char source, char dst_reg);
char BIS(char REG_or_CON, char word_byte_ctrl, char source, char dst_reg);
char MOV(char REG_or_CON, char word_byte_ctrl, char source, char dst_reg);
char SWAP(char REG_or_CON, char word_byte_ctrl, char source, char dst_reg);


#endif // !TWO_OPERAND_INSTRUCTIONS_H

