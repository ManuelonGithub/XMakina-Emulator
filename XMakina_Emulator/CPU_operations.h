/*
* X-Makina Emulator Project - CPU_operations.h
* Debugger header contains all the function prototypes and definitions used during the XMakine machine emulation
*
* Programmer: Manuel Burnay
*
*
* Date created: 30/06/2018
*/

#include "XMakina_Emulator_entities.h"

#pragma once
#ifndef CPU_OPERATIONS_H
#define CPU_OPERATIONS_H

#define INST_CATEGORY(INST) ((INST & 0xC000) >> 14)

enum PROCESS_FLAGS { PROCESS_FAILURE, PROCESS_SUCCESS};
enum BUS_CONTROLS {BYTE = 0, WORD = 1, READ = 0, WRITE = 1};
enum INST_CATEGORY {BRANCHING, ALU, MEM_ACCESS_AND_REG_INIT, MEM_ACCESS_REL};
enum INST_TYPES { INVALID_INST = -1, SUBROUTINE_BRANCH, CONDITIONAL_BRANCH, TWO_OPERAND, SINGLE_REGISTER, DIRECT_MEMORY_ACCESS, REGISTER_INITIALIZATION, RELATIVE_MEMORY_ACCESS};

extern union XMakina_memory memory;
extern union XMakina_register_file register_file;
extern struct XMakina_system_registers system_registers;

struct branching_category {
	unsigned short operand : 10;
	unsigned short ext_info : 3;
	unsigned short is_not_BL : 1;
	unsigned short category : 2;
};

struct ALU_category {
	unsigned short operands : 8;
	unsigned short is_single_reg : 1;
	unsigned short inst_code : 5;
	unsigned short category : 2;
};

struct mem_access_reg_init_category {
	unsigned short operands : 11;
	unsigned short inst_code : 3;
	unsigned short category : 2;
};

struct subroutine_branch_instruction {
	unsigned short offset : 13;
	unsigned short signature : 1;
	unsigned short category : 2;
};

struct conditional_branch_instruction {
	unsigned short offset : 10;
	unsigned short type : 3;
	unsigned short signature : 1;
	unsigned short category : 2;
};

struct two_operand_instruction {
	unsigned short destination_reg : 3;
	unsigned short source : 3;
	unsigned short word_byte_control : 1;
	unsigned short reg_const_control : 1;
	unsigned short signature : 1;
	unsigned short type : 4;
	unsigned short non_descriptive : 1;
	unsigned short category : 2;
};

struct single_register_instruction {
	unsigned short destination_reg : 3;
	unsigned short unused_bits_0 : 3;
	unsigned short word_byte_control : 1;
	unsigned short unused_bits_1 : 1;
	unsigned short signature : 1;
	unsigned short type : 3;
	unsigned short non_descriptive : 2;
	unsigned short category : 2;
};

struct direct_memory_access_instruction {
	unsigned short destination_reg : 3;
	unsigned short source : 3;
	unsigned short word_byte_control : 1;
	unsigned short unused_bits : 1;
	unsigned short increment_flag : 1;
	unsigned short decrement_flag : 1;
	unsigned short pre_pos_addressing_control : 1;
	unsigned short type : 3;
	unsigned short category : 2;
};

struct register_initialization_instruction {
	unsigned short destination_reg : 3;
	unsigned short init_value : 8;
	unsigned short type : 3;
	unsigned short category : 2;
};

struct relative_memory_access_instruction {
	unsigned short destination_reg : 3;
	unsigned short source : 3;
	unsigned short word_byte_control : 1;
	unsigned short offset : 6;
	unsigned short type : 1;
	unsigned short category : 2;
};

union XMakina_instruction_set {
	unsigned short instruction;
	struct branching_category br_cat;
	struct ALU_category ALU_cat;
	struct mem_access_reg_init_category MA_RI_cat;
};

char fetch();
char decode();
void execute();
void device_management();
void debugger_tiggers();

void bus(unsigned short MAR, unsigned short * MBR, char byte_word_control, char read_write_control);
void device_memory_access(unsigned short MAR, unsigned short * MBR, char byte_word_control, char read_write_control);

void interrupt_return_management();

#endif // !CPU_OPERATIONS_H

