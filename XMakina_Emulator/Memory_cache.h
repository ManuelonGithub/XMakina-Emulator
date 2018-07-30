


#include "XMakina_Emulator_entities.h"

#pragma once
#ifndef MEMORY_CACHE_H
#define MEMORY_CACHE_H

#define CACHE_SIZE 32
#define CACHE_MEM_ACCESS_CLK_INC 1

enum ACCESS_STATUSES {HIT_READ, HIT_WRITE, MISS_READ, MISS_WRITE};

extern System_registers sys_reg;
extern Emulation_properties emulation;

union content {
	unsigned short word;
	unsigned char byte[2];
};

typedef struct cache_line {
	unsigned short address;
	union content contents;
	union {
		unsigned char control;
		struct {
			unsigned char dirty_bit : 1;
			unsigned char unused_bits : 2;
			unsigned char LRU : 5;
		};
	};
} cache_line;

typedef struct mem_cache_options {
	unsigned char op_control;
	unsigned char mem_organization;
	unsigned char replacement_policy;
} mem_cache_options;

void direct_mapping(char word_byte_control, char read_write_control);
void write_through_policy(cache_line * cache_line, char word_byte_control, char read_write_control);
void write_back_policy(cache_line * cache_line, char word_byte_control, char read_write_control);

#endif // !MEMORY_CACHE_H
