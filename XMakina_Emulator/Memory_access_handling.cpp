/*
* X-Makina Emulator Project - Memory_access_handling.cpp
* contains all the functions needed for the emulated machine to access its memory.
*
* Programmer: Manuel Burnay
*
* Rev 1.0: Contents work as intended and have been properly documented.
*
* Date created: 29/07/2018
*/

#include "stdafx.h"
#include "Memory_access_handling.h"

cache_line memory_cache[CACHE_SIZE];
mem_cache_options cache_options;

void (*memory_cache_access[]) (char, char) { direct_mapping, associative_mapping, hybrid_mapping };
void (*replacement_policy[]) (cache_line*, char, char) { write_through_policy, write_back_policy };

/* Memory Bus function:
* It handles the communication between the XMakina machine and primary memory.
* The machine cannot communicate/address its memory without it.
* The function handles read/write, byte/word controls,
* and like in XMakina, only interacts with MAR and MBR.
*
* NOTE:
* WORD_ADDR_CONV, DEVICE_MEMORY_SPACE and MEMORY_ACCESS_CLK_INC are defined in XMakina_Emulator_entities.h
*/
void memory_bus(char word_byte_control, char read_write_control)
{
	emulation.current_cycle_status = NORMAL_MEM_ACCESS;		// Initialize the emulation flag as to keep the behaviour of the function predictable.

	if (sys_reg.MAR < DEVICE_MEMORY_SPACE) {
		emulation.current_cycle_status = device_memory_access(word_byte_control, read_write_control);	// Because the user can be attempting reading/writing data to the device memory space
	}																									// without wanting to initiate a device data transfer process, 
																										// So the system must keep track of this in order to keep the code efficient.
	if (emulation.current_cycle_status == NORMAL_MEM_ACCESS) {
		if (read_write_control == READ) {
			sys_reg.MBR = (word_byte_control == WORD) ? memory.word[WORD_ADDR_CONV(sys_reg.MAR)] : memory.byte[sys_reg.MAR];
		}
		else {
			if (word_byte_control == WORD) {
				memory.word[WORD_ADDR_CONV(sys_reg.MAR)] = sys_reg.MBR;
			}
			else {
				memory.byte[sys_reg.MAR] = (unsigned char)sys_reg.MBR;
			}
		}
	}

	emulation.sys_clk += MEMORY_ACCESS_CLK_INC;
	emulation.run_clk += MEMORY_ACCESS_CLK_INC;
}

/* Device memory access:
* Function is called by the bus to handle certain events that occur when the machine addresses
* the memory space allocated to device ports.
*
* NOTE:
* WORD_ADDR_CONV is defined in XMakina_Emulator_entities.h
*/
char device_memory_access(char word_byte_control, char read_write_control)
{
	unsigned char dev_num = WORD_ADDR_CONV(sys_reg.MAR);

	switch (device[dev_num].dev_port->I_O)
	{
	case (INPUT):
		if (read_write_control == READ) {
			sys_reg.MBR = (word_byte_control == WORD) ? memory.word[WORD_ADDR_CONV(sys_reg.MAR)] : memory.byte[sys_reg.MAR];
			device[dev_num].dev_port->DBA = DISABLED;
			return DEVICE_MEM_ACCESS;
		}
		else {
			return NORMAL_MEM_ACCESS;	// In order to satisfy the instruction of the user, 
		}								// even if it was determined that it an invalid attempt of starting a device data transfer process,
		break;							// The system will still follow through with the memory access instruction.

	case (OUTPUT):
		if (read_write_control == WRITE) {
			if (device[dev_num].dev_port->DBA = DISABLED) {
				device[dev_num].dev_port->OV = ENABLED;
			}

			if (word_byte_control == WORD) {
				memory.word[WORD_ADDR_CONV(sys_reg.MAR)] = sys_reg.MBR;
			}
			else {
				memory.byte[sys_reg.MAR] = (unsigned char)sys_reg.MBR;
			}

			device[dev_num].dev_port->DBA = DISABLED;
			device[dev_num].time_left = device[dev_num].proc_time;
			return DEVICE_MEM_ACCESS;
		}
		else {
			return NORMAL_MEM_ACCESS;
		}
		break;

	default:
		return NORMAL_MEM_ACCESS;
		break;
	}
}

