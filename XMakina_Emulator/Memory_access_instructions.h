
#include "XMakina_Emulator_entities.h"

#pragma once
#ifndef MEMORY_ACCESS_INSTRUCTIONS_H
#define MEMORY_ACCESS_INSTRUCTIONS_H

enum DIRECT_MEMORY_ACCESS_ACTION { NO_ACTION, POST_INCREMEMT, POST_DECREMENT, PRE_INCREMENT = 5, PRE_DECREMENT };

extern union XMakina_memory memory;
extern unsigned short MAR;
extern unsigned short MBR;
extern union XMakina_instruction_set instruction;
extern unsigned short REG_CON_table[REG_OR_CON][XMAKINA_CPU_REG_COUNT];

struct direct_memory_access_instruction {
	unsigned short destination_reg : 3;
	unsigned short source : 3;
	unsigned short word_byte_control : 1;
	unsigned short unused_bits : 1;
	unsigned short action : 3;
	unsigned short inst_code : 3;
	unsigned short inst_category : 2;
};

struct relative_memory_access_instruction {
	unsigned short destination_reg : 3;
	unsigned short source : 3;
	unsigned short word_byte_control : 1;
	signed short offset : 6;
	unsigned short inst_code : 1;
	unsigned short inst_category : 2;
};

char LD(char action, char word_byte_control, char src_reg, char dst_reg);
char ST(char action, char word_byte_control, char src_reg, char dst_reg);

char LDR(signed short offset, char word_byte_control, char src_reg, char dst_reg);
char STR(signed short offset, char word_byte_control, char src_reg, char dst_reg);

#endif // !MEMORY_ACCESS_INSTRUCTIONS_H
