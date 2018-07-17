/*
 * X-Makina Emulator Project - Bus_Devices_Interrupt_operations.h
 * Contains all the function prototypes and definitions related to the bus, devices, and interrupt handling.
 *
 * Programmer: Manuel Burnay
 *
 * Rev 1.0: Instructions work as intended and have been properly documented.
 *
 * Date created: 10/07/2018
 */

#include "XMakina_Emulator_entities.h"

#pragma once
#ifndef BUS_DEVICE_INTERRUPTS_OPERATIONS_H
#define BUS_DEVICE_INTERRUPTS_OPERATIONS_H

#define DEVICE_MEMORY_SPACE 16
#define DEV_INT_VECTOR_MEMORY_SPACE 64
#define DEV_INT_VECTOR_MEM_WORD_SPACE 32
#define INT_OFF -1

#define DEV_INT_VECT_PSW_WORD(dev_num) (LAST_WORD - (DEV_INT_VECTOR_MEM_WORD_SPACE - 1) + (dev_num * WORD_STEP))
#define DEV_INT_VECT_PC_WORD(dev_num) (LAST_WORD - (DEV_INT_VECTOR_MEM_WORD_SPACE - 1) + (dev_num * WORD_STEP) + 1)

enum DEVICE_TYPES { OUTPUT = 0, INPUT, DISABLED = 0, ENABLED };
enum BUS_CONTROLS { READ = 0, WRITE = 1, DEVICE_MEM_ACCESS = 1, NORMAL_MEM_ACCESS = 0 };

extern XMakina_memory memory;
extern System_registers sys_reg;
extern Emulation_properties emulation;
extern XMakina_register_file reg_file;

/* Device data queue:
 * This structure is used to efficiently manage when to read the input device file,
 * after the device initialization has been made.
 */
typedef struct input_device_data_queue {
	unsigned char data_avail;
	unsigned int clk;
	unsigned char dev;
	unsigned char data;
} input_device_data_queue;

typedef struct device_interrupt_vector {
	union {
		unsigned short * word[2];
		struct {
			PSW_reg_format * INT_PSW;
			register_format * INT_PC;
		};
	};
} device_interrupt_vector;

/* Emulated device structure:
 * Using this structure for emualted devices allows for the system to efficiently
 * keep track and address all the properties of a device.
 * The interrupt vector structure allows the access to the device's interrupt vector memory space,
 * and the device port pointer allows the access to the device's port in the XMakina memory
 */
typedef struct Emulated_device {
	device_interrupt_vector int_vector;
	Device_port * dev_port;
	int proc_time;
	int time_left;
} Emulated_device;

void device_init();
void device_management();
void input_device_data_process(unsigned char dev_num, unsigned char data);
void output_device_data_process(unsigned char dev_num);

void bus(char word_byte_control, char read_write_control);
char device_memory_access(char word_byte_control, char read_write_control);

void interrupt_handling_process(unsigned char dev_num);
void interrupt_return_process();

void push(unsigned short * reg);
void pull(unsigned short * reg);

#endif // !BUS_AND_DEVICE_OPERATIONS_H