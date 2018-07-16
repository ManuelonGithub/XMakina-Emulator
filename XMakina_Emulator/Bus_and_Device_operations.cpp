
#include "stdafx.h"
#include "Bus_Devices_Interrupt_operations.h"

FILE * device_file = 0;
Emulated_device device[DEVICE_NUMBER_SUPPORTED];
input_device_data_queue input_dev_queue;

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
	emulation.run_clk += MEMORY_ACCESS_CLK_INC;
}

void device_memory_access(unsigned short MAR, unsigned short * MBR, char word_byte_control, char read_write_control)
{
}

void device_init()
{
	char filename[100];
	int i, IO, proc_time;

	printf("file name: ");
	scanf_s("%s", &filename, 100);

	if (fopen_s(&device_file, filename, "r") != 0) {
		printf("Error opening Device file. \n\n");
		return;
	} 

	for (i = 0; i < DEVICE_NUMBER_SUPPORTED; i++) {
		device[i].dev_port = &memory.dev_port[i];

		fscanf_s(device_file, "%d %d", &IO, &proc_time);

		device[i].dev_port->I_O = IO;
		device[i].proc_time = proc_time;
	}

	input_dev_queue.data_avail = 0;
}

void device_management()
{
	if (device_file != 0) {
		if (input_dev_queue.data_avail == 1) {
			if (input_dev_queue.clk <= emulation.sys_clk) {
				device[input_dev_queue.dev].dev_port->data = input_dev_queue.data;
				input_dev_queue.data_avail = 0;
			}
		}
		else {
			fscanf_s(device_file, "%d %d %c", &input_dev_queue.clk, &input_dev_queue.dev, &input_dev_queue.data);
			input_dev_queue.data_avail = 1;
		}

		printf("device queue:\n clk for event: %d\n dev: %d \n data: %c \n", input_dev_queue.clk, input_dev_queue.dev, input_dev_queue.data);
	}
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