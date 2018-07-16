
#include "XMakina_Emulator_entities.h"

#pragma once
#ifndef BUS_DEVICE_INTERRUPTS_OPERATIONS_H
#define BUS_DEVICE_INTERRUPTS_OPERATIONS_H

#define DEVICE_MEMORY_SPACE 16

enum BUS_CONTROLS { READ = 0, WRITE = 1 };

extern XMakina_memory memory;
extern System_registers sys_reg;
extern Emulation_properties emulation;

struct input_device_data_queue {
	unsigned char data_avail;
	unsigned int clk;
	unsigned char dev;
	unsigned char data;
};

void device_init();
void device_management();
void bus(unsigned short MAR, unsigned short * MBR, char word_byte_control, char read_write_control);
void device_memory_access(unsigned short MAR, unsigned short * MBR, char word_byte_control, char read_write_control);
void interrupt_return_process();


#endif // !BUS_AND_DEVICE_OPERATIONS_H