/* Cache initialization function: 
 * Function sets the memory covered by the cache memory to 0,
 * so the cache implementation can confidently make the assumption
 * that the contents inside each cache line are 0 in the very beginning of a program.
 */
void initialize_cache()
{
	memset(memory_cache, 0, sizeof(memory_cache));
}

/* New bus function:
 * This new bus function has the cache implementation inside it,
 * and can swap between cache being present in the machine or not.
 * It also makes sure that any access to device memory is done through the memory bus
 */
void bus(char word_byte_control, char read_write_control)
{
	if (cache_options.op_control == DISABLED || sys_reg.MAR < DEVICE_MEMORY_SPACE) {
		memory_bus(word_byte_control, read_write_control);
	}
	else {
		(*memory_cache_access[cache_options.mem_org]) (word_byte_control, read_write_control);
	}
}

/* Direct mapping function:
 * Function is called by the bus if the user has set it as the run's selected cache organization.
 * It implements cache direct mapping organization in this emulation.
 * It determines which cache line will be addressed, and if it was a Miss or a Hit.
 * This information is then passed to the selected cache replacement policy function.
 * It also increments the emulation clock and the comparison clock accordingly.
 */
void direct_mapping(char word_byte_control, char read_write_control)
{
	unsigned short cache_index = WORD_ADDR_CONV(sys_reg.MAR) % CACHE_SIZE;	// The addressed cache line is determined by the LSBis of the word address needed to cover the whole memory cache. for cache size of 32, 5 bits are needed.

	char access_status;	// This holds the internal code used by the policy implementations to determine if it is a Miss/Hit (bit 1) and Read/Write (bit 0)

	access_status = read_write_control;
	access_status |= ((memory_cache[cache_index].address != WORD_ADDR_CONV(sys_reg.MAR)) << 1);

	(*replacement_policy[cache_options.policy]) (&memory_cache[cache_index], word_byte_control, access_status);

	emulation.sys_clk += CACHE_MEM_ACCESS_CLK_INC;
	emulation.run_clk += CACHE_MEM_ACCESS_CLK_INC;
	cache_options.comparison_clk -= CACHE_MEM_ACCESS_CLK_INC;
}

/* Associative mapping function:
 * Function is called by the bus if the user has set it as the run's selected cache organization.
 * It implements cache direct associative organization in this emulation.
 * It determines which cache line will be addressed, and if it was a Miss or a Hit.
 * This information is then passed to the selected cache replacement policy function.
 * It also increments the emulation clock and the comparison clock accordingly.
 */
void associative_mapping(char word_byte_control, char read_write_control)
{
	int i, cache_index;
	char LRU_cache_value = MAX_LRU_VALUE;	// In Associative mapping, the highest LRU value is 31.
	char access_status;
	
	access_status = MISS;	// Is initialized as a miss so it isn't addressed every loop iteration that matches the miss condition.

	for (i = 0; i < CACHE_SIZE; i++) {
		if (memory_cache[i].address == WORD_ADDR_CONV(sys_reg.MAR)) {	// Hit condition
			cache_index = i;
			access_status = HIT;
			break;
		}
		else if (memory_cache[i].LRU < LRU_cache_value) {	// Miss condition. Will remember the first instance of LRU = 0
			cache_index = i;
			LRU_cache_value = memory_cache[i].LRU;
		}
	}

	access_status |= read_write_control;

	(*replacement_policy[cache_options.policy]) (&memory_cache[cache_index], word_byte_control, access_status);

	for (i = 0; i < CACHE_SIZE; i++) {								// this loop updates the LRU values of each cache line, if needed.
		if (memory_cache[i].LRU > memory_cache[cache_index].LRU) {	// Will only updates cache lines with a higher LRU value than the accessed cache line.
			memory_cache[i].LRU--;
		}
	}

	memory_cache[cache_index].LRU = MAX_LRU_VALUE;	// The accessed cache line is updated with the highest LRU value.

	emulation.sys_clk += CACHE_MEM_ACCESS_CLK_INC;
	emulation.run_clk += CACHE_MEM_ACCESS_CLK_INC;
	cache_options.comparison_clk -= CACHE_MEM_ACCESS_CLK_INC;
}

