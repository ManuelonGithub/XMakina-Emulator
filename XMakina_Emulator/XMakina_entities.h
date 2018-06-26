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
#ifndef XMAKINA_ENTITIES_H
#define XMAKINA_ENTITIES

#define MEM_SIZE_BYTES (1 << 16)
#define MEM_SIZE_WORDS (1 << 15)
#define LAST_WORD (MEM_SIZE_WORDS - 1)
#define LAST_BYTE (MEM_SIZE_BYTES - 1)

#define XMAKINA_CPU_REG_COUNT 8

union XMakina_memory {
	unsigned char byte[MEM_SIZE_BYTES];
	unsigned short word[MEM_SIZE_WORDS];
};

struct XMakina_register_identifiers {
	unsigned short int R0, R1, R2, R3, LR, SP, PSW, PC;
};

union XMakina_register_file {
	struct XMakina_register_identifiers ID;
	unsigned short int reg[XMAKINA_CPU_REG_COUNT];
};
#endif // !XMAKINA_ENTITIES_H
