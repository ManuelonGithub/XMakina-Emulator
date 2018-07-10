
#include "XMakina_Emulator_entities.h"

#pragma once
#ifndef BUS_DEVICE_INTERRUPTS_OPERATIONS_H
#define BUS_DEVICE_INTERRUPTS_OPERATIONS_H

enum BUS_CONTROLS { WORD = 0, BYTE = 1, READ = 0, WRITE = 1 };

extern unsigned int System_clk;

extern union XMakina_memory memory;
extern unsigned short MAR;
extern unsigned short MBR;
extern unsigned short REG_CON_table[REG_OR_CON][XMAKINA_CPU_REG_COUNT];

void device_management();
void bus(unsigned short MAR, unsigned short * MBR, char word_byte_control, char read_write_control);
void device_memory_access(unsigned short MAR, unsigned short * MBR, char word_byte_control, char read_write_control);
void interrupt_return_process();


#endif // !BUS_AND_DEVICE_OPERATIONS_H