/* Hybrid mapping function:
 * Function is called by the bus if the user has set it as the run's selected cache organization.
 * It implements cache hybrid associative organization in this emulation.
 * It determines which cache line will be addressed, and if it was a Miss or a Hit.
 * This information is then passed to the selected cache replacement policy function.
 * It also increments the emulation clock and the comparison clock accordingly.
 */
void hybrid_mapping(char word_byte_control, char read_write_control)
{
	int i, cache_index, page, starting_addr;
	char LRU_cache_value = MAX_HYBRID_LRU_VALUE;	// In hybrid, the highest LRU value is 3.
	char access_status;

	page = ((WORD_ADDR_CONV(sys_reg.MAR)) & HYBRID_PAGE_MASK) >> 2;	// First the cache page is determined. Bits 2-4 are retreived from the word address being accessed, and shifted to the right twice so it only occupies 3 bits.
	starting_addr = (page * HYBRID_LINES_IN_PAGE);					// The first cache line of the determined page can be found by multiplying the page value by 4.
	access_status = MISS;											// Like in associative, status is initialized as a Miss to avoid multiple addressing inside the for loop

	for (i = starting_addr; i <= (starting_addr + HYBRID_LINES_IN_PAGE); i++) {	// Searches the 4 cache lines inside the page
		if (memory_cache[i].address == WORD_ADDR_CONV(sys_reg.MAR)) {	// Hit condition
			cache_index = i;
			access_status = HIT;
			break;
		}
		else if (memory_cache[i].LRU < LRU_cache_value) {	// Miss condition. Will remember the first instance of LRU = 0.
			cache_index = i;
			LRU_cache_value = memory_cache[i].LRU;
		}
	}

	access_status |= read_write_control;

	(*replacement_policy[cache_options.policy]) (&memory_cache[cache_index], word_byte_control, access_status);

	for (i = starting_addr; i <= (starting_addr + HYBRID_LINES_IN_PAGE); i++) {	// this loop updates the LRU values of each cache line, if needed.
		if (memory_cache[i].LRU > memory_cache[cache_index].LRU) {				// Will only updates cache lines with a higher LRU value than the accessed cache line.
			memory_cache[i].LRU--;
		}
	}

	memory_cache[cache_index].LRU = MAX_HYBRID_LRU_VALUE;

	emulation.sys_clk += CACHE_MEM_ACCESS_CLK_INC;
	emulation.run_clk += CACHE_MEM_ACCESS_CLK_INC;
	cache_options.comparison_clk -= CACHE_MEM_ACCESS_CLK_INC;
}

/* Write-through replacement policy function:
 * Implementation how the cache line is accessed when write-through policy is selected.
 * Because policies also modify how reads function, policy functions also cover that access status.
 * In write through, cache will always be consistent with primary memory, so every write is passed to primary memory.
 * This is the slowest implementation of cache. Only when there is a Read Hit will it save time, 
 * any other possible takes longer.
 */
void write_through_policy(cache_line * cache_line, char word_byte_control, char access_status)
{
	switch (access_status)
	{
	case MISS_WRITE:
		memory_bus(word_byte_control, WRITE);	// By writing to primary memory first, the need to memorize the previous value of MBR won't be needed.
		memory_bus(WORD, READ);
		cache_line->address = WORD_ADDR_CONV(sys_reg.MAR);
		cache_line->contents.word = sys_reg.MBR;

		cache_options.comparison_clk -= MEMORY_ACCESS_CLK_INC;	// With no cache, memory would only be access once here, so the comparison clk is adjusted accordingly.
		break;

	case HIT_WRITE:
		if (word_byte_control == WORD) {
			cache_line->contents.word = sys_reg.MBR;
		}
		else {
			cache_line->contents.byte[(sys_reg.MAR % 2)] = (unsigned char)sys_reg.MBR;
		}
		memory_bus(word_byte_control, WRITE);
		break;

	case MISS_READ:
		memory_bus(WORD, READ);
		cache_line->address = WORD_ADDR_CONV(sys_reg.MAR);
		cache_line->contents.word = sys_reg.MBR;

		cache_options.comparison_clk -= MEMORY_ACCESS_CLK_INC;	// Because the Miss Read cascades down to a Hit Read, the comparison clk needs to be adjusted so it remains accurate, despite the changes made to it in the Hit Read case.

	case HIT_READ:
		sys_reg.MBR = (word_byte_control == WORD) ? cache_line->contents.word : cache_line->contents.byte[(sys_reg.MAR % 2)];

		cache_options.comparison_clk += MEMORY_ACCESS_CLK_INC;	// No cache would've accessed the bus here, so comparison clk is adjusted accordingly.
		break;

	default:
		break;
	}
}

