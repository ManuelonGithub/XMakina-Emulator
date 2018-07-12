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
#define LSBi(value) (value & 0x1)
#define WORD_MSBi(word) (word & 0x8000)
#define BYTE_MSBi(byte) (byte & 0x80)

#define MAX_PROG_NAME_SIZE 30

#define MEM_SIZE_BYTES (1 << 16)
#define MEM_SIZE_WORDS (1 << 15)
#define LAST_WORD (MEM_SIZE_WORDS - 1)
#define LAST_BYTE (MEM_SIZE_BYTES - 1)
#define WORD_ADDR_CONV(byte_addr) (byte_addr >> 1)
#define BYTE_ADDR_CONV(word_addr) (word_addr << 1)
#define XMAKINA_CPU_REG_COUNT 8
#define DEVICE_NUMBER_SUPPORTED 8

#define PC_BYTE_STEP 1
#define PC_WORD_STEP (PC_BYTE_STEP*2)
#define PC_NO_INC 0
#define MEMORY_ACCESS_CLK_INC 3
#define NORMAL_OP_CLK_INC 1

enum OPERATION_SIZE { WORD, BYTE };
enum PROCESS_FLAGS { PROCESS_FAILURE, PROCESS_SUCCESS };
enum BIT_MANIP { CLEAR, SET };

typedef struct Device_port {
	union {
		unsigned char control;
		struct {
			unsigned char IE : 1;
			unsigned char I_O : 1;
			unsigned char DBA : 1;
			unsigned char OV : 1;
			unsigned char RES : 4;
		};
	};
	unsigned char data;
} Device_port;

typedef struct Device {
	Device_port * dev_port;
	int proc_time;
	int time_left;
} Device;

typedef union XMakina_memory {
	unsigned char byte[MEM_SIZE_BYTES];
	unsigned short word[MEM_SIZE_WORDS];
	Device_port dev_port[DEVICE_NUMBER_SUPPORTED];
} XMakina_memory;

typedef union PSW_reg_format {
	unsigned short word;
	struct {
		unsigned short C : 1;
		unsigned short Z : 1;
		unsigned short N : 1;
		unsigned short SLP : 1;
		unsigned short V : 1;
		unsigned short PRIORITY : 3;
		unsigned short unused_bits : 8;
	};
} PSW_reg_format;

typedef union register_format {
	unsigned short word;
	struct {
		unsigned char LSB;
		unsigned char MSB;
	};
} reg;

typedef union XMakina_register_file {
	struct {
		register_format R0, R1, R2, R3, LR, SP;
		PSW_reg_format PSW;
		register_format PC;
	};
	register_format REG [XMAKINA_CPU_REG_COUNT];
} XMakina_register_file;

typedef union IX_bit_format {
	unsigned short word;
	struct {
		unsigned short undescriptive_0 : 8;
		unsigned short ALU_cat_sig_bit : 1;
		unsigned short undescriptive_1 : 2;
		unsigned short multi_purporse : 3;
		unsigned short category : 2;
	} bits;
} IX_bit_format;

typedef struct System_registers {
	unsigned short MAR;
	unsigned short MBR;
	IX_bit_format IX;
} System_registers;

typedef struct Emulation_properties {
	unsigned int sys_clk;
	char program_name[MAX_PROG_NAME_SIZE];
	char system_status = '\0';
	union {
		char current_cycle_status;
		char current_cycle_inst_type;
	};
} Emulation_properties;

#endif // !XMAKINA_EMULATOR_ENTITIES_H
