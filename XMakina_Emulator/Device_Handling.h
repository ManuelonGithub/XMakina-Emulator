/*
 * X-Makina Emulator Project - Device_Handling.h
 * Contains all the function prototypes and definitions related to the devices and interrupt handling.
 *
 * Programmer: Manuel Burnay
 *
 * Rev 1.0: Instructions work as intended and have been properly documented.
 *
 * Date created: 10/07/2018
 */

#include "XMakina_Emulator_entities.h"

#pragma once
#ifndef DEVICE_HANGLING_H
#define DEVICE_HANGLING_H

#define DEVICE_MEMORY_SPACE 16
#define DEV_INT_VECTOR_MEMORY_SPACE 64
#define DEV_INT_VECTOR_MEM_WORD_SPACE 32
#define INT_OFF -1

#define DEV_INT_VECT_PSW_WORD(dev_num) (LAST_WORD - (DEV_INT_VECTOR_MEM_WORD_SPACE - 1) + (dev_num * WORD_STEP))
#define DEV_INT_VECT_PC_WORD(dev_num) (LAST_WORD - (DEV_INT_VECTOR_MEM_WORD_SPACE - 1) + (dev_num * WORD_STEP) + 1)

extern XMakina_memory memory;
extern System_registers sys_reg;
extern Emulation_properties emulation;
extern XMakina_register_file reg_file;
extern Emulated_device device[DEVICE_NUMBER_SUPPORTED];

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

void device_init();
void close_device_files();
void device_management();
void input_device_data_process(unsigned char dev_num, unsigned char data);
void output_device_data_process(unsigned char dev_num);

void interrupt_handling_process(unsigned char dev_num);
void interrupt_return_process();

void push(unsigned short * reg);
void pull(unsigned short * reg);

#endif // !DEVICE_HANGLING_H