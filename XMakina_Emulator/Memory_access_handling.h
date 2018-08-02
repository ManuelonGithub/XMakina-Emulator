

#include "XMakina_Emulator_entities.h"

#pragma once
#ifndef MEMORY_ACCESS_HANDLING_H
#define MEMORY_ACCESS_HANDLING_H

#define DEVICE_MEMORY_SPACE 16

#define CACHE_SIZE 32
#define CACHE_MEM_ACCESS_CLK_INC 1
#define MIN_LRU_VALUE 0
#define MAX_LRU_VALUE 0x1F
#define MISS_FLAG_MASK 1
#define HIT_FLAG_MASK ~(1<<1)

enum CACHE_CONFIGS { DIRECT_MAPPING, ASSOCIATIVE_MAPPING, HYBRID_MAPPING, WRITE_THROUGH = 0, WRITE_BACK };
enum ACCESS_STATUSES {HIT_READ, HIT_WRITE, MISS_READ, MISS_WRITE, HIT = 0, MISS = 2 };
enum DEVICE_TYPES { OUTPUT = 0, INPUT, DISABLED = 0, ENABLED };
enum BUS_CONTROLS { READ = 0, WRITE = 1, DEVICE_MEM_ACCESS = 1, NORMAL_MEM_ACCESS = 0 };

extern XMakina_memory memory;
extern System_registers sys_reg;
extern Emulation_properties emulation;
extern Emulated_device device[DEVICE_NUMBER_SUPPORTED];

union content {
	unsigned short word;
	unsigned char byte[2];
};

typedef union cache_access_status {
	char status;
	struct {
		char read_write: 1;
		char hit_miss : 1;
		char unused : 6;
	};
} cache_access_status;

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
	unsigned char mem_org;
	unsigned char policy;
} mem_cache_options;

void memory_bus(char word_byte_control, char read_write_control);
char device_memory_access(char word_byte_control, char read_write_control);

void bus(char word_byte_control, char read_write_control);

void initialize_cache();
void direct_mapping(char word_byte_control, char read_write_control);
void associative_mapping(char word_byte_control, char read_write_control);
void write_through_policy(cache_line * cache_line, char word_byte_control, char access_status);
void write_back_policy(cache_line * cache_line, char word_byte_control, char access_status);

#endif // !MEMORY_CACHE_H
