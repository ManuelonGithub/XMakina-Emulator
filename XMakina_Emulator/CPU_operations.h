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

#define BYTE_STEP 1
#define WORD_STEP (BYTE_STEP*2)
#define NO_INC 0

#define INST_CATEGORY(INST) ((INST & 0xC000) >> 14)

#define BRANCH_INST_TYPE_SIGNATURE(INST) ((INST & 0x2000) >> 13)
#define BRANCHING_INST_CODE(INST) ((INST & 0x1C00) >> 10)
#define BRANCHING_OFFSET_WORD(OFFSET) ((OFFSET << 1))
#define ALU_INST_TYPE_SIGNATURE(INST) ((INST & 0x100) >> 8)
#define ALU_INST_CODE(INST) ((INST & 0x1E00) >> 0)
#define MEM_ACCESS_AND_REG_INIT_INST_TYPE(INST) ((INST & 0x3800) >> 11)
#define REG_INIT_INST_CODE_START 2
#define RELATIVE_MEMORY_ACCESS_INST_TYPE(INST) ((INST & 0x2000) >> 13)

enum PROCESS_FLAGS { PROCESS_FAILURE, PROCESS_SUCCESS};
enum BUS_CONTROLS { WORD = 0, BYTE = 1, READ = 0, WRITE = 1};
enum INST_CATEGORY {BRANCHING, ALU, MEM_ACCESS_AND_REG_INIT, MEM_ACCESS_REL};
enum INST_TYPES { INVALID_INST = -1, BRANCH_WITH_LINK_INST, CONDITIONAL_BRANCH_INST, TWO_OPERAND_INST, 
					SINGLE_REGISTER_INST, DIRECT_MEMORY_ACCESS_INST, REGISTER_INITIALIZATION_INST, RELATIVE_MEMORY_ACCESS_INST};
enum DIRECT_MEMORY_ACCESS_ACTION { NO_ACTION, POST_INCREMEMT, POST_DECREMENT, PRE_INCREMENT = 5, PRE_DECREMENT};

extern union XMakina_memory memory;
extern unsigned short MAR;
extern unsigned short MBR;
extern union XMakina_instruction_set instruction;
extern unsigned short REG_CON_table[REG_OR_CON][XMAKINA_CPU_REG_COUNT];

//struct branching_category {
//	unsigned short operand : 10;
//	unsigned short ext_info : 3;
//	unsigned short is_not_BL : 1;
//	unsigned short inst_category : 2;
//};

//struct ALU_category {
//	unsigned short operands : 8;
//	unsigned short is_single_reg : 1;
//	unsigned short inst_code : 5;
//	unsigned short inst_category : 2;
//};

//struct mem_access_reg_init_category {
//	unsigned short operands : 11;
//	unsigned short inst_code : 3;
//	unsigned short inst_category : 2;
//};

char fetch();
char decode();
char execute(char instruction_type);
void device_management();
void debugger_tiggers();

void bus(unsigned short MAR, unsigned short * MBR, char byte_word_control, char read_write_control);
void device_memory_access(unsigned short MAR, unsigned short * MBR, char byte_word_control, char read_write_control);

void interrupt_return_process();

char BL(signed short label);

char BEQ_BZ(signed short label);
char BNE_BNZ(signed short label);
char BC_BHS(signed short label);
char BNC_BLO(signed short label);
char BN(signed short label);
char BGE(signed short label);
char BLT(signed short label);
char BAL(signed short label);

char MOVL(unsigned short dst_reg, unsigned short value);
char MOVLZ(unsigned short dst_reg, unsigned short value);
char MOVH(unsigned short dst_reg, unsigned short value);

char LD(unsigned short action, unsigned short word_byte_control, unsigned short src_reg, unsigned short dst_reg);
char ST(unsigned short action, unsigned short word_byte_control, unsigned short src_reg, unsigned short dst_reg);


#endif // !CPU_OPERATIONS_H

