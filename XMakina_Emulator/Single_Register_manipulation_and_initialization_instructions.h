
#include "XMakina_Emulator_entities.h"

#pragma once

#ifndef SINGLE_REGISTER_MANIPULATION_AND_INITIALIZATION_INSTRUCTIONS_H
#define SINGLE_REGISTER_MANIPULATION_AND_INITIALIZATION_INSTRUCTIONS_H

#define SINGLE_RIGHT_SHIFT(value) (value >> 1)

extern union XMakina_memory memory;
extern unsigned short MAR;
extern unsigned short MBR;
extern unsigned short REG_CON_table[REG_OR_CON][XMAKINA_CPU_REG_COUNT];

struct single_register_instruction {
	unsigned short destination_reg : 3;
	unsigned short unused_bits_0 : 3;
	unsigned short word_byte_control : 1;
	unsigned short unused_bits_1 : 1;
	unsigned short inst_type_signature : 1;
	unsigned short inst_code : 3;
	unsigned short unused_bits : 2;
	unsigned short inst_category : 2;
};

struct register_initialization_instruction {
	unsigned short destination_reg : 3;
	unsigned short value : 8;
	unsigned short inst_code : 3;
	unsigned short inst_category : 2;
};



char MOVL(char dst_reg, unsigned short value);
char MOVLZ(char dst_reg, unsigned short value);
char MOVH(char dst_reg, unsigned short value);

char SRA(char word_byte_control, char dst_reg);
char RRC(char word_byte_control, char dst_reg);
char SWPB(char word_byte_control, char dst_reg);
char SXT(char word_byte_control, char dst_reg);

#endif // !SINGLE_REGISTER_MANIPULATION_AND_INITIALIZATION_INSTRUCTIONS_H
