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


union XMakina_memory {
	unsigned char byte[MEM_SIZE_BYTES];
	unsigned short word[MEM_SIZE_WORDS];
};

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

struct direct_memory_access_instruction {
	unsigned short destination_reg : 3;
	unsigned short source : 3;
	unsigned short word_byte_control : 1;
	unsigned short unused_bits : 1;
	unsigned short action : 3;
	unsigned short inst_code : 3;
	unsigned short inst_category : 2;
};

struct register_initialization_instruction {
	unsigned short destination_reg : 3;
	unsigned short value : 8;
	unsigned short inst_code : 3;
	unsigned short inst_category : 2;
};

struct relative_memory_access_instruction {
	unsigned short destination_reg : 3;
	unsigned short source : 3;
	unsigned short word_byte_control : 1;
	unsigned short offset : 6;
	unsigned short inst_code : 1;
	unsigned short inst_category : 2;
};

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

//struct XMakina_register_identifiers {
//	unsigned short R0, R1, R2, R3, LR, SP, PSW, PC;
//};

//union XMakina_register_file {
//	struct XMakina_register_identifiers ID;
//	unsigned short reg[XMAKINA_CPU_REG_COUNT];
//};

#endif // !XMAKINA_EMULATOR_ENTITIES_H
