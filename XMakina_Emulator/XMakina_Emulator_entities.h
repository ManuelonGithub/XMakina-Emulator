/*
 * X-Makina Emulator Project - XMakina_emulator_entities.h
 * XMakina entities header file contains the definitions of the several components of an XMakina Machine
 *
 * Programmer: Manuel Burnay
 *
 * Rev 1.0: Instructions work as intended and have been properly documented.
 *
 * Date created: 26/06/2018
 */

#pragma once
#ifndef XMAKINA_EMULATOR_ENTITIES_H
#define XMAKINA_EMULATOR_ENTITIES_H

#define HIGH_BYTE_SHIFT(x) (x*(1 << 8))	// returns the value of x shfited by 8 bits
#define LOW_BYTE_MASK 0x00FF
#define HIGH_BYTE_MASK 0xFF00

/* Macro changes the bit specified by the the 'bit' position of the 'src' by the the binary number specified by the 'value'
 * -> src[bit] = value.
 * General macro used to set/clear any bit of the source value
 */
#define BIT_CHANGE(src, bit, value) (src ^= (-value ^ src) & (1 << bit))

 /* Macro returns the binary value of the specific 'bit' in the 'src'
 * -> returns src[bit]
 * General macro used to retreive the value of any bit in the source value
 */
#define BIT_CHECK(src, bit) ((src >> bit) & 1)
#define WORD_MSBi 15
#define BYTE_MSBi 7
#define LSBi 0

#define MAX_PROG_NAME_SIZE 30
#define XMAKINA_CPU_REG_COUNT 8
#define DEVICE_NUMBER_SUPPORTED 8
#define MEM_SIZE_BYTES (1 << 16)
#define MEM_SIZE_WORDS (1 << 15)

#define LAST_WORD (MEM_SIZE_WORDS - 1)
#define LAST_BYTE (MEM_SIZE_BYTES - 1)
#define WORD_ADDR_CONV(byte_addr) (byte_addr >> 1)
#define BYTE_ADDR_CONV(word_addr) (word_addr << 1)

#define TRUE 1
#define FALSE 0

#define BYTE_STEP 1
#define WORD_STEP (BYTE_STEP*2)
#define NO_INC 0
#define MEMORY_ACCESS_CLK_INC 3
#define NORMAL_OP_CLK_INC 1

enum OPERATION_SIZE { WORD, BYTE };
enum PROCESS_FLAGS { PROCESS_FAILURE, PROCESS_SUCCESS };
enum BIT_MANIP { CLEAR, SET };

void update_PSW(unsigned short src, unsigned short dst, unsigned short res, unsigned char W_B_ctrl);	// The function is here due to multiple functions requiring it.

/* Device port structure:
 * speficies the bit structure of the memory word dedicated to a device port
 */
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

typedef union XMakina_memory {
	unsigned char byte[MEM_SIZE_BYTES];
	unsigned short word[MEM_SIZE_WORDS];
	Device_port dev_port[DEVICE_NUMBER_SUPPORTED];
} XMakina_memory;

/* Program Status Word register format structure:
 * speficies the bit structure of PSW.
 */
typedef union PSW_reg_format {
	unsigned short word;
	struct {
		unsigned short C : 1;
		unsigned short Z : 1;
		unsigned short N : 1;
		unsigned short SLP : 1;
		unsigned short V : 1;
		unsigned short PRIORITY : 3;
		unsigned short res : 5;
		unsigned short prev_PRIORITY : 3;
	};
} PSW_reg_format;

/* Register Format structure:
 * speficies the structure of a general register.
 */
typedef struct BCD_Overlay {
	unsigned short nib0 : 4;
	unsigned short nib1 : 4;
	unsigned short nib2 : 4;
	unsigned short nib3 : 4;
} BCD_Overlay;

typedef union register_format {
	unsigned short word;
	struct {
		unsigned char LSB;
		unsigned char MSB;
	};
	BCD_Overlay BCD;
} register_format;

/* XMakina Register File union:
 * Allows the emulation to easily pair an array of registers,
 * with the specific register it represents.
 * This allows for the emulation code to look very clean when addressing
 * a spefic register, while at the same time not having to come up with
 * complex solutions to figure out the register position in the register file.
 */
typedef union XMakina_register_file {
	struct {
		register_format R0, R1, R2, R3, LR, SP;
		PSW_reg_format PSW;
		register_format PC;
	};
	register_format REG [XMAKINA_CPU_REG_COUNT];
} XMakina_register_file;

/* Instruction Register format structure:
 * speficies the bit structure of IX.
 * This bit format allows for the decoding and execution process
 * to be free of heavy bit masking and bitwise operations.
 */
typedef union IX_bit_format {
	unsigned short word;
	struct {
		unsigned short undescriptive_0 : 8;
		unsigned short ALU_cat_sig_bit : 1;
		unsigned short undescriptive_1 : 2;

		/* These bits contains both the instruction code of a Memory address/Reg. init. instruction
		 * and the bit signature for the branching instructions, thus the name multi-purpose.
		 * thus the name multi-purpose.
		 */
		unsigned short multi_purporse : 3;
		unsigned short category : 2;
	} bits;
} IX_bit_format;

/* System register structure:
 * Contains all the registers unaddressable/hidden to the user.
 * They are used for communication between external entities such as memory,
 * or used in the CPU cycle to temporarely contain values to be decoded/used in operations.
 */
typedef struct System_registers {
	unsigned short MAR;
	unsigned short MBR;
	IX_bit_format IX;
	register_format temp_reg_A;
	register_format temp_reg_B;
} System_registers;

/* Emulation properties structure:
 * Contains properties related to the emulation run,
 * such as the system/run clocks, the current program name, the system status,
 * which is altered by the debugger to indicate that the user wishes to exit the emulation.
 * Current cycle status/instruction type is used by the CPU cycle to:
 * 1) either indicate that invalid has been attempted to be executed,
 * 2) or transfer the decoded instruction type from the decoding process to the execution process.
 * Lastly it also contains the flag that is set when ctrl C signal is detected.
 */
typedef struct Emulation_properties {
	unsigned int sys_clk;
	unsigned int run_clk;
	char program_name[MAX_PROG_NAME_SIZE];
	char system_status;
	union {
		char current_cycle_status;
		char current_cycle_inst_type;
	};
	unsigned char ctrl_C_detected;
} Emulation_properties;

//typedef struct device_interrupt_vector {
//	union {
//		unsigned short * word[2];
//		struct {
//			PSW_reg_format * INT_PSW;
//			register_format * INT_PC;
//		};
//	};
//} device_interrupt_vector;
//
//typedef struct Emulated_device {
//	device_interrupt_vector int_vector;
//	Device_port * dev_port;
//	int proc_time;
//	int time_left;
//} Emulated_device;

#endif // !XMAKINA_EMULATOR_ENTITIES_H
