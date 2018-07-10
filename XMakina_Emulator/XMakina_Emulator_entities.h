/*
* X-Makina Emulator Project - XMakina_entities.h
* XMakina entities header file contains the definitions of the several components of an XMakina Machine
*
* Programmer: Manuel Burnay
*
*
* Date created: 26/06/2018
*/

#pragma once
#ifndef XMAKINA_EMULATOR_ENTITIES_H
#define XMAKINA_EMULATOR_ENTITIES_H

#define HIGH_BYTE_SHIFT(x) (x*(1 << 8))
#define LOW_BYTE_MASK 0x00FF
#define HIGH_BYTE_MASK 0xFF00
#define LOW_BYTE_OVERWRITE(dst, value) ((dst & HIGH_BYTE_MASK) | (value & LOW_BYTE_MASK))
#define HIGH_BYTE_OVERWRITE(dst, value) ((dst & LOW_BYTE_MASK) | (value & HIGH_BYTE_MASK))
#define BIT_CHANGE(src, bit, value) (src ^= (-value ^ src) & (1 << bit))
#define BIT_CHECK(src, bit) ((src >> bit) & 1)

#define MAX_PROG_NAME_SIZE 30

#define MEM_SIZE_BYTES (1 << 16)
#define MEM_SIZE_WORDS (1 << 15)
#define LAST_WORD (MEM_SIZE_WORDS - 1)
#define LAST_BYTE (MEM_SIZE_BYTES - 1)
#define WORD_ADDR_CONV(byte_addr) (byte_addr >> 1)
#define BYTE_ADDR_CONV(word_addr) (word_addr << 1)
#define XMAKINA_CPU_REG_COUNT 8
#define DEVICE_MEMORY_SPACE 16

#define REG_OR_CON 2
#define REG 0
#define CON 1
#define LR (REG_CON_table[REG][4])
#define SP (REG_CON_table[REG][5])
#define PSW (REG_CON_table[REG][6])
#define PC (REG_CON_table[REG][7])

#define PSW_PRIORITY ((PSW & 0x00E0) >> 5)
#define PSW_V ((PSW & 0x0010) >> 4)
#define PSW_SLP ((PSW & 0x0008) >> 3)
#define PSW_N ((PSW & 0x0004) >> 2)
#define PSW_Z ((PSW & 0x0002) >> 1)
#define PSW_C (PSW & 0x0001)

#define IX (instruction.opcode)

#define PC_BYTE_STEP 1
#define PC_WORD_STEP (PC_BYTE_STEP*2)
#define PC_NO_INC 0
#define MEMORY_ACCESS_CLK_INC 3
#define NORMAL_OP_CLK_INC 1

enum PROCESS_FLAGS { PROCESS_FAILURE, PROCESS_SUCCESS };
enum BIT_MANIP { CLEAR, SET };

union XMakina_memory {
	unsigned char byte[MEM_SIZE_BYTES];
	unsigned short word[MEM_SIZE_WORDS];
};

struct two_operand_instruction {
	unsigned short destination_reg : 3;
	unsigned short source : 3;
	unsigned short word_byte_control : 1;
	unsigned short reg_const_control : 1;
	unsigned short inst_type_signature : 1;
	unsigned short inst_code : 4;
	unsigned short unused_bits : 1;
	unsigned short inst_category : 2;
};

unsigned short REG_CON_table[REG_OR_CON][XMAKINA_CPU_REG_COUNT] =
{
	{ 0, 0, 0, 0, 0, 0, 0, 0 },		// Registers 
	{ 0, 1, 2, 4, 8, 32, 48, -1 }	// Constant  
};

//struct PSW_word {
//	unsigned short C : 1;
//	unsigned short Z : 1;
//	unsigned short SLP : 1;
//	unsigned short V : 1;
//	unsigned short PRIORITY : 3;
//	unsigned short unused_bits : 8;
//};

//struct XMakina_register_identifiers {
//	unsigned short R0, R1, R2, R3, LR, SP, PSW, PC;
//};

//union XMakina_register_file {
//	struct XMakina_register_identifiers ID;
//	unsigned short reg[XMAKINA_CPU_REG_COUNT];
//};

#endif // !XMAKINA_EMULATOR_ENTITIES_H