/* Write-back replacement policy function:
* Implementation how the cache line is accessed when write-back policy is selected.
* Because policies also modify how reads function, policy functions also cover that access status.
* In write backh, any cache writes are stored solely on cache, 
* and will only be written back to primary memory once that line needs to be re-mapped.
* This is the fastest implementation of cache. If the dirty bit isn't set, 
* there are 3 possibilities (Miss & Write & Word; a Hit, regardless of any other factor) where this cache assess takes less time than a no-cache implementation.
*/
void write_back_policy(cache_line * cache_line, char word_byte_control, char access_status)
{
	unsigned short mem_location_accessed = sys_reg.MAR, overwriting_data = sys_reg.MBR;	// Because the memory bus implementation doesn't take MAR and MBR as arguments, 
																						// but rather accesses the global variables in the function, the current states of MAR and MBR are memorized.
	switch (access_status)
	{
	case MISS_WRITE:
		if (cache_line->dirty_bit == 1) {						// Servicing the dirty bit if the cache line has it set.
			sys_reg.MAR = BYTE_ADDR_CONV(cache_line->address);
			sys_reg.MBR = cache_line->contents.word;
			memory_bus(WRITE, WORD);
			sys_reg.MAR = mem_location_accessed;
			sys_reg.MBR = overwriting_data;
																	// No need to clear the diry bit, since this case cascades down to Hit Write, where the dirty bit gets set.
			cache_options.comparison_clk -= MEMORY_ACCESS_CLK_INC;	// No cache implementation doesn't need to service a dirty bit, so comparison clk is adjusted accordingly.
		}

		if (word_byte_control == BYTE) {	// If a Miss->Write->Byte addressing occurs, memory needs to accessed so the cache line contains the byte of the word that isn't overwritten.
			memory_bus(READ, WORD);
			cache_line->contents.word = sys_reg.MBR;
			sys_reg.MBR = overwriting_data;

			cache_options.comparison_clk -= MEMORY_ACCESS_CLK_INC;	// because Miss write cascades down to Hit Write case, the comparison clk needs to be adjusted here to counter the changes made to it in the Hit Write case. 
		}

		cache_line->address = WORD_ADDR_CONV(sys_reg.MAR);

	case HIT_WRITE:
		if (word_byte_control == WORD) {
			cache_line->contents.word = sys_reg.MBR;
		}
		else {
			cache_line->contents.byte[(sys_reg.MAR % 2)] = (unsigned char)sys_reg.MBR;
		}
		cache_line->dirty_bit = 1;	// The cache data is no longer consistent with primary memory, so the dirty bit is set.

		cache_options.comparison_clk += MEMORY_ACCESS_CLK_INC;	// No cache would've needed to access the bus here, so the comparison clk is adjusted.
		break;

	case MISS_READ:
		if (cache_line->dirty_bit == 1) {						// Servicing the dirty bit if the cache line has it set.
			sys_reg.MAR = BYTE_ADDR_CONV(cache_line->address);
			sys_reg.MBR = cache_line->contents.word;
			memory_bus(WRITE, WORD);
			sys_reg.MAR = mem_location_accessed;
			cache_line->dirty_bit = 0;

			cache_options.comparison_clk -= MEMORY_ACCESS_CLK_INC;	// No cache implementation doesn't need to service a dirty bit, so comparison clk is adjusted accordingly.
		}

		memory_bus(READ, WORD);
		cache_line->address = WORD_ADDR_CONV(sys_reg.MAR);
		cache_line->contents.word = sys_reg.MBR;

		cache_options.comparison_clk -= MEMORY_ACCESS_CLK_INC;	// because Miss Read cascades down to Hit Read case, the comparison clk needs to be adjusted here to counter the changes made to it in the Hit Read case. 

	case HIT_READ:
		sys_reg.MBR = (word_byte_control == WORD) ? cache_line->contents.word : cache_line->contents.byte[(sys_reg.MAR % 2)];

		cache_options.comparison_clk += MEMORY_ACCESS_CLK_INC;	// No cache would've needed to access the bus here, so the comparison clk is adjusted.
		break;

	default:
		break;
	}
}