
#include "XMakina_Emulator_entities.h"

#pragma once

#ifndef SINGLE_REGISTER_MANIPULATION_AND_INITIALIZATION_INSTRUCTIONS_H
#define SINGLE_REGISTER_MANIPULATION_AND_INITIALIZATION_INSTRUCTIONS_H

#define SINGLE_RIGHT_SHIFT(value) (value >> 1)


extern XMakina_register_file reg_file;

/* Single Register intruction:
*		- "Destination Register": 3 bits used to determine the destination register used in the execution.
*		- "Word-Byte control": Signals the process to either address the whole word of the operands, or just the Low Byte
*		- "Instruction Category signature": Bit that allows the decoding circuitry to
*			distinguish between instruction types that share the same instruction category.
*		- "Intruction code": Bit(s) that allows the decode/execution circuitry
*			to distinguish between instructions within the same instrucition type
*		- Instruction category: The two MSBi's of the instruction opcode.
*			Instructions within the same category don't necessarely share anything in common,
*			it's just a way to efficiently decode the instruction opcode.
*/
typedef struct single_register_instruction {
	unsigned short dst_reg : 3;
	unsigned short unused_bits_0 : 3;		// Bit(s) that aren't used to describe any relevant information in the instruction opcode
	unsigned short W_B_ctrl : 1;
	unsigned short unused_bits_1 : 1;		// Bit(s) that aren't used to describe any relevant information in the instruction opcode
	unsigned short inst_type_signature : 1;
	unsigned short inst_code : 3;
	unsigned short unused_bits : 2;			// Bit(s) that aren't used to describe any relevant information in the instruction opcode
	unsigned short inst_category : 2;
} single_register_instruction;


/* Single Register intruction:
*		- "Destination Register": 3 bits used to determine the destination register used in the execution.
*		- "Value": Unique to register initialization. It's a byte value that will written to the destination, 
*			in the manner described by what the instruction code represents
*		- "Intruction code": Bit(s) that allows the decode/execution circuitry
*			to distinguish between instructions within the same instrucition type
*		- Instruction category: The two MSBi's of the instruction opcode.
*			Instructions within the same category don't necessarely share anything in common,
*			it's just a way to efficiently decode the instruction opcode.
*/
typedef struct register_initialization_instruction {
	unsigned short dst_reg : 3;
	unsigned short value : 8;
	unsigned short inst_code : 3;
	unsigned short inst_category : 2;
} register_initialization_instruction;



char MOVL(char dst_reg, unsigned short value);
char MOVLZ(char dst_reg, unsigned short value);
char MOVH(char dst_reg, unsigned short value);

char SRA(char word_byte_control, char dst_reg);
char RRC(char word_byte_control, char dst_reg);
char SWPB(char word_byte_control, char dst_reg);
char SXT(char word_byte_control, char dst_reg);

#endif // !SINGLE_REGISTER_MANIPULATION_AND_INITIALIZATION_INSTRUCTIONS_H
