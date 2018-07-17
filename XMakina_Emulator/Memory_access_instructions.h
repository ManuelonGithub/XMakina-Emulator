/*
 * X-Makina Emulator Project - Memory_access_instructions.h
 * Contains all the function prototypes and definitions used when the CPU cycle is executing a Memory access instruction
 *
 * In order to keep the files at a minimum, while keeping everything organized,
 * Both direct and relative memory addressing are covered in this file.
 *
 * Programmer: Manuel Burnay
 *
 * Rev 1.0: Instructions work as intended and have been properly documented.
 *
 * Date created: 10/07/2018
 */

#include "XMakina_Emulator_entities.h"

#pragma once
#ifndef MEMORY_ACCESS_INSTRUCTIONS_H
#define MEMORY_ACCESS_INSTRUCTIONS_H

enum DIRECT_MEMORY_ACCESS_ACTION { NO_ACTION, POST_INCREMEMT, POST_DECREMENT, PRE_INCREMENT = 5, PRE_DECREMENT };

extern XMakina_memory memory;
extern XMakina_register_file reg_file;
extern System_registers sys_reg;

/* Direct memory access intruction:
 *		- "Destination Register": 3 bits used to determine the destination register used in the execution.
 *		- "source": 3 bits used to determine the source register used in the execution.
 *		- "Word-Byte control": Signals the process to either address the whole word of the operands, or just the Low Byte
 *		- "Action": 3 bits used to determine how the instruction will modify the contents of either source (LD) or destination (ST) register
 *		- "Intruction code": Bit(s) that allows the decode/execution circuitry 
 *			to distinguish between instructions within the same instrucition type
 *		- Instruction category: The two MSBi's of the instruction opcode. 
 *			Instructions within the same category don't necessarely share anything in common,
 *			it's just a way to efficiently decode the instruction opcode.
 */
struct direct_memory_access_instruction {
	unsigned short dst_reg : 3;
	unsigned short source : 3;
	unsigned short W_B_ctrl : 1;
	unsigned short unused_bits : 1;
	unsigned short action : 3;
	unsigned short inst_code : 3;
	unsigned short inst_category : 2;
};

/* Relative memory access intruction:
*		- "Destination Register": 3 bits used to determine the destination register used in the execution.
*		- "source": 3 bits used to determine the source register used in the execution.
*		- "Word-Byte control": Signals the process to either address the whole word of the operands, or just the Low Byte
*		- "Offset": 6 bits that contain the offset added to either the contents of either source (LD) or destination (ST) register
*		- "Intruction code": Bit(s) that allows the decode/execution circuitry
*			to distinguish between instructions within the same instrucition type
*		- Instruction category: The two MSBi's of the instruction opcode.
*			Instructions within the same category don't necessarely share anything in common,
*			it's just a way to efficiently decode the instruction opcode.
*/
struct relative_memory_access_instruction {
	unsigned short dst_reg : 3;
	unsigned short source : 3;
	unsigned short W_B_ctrl : 1;
	signed short offset : 6;
	unsigned short inst_code : 1;
	unsigned short inst_category : 2;
};

char LD(char action, char word_byte_control, char src_reg, char dst_reg);
char ST(char action, char word_byte_control, char src_reg, char dst_reg);

char LDR(signed short offset, char word_byte_control, char src_reg, char dst_reg);
char STR(signed short offset, char word_byte_control, char src_reg, char dst_reg);

#endif // !MEMORY_ACCESS_INSTRUCTIONS_H
