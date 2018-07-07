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

#define MEM_SIZE_BYTES (1 << 16)
#define MEM_SIZE_WORDS (1 << 15)
#define LAST_WORD (MEM_SIZE_WORDS - 1)
#define LAST_BYTE (MEM_SIZE_BYTES - 1)
#define MAX_PROG_NAME_SIZE 20
#define XMAKINA_CPU_REG_COUNT 8
#define DEVICE_MEMORY_SPACE 16

union XMakina_memory {
	unsigned char byte[MEM_SIZE_BYTES];
	unsigned short word[MEM_SIZE_WORDS];
};

struct XMakina_register_identifiers {
	unsigned short R0, R1, R2, R3, LR, SP, PSW, PC;
};

struct XMakina_system_registers {
	unsigned short MAR, MBR, IX;
};

union XMakina_register_file {
	struct XMakina_register_identifiers ID;
	unsigned short reg[XMAKINA_CPU_REG_COUNT];
};
#endif // !XMAKINA_EMULATOR_ENTITIES_H
