/*
* X-Makina Emulator Project - Memory_access_handling.h
* Contains all the function prototypes and definitions needed for the emulated machine to access its memory.
*
* Programmer: Manuel Burnay
*
* Rev 1.0: Contents work as intended and have been properly documented.
*
* Date created: 29/07/2018
*/

#include "XMakina_Emulator_entities.h"

#pragma once
#ifndef MEMORY_ACCESS_HANDLING_H
#define MEMORY_ACCESS_HANDLING_H

#define DEVICE_MEMORY_SPACE 16

#define CACHE_SIZE 32
#define CACHE_MEM_ACCESS_CLK_INC 1
#define MIN_LRU_VALUE 0
#define MAX_LRU_VALUE 0x1F
#define HYBRID_PAGE_MASK 0x1c
#define HYBRID_LINES_IN_PAGE 4
#define MAX_HYBRID_LRU_VALUE 3

enum CACHE_CONFIGS { DIRECT, ASSOCIATIVE, HYBRID, WRITE_THROUGH = 0, WRITE_BACK };
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

/* Cache line struct:
 * Implementation of an Xmakina cache line.
 * 40 Bits wide: 
 * 16 for the memory word address the line is mapped to (will be 0 is it is not mapped) - address.
 * 16 for the word data stored in the cache line. Data can be split between a hig byte and a low byte - contents.
 *  8 for the control status of the cache line. This byte contains the dirty bit and the LRU of the cache line.
 */
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

/* Cache options structure:
 * Structure that constains the user configuration for the memory cache implementation.
 * op_control -> Enables cache access if set.
 * mem_org -> Constains the value related to the selected cache organization, so the function pointer in the bus function works properly.
 * policy -> Constains the value related to the selected cache replacement policy, so the function pointer in the cache organization function works properly.
 * comparison clock -> Clock value that is used to compute the clock count of a program if cache wasn't implemented.
 */
typedef struct mem_cache_options {
	unsigned char op_control;
	unsigned char mem_org;
	unsigned char policy;
	int comparison_clk;
} mem_cache_options;

void memory_bus(char word_byte_control, char read_write_control);
char device_memory_access(char word_byte_control, char read_write_control);

void bus(char word_byte_control, char read_write_control);

void initialize_cache();

void direct_mapping(char word_byte_control, char read_write_control);
void associative_mapping(char word_byte_control, char read_write_control);
void hybrid_mapping(char word_byte_control, char read_write_control);

void write_through_policy(cache_line * cache_line, char word_byte_control, char access_status);
void write_back_policy(cache_line * cache_line, char word_byte_control, char access_status);

#endif // !MEMORY_CACHE_H
