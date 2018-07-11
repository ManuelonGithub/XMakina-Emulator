
#include "stdafx.h"
#include "Bus_Devices_Interrupt_operations.h"

void bus(unsigned short MAR, unsigned short * MBR, char word_byte_control, char read_write_control)
{
	if (MAR < DEVICE_MEMORY_SPACE) {
		device_memory_access(MAR, MBR, word_byte_control, read_write_control);
	}
	else {
		if (read_write_control == READ) {
			*MBR = (word_byte_control == WORD) ? memory.word[WORD_ADDR_CONV(MAR)] : memory.byte[MAR];
		}
		else {
			if (word_byte_control == WORD) {
				memory.word[WORD_ADDR_CONV(MAR)] = *MBR;
			}
			else {
				memory.byte[MAR] = (unsigned char)* MBR;
			}
		}
	}

	emulation.sys_clk += MEMORY_ACCESS_CLK_INC;
}

void device_memory_access(unsigned short MAR, unsigned short * MBR, char word_byte_control, char read_write_control)
{
}

void device_management()
{
}

void interrupt_return_process()
{
}

//void bus(char word_byte_control, char read_write_control)
//{
//	if (sys_reg.MAR < DEVICE_MEMORY_SPACE) {
//		device_memory_access(word_byte_control, read_write_control);
//	}
//	else {
//		if (read_write_control == READ) {
//			sys_reg.MBR = (word_byte_control == WORD) ? memory.word[WORD_ADDR_CONV(sys_reg.MAR)] : memory.byte[sys_reg.MAR];
//		}
//		else {
//			if (word_byte_control == WORD) {
//				memory.word[WORD_ADDR_CONV(sys_reg.MAR)] = sys_reg.MBR;
//			}
//			else {
//				memory.byte[sys_reg.MAR] = (unsigned char)sys_reg.MBR;
//			}
//		}
//	}
//
//	emulation.sys_clk += MEMORY_ACCESS_CLK_INC;
//}
//
//void device_memory_access(char word_byte_control, char read_write_control)
//{
//}