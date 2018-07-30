/*
* X-Makina Emulator Project - Memory_cache.cpp
* Contains the Main body and the ctrl C signal catcher.
*
* Programmer: Manuel Burnay
*
* Rev 1.0: Instructions work as intended and have been properly documented.
*
* Date created: 29/07/2018
*/

#include "stdafx.h"
#include "Memory_cache.h"
#include "Bus_Devices_Interrupt_operations.h"

cache_line memory_cache[CACHE_SIZE];
mem_cache_options cache_options;

void(*replacement_policy[]) (cache_line*, char, char) { write_through_policy, write_back_policy };

void initialize_cache()
{
	memset(memory_cache, 0, sizeof(memory_cache));
}

void direct_mapping(char word_byte_control, char read_write_control)
{
	unsigned short cache_index = sys_reg.MAR % 5;
	
	(*replacement_policy[cache_options.replacement_policy]) (&memory_cache[cache_index], word_byte_control, read_write_control);

	emulation.sys_clk += CACHE_MEM_ACCESS_CLK_INC;
	emulation.run_clk += CACHE_MEM_ACCESS_CLK_INC;
}

void associative_mapping(char word_byte_control, char read_write_control)
{
	int i, cache_index, LRU_cache_value = MAX_LRU_VALUE;

	for (i = 0; i < CACHE_SIZE; i++) {
		if (memory_cache[i].address == sys_reg.MAR) {
			cache_index = i;
			break;
		}
		else if (memory_cache[i].LRU < LRU_cache_value) {
			cache_index = i;
			LRU_cache_value = memory_cache[i].LRU;
		}
	}

	(*replacement_policy[cache_options.replacement_policy]) (&memory_cache[cache_index], word_byte_control, read_write_control);

	if (memory_cache[cache_index].LRU != MIN_LRU_VALUE) {
		for (i = 0; i < CACHE_SIZE; i++) {
			if (memory_cache[i].LRU > memory_cache[cache_index].LRU) {
				memory_cache[i].LRU--;
			}
		}
	}

	memory_cache[cache_index].LRU = MAX_LRU_VALUE;

	emulation.sys_clk += CACHE_MEM_ACCESS_CLK_INC;
	emulation.run_clk += CACHE_MEM_ACCESS_CLK_INC;
}

//void direct_mapping_write_back(char word_byte_control, char read_write_control)
//{
//	unsigned short mem_location_accessed = sys_reg.MAR, overwriting_data = sys_reg.MBR;
//	unsigned short cache_index = sys_reg.MAR % 5;
//	unsigned char access_status = read_write_control;
//
//	access_status |= ((memory_cache[cache_index].address != sys_reg.MAR) << 1);
//
//	switch (access_status)
//	{
//	case MISS_WRITE:
//		if (memory_cache[cache_index].dirty_bit == 1) {
//			sys_reg.MAR = memory_cache[cache_index].address;
//			sys_reg.MBR = memory_cache[cache_index].contents.word;
//			bus(WRITE, WORD);
//			sys_reg.MAR = mem_location_accessed;
//		}
//
//		if (word_byte_control == BYTE) {
//			bus(READ, WORD);
//			memory_cache[cache_index].contents.word = sys_reg.MBR;
//		}
//
//		memory_cache[cache_index].address = sys_reg.MAR;
//		sys_reg.MBR = overwriting_data;
//
//	case HIT_WRITE:
//		if (word_byte_control == WORD) {
//			memory_cache[cache_index].contents.word = sys_reg.MBR;
//		}
//		else {
//			memory_cache[cache_index].contents.byte[(sys_reg.MAR % 2)] = (unsigned char)sys_reg.MBR;
//		}
//		memory_cache[cache_index].dirty_bit = 1;
//		break;
//
//	case MISS_READ:
//		if (memory_cache[cache_index].dirty_bit == 1) {
//			sys_reg.MAR = memory_cache[cache_index].address;
//			sys_reg.MBR = memory_cache[cache_index].contents.word;
//			bus(WRITE, WORD);
//			memory_cache[cache_index].dirty_bit = 0;
//			sys_reg.MAR = mem_location_accessed;
//		}
//
//		bus(READ, WORD);
//		memory_cache[cache_index].address = sys_reg.MAR;
//		memory_cache[cache_index].contents.word = sys_reg.MBR;
//
//	case HIT_READ:
//		sys_reg.MBR = (word_byte_control == WORD) ? memory_cache[cache_index].contents.word : memory_cache[cache_index].contents.byte[(sys_reg.MAR % 2)];
//		break;
//
//	default:
//		break;
//	}
//}

void write_through_policy(cache_line * cache_line, char word_byte_control, char read_write_control)
{
	unsigned char access_status = read_write_control;

	access_status |= ((cache_line->address != sys_reg.MAR && cache_line->address != (sys_reg.MAR-1)) << 1);

	switch (access_status)
	{
	case MISS_WRITE:
		bus(word_byte_control, WRITE);
		bus(WORD, READ);
		cache_line->address = sys_reg.MAR;
		cache_line->contents.word = sys_reg.MBR;
		break;

	case HIT_WRITE:
		if (word_byte_control == WORD) {
			cache_line->contents.word = sys_reg.MBR;
		}
		else {
			cache_line->contents.byte[(sys_reg.MAR % 2)] = (unsigned char)sys_reg.MBR;
			sys_reg.MBR = cache_line->contents.word;
		}
		bus(WORD, WRITE);
		break;

	case MISS_READ:
		bus(WORD, READ);
		cache_line->address = sys_reg.MAR;
		cache_line->contents.word = sys_reg.MBR;

	case HIT_READ:
		sys_reg.MBR = (word_byte_control == WORD) ? cache_line->contents.word : cache_line->contents.byte[(sys_reg.MAR % 2)];
		break;

	default:
		break;
	}
}

void write_back_policy(cache_line * cache_line, char word_byte_control, char read_write_control)
{
	unsigned short mem_location_accessed = sys_reg.MAR, overwriting_data = sys_reg.MBR;
	unsigned char access_status = read_write_control;

	access_status |= ((cache_line->address != sys_reg.MAR) << 1);

	switch (access_status)
	{
	case MISS_WRITE:
		if (cache_line->dirty_bit == 1) {
			sys_reg.MAR = cache_line->address;
			sys_reg.MBR = cache_line->contents.word;
			bus(WRITE, WORD);
			sys_reg.MAR = mem_location_accessed;
		}

		if (word_byte_control == BYTE) {
			bus(READ, WORD);
			cache_line->contents.word = sys_reg.MBR;
		}

		cache_line->address = sys_reg.MAR;
		sys_reg.MBR = overwriting_data;

	case HIT_WRITE:
		if (word_byte_control == WORD) {
			cache_line->contents.word = sys_reg.MBR;
		}
		else {
			cache_line->contents.byte[(sys_reg.MAR % 2)] = (unsigned char)sys_reg.MBR;
		}
		cache_line->dirty_bit = 1;
		break;

	case MISS_READ:
		if (cache_line->dirty_bit == 1) {
			sys_reg.MAR = cache_line->address;
			sys_reg.MBR = cache_line->contents.word;
			bus(WRITE, WORD);
			cache_line->dirty_bit = 0;
			sys_reg.MAR = mem_location_accessed;
		}

		bus(READ, WORD);
		cache_line->address = sys_reg.MAR;
		cache_line->contents.word = sys_reg.MBR;

	case HIT_READ:
		sys_reg.MBR = (word_byte_control == WORD) ? cache_line->contents.word : cache_line->contents.byte[(sys_reg.MAR % 2)];
		break;

	default:
		break;
	}
}