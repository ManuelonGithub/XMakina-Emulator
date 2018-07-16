/*
* X-Makina Emulator Project - Bus_Devices_Interrupt_operations.cpp
* Contains all the functions related to the bus, devices, and interrupt handling.
*
* Programmer: Manuel Burnay
*
* Date created: 10/07/2018
*/

#include "stdafx.h"
#include "Bus_Devices_Interrupt_operations.h"

FILE * device_input_file = 0;		// It is initialized to 0 so the emulation can avoid attempting to read a file if it hasn't been opened.
FILE * device_output_file = 0;
Emulated_device device[DEVICE_NUMBER_SUPPORTED];
input_device_data_queue input_dev_queue;
char interrupt_queue[DEVICE_NUMBER_SUPPORTED];


/* Bus function:
 * It handles the communication between the XMakina machine and its memory. 
 * The machine cannot communicate/address its memory without it.
 * The function handles read/write, byte/word controls,
 * and like in XMakina, only interacts with MAR and MBR.
 */
void bus(char word_byte_control, char read_write_control)
{
	emulation.current_cycle_status = NORMAL_MEM_ACCESS;

	if (sys_reg.MAR < DEVICE_MEMORY_SPACE) {
		emulation.current_cycle_status = device_memory_access(word_byte_control, read_write_control);
	}

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
 */
char device_memory_access(char word_byte_control, char read_write_control)
{
	unsigned char dev_num = WORD_ADDR_CONV(sys_reg.MAR);

	if (sys_reg.MAR % 2 == 1) {
		switch (device[dev_num].dev_port->I_O)
		{
		case (INPUT):
			if (read_write_control == READ) {
				device[dev_num].dev_port->DBA = DISABLED;
				sys_reg.MBR = (word_byte_control == WORD) ? memory.word[WORD_ADDR_CONV(sys_reg.MAR)] : memory.byte[sys_reg.MAR];
				return DEVICE_MEM_ACCESS;
			}
			else {
				return NORMAL_MEM_ACCESS;
			}
			break;

		case (OUTPUT):
			if (read_write_control == WRITE) {
				if (device[dev_num].dev_port->DBA = DISABLED) {
					device[dev_num].dev_port->OV = ENABLED;
				}

				device[dev_num].dev_port->DBA = DISABLED;
				memory.byte[sys_reg.MAR] = (unsigned char)sys_reg.MBR;
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
	else {
		return NORMAL_MEM_ACCESS;
	}
}

/* Device initialization function:
 * Function that handles initialization of the devices and device ports.
 * Also handles opening the device files (input and output).
 */
void device_init()
{
	char filename[100];
	char out_filename[107] = "OUTPUT_";

	int i, IO, proc_time;

	printf("file name: ");
	scanf_s("%s", &filename, 100);

	if (fopen_s(&device_input_file, filename, "r") != 0) {
		printf("Error opening Device file. \n\n");
		return;
	} 

	strcat_s(out_filename, sizeof(out_filename), filename);

	if (fopen_s(&device_output_file, out_filename, "w") != 0) {
		printf("There was an error when attempting to create the device output file. \n\n");
		fclose(device_input_file);
		return;
	}

	for (i = 0; i < DEVICE_NUMBER_SUPPORTED; i++) {
		interrupt_queue[i] = INT_OFF;

		device[i].dev_port = &memory.dev_port[i];
		device[i].dev_port->control = 0;
		device[i].int_vector.word[0] = &memory.word[DEV_INT_VECT_PSW_WORD(i)];
		device[i].int_vector.word[1] = &memory.word[DEV_INT_VECT_PC_WORD(i)];

		fscanf_s(device_input_file, "%d %d", &IO, &proc_time);

		device[i].dev_port->I_O = IO;
		device[i].proc_time = proc_time;
	}

	for (i = 0; i < DEVICE_NUMBER_SUPPORTED; i++) {
		printf("Dev%d: I/O = %d\n", i, device[i].dev_port->I_O);
	}
	input_dev_queue.data_avail = 0;
}

/* Device management function:
 * Function is called at the end of every CPU cycle.
 * It handles data communication between XMakina and devices,
 * by reading the input device file, and managing the data insertion from the input devies,
 * to the appropriate memory location at the appropriate time,
 * or managing the data processing times of the output devices. <--- TODO
 */
void device_management()
{
	int i;

	if (device_input_file != 0) {
		if (input_dev_queue.data_avail == 1) {
			if (input_dev_queue.clk <= emulation.sys_clk) {
				input_device_data_process(input_dev_queue.dev, input_dev_queue.data);
				input_dev_queue.data_avail = 0;
			}
		}
		else {
			fscanf_s(device_input_file, "%d %d %c", &input_dev_queue.clk, &input_dev_queue.dev, &input_dev_queue.data);
			input_dev_queue.data_avail = 1;
		}

		printf("device queue:\n clk for event: %d\n dev: %d \n data: %c \n", input_dev_queue.clk, input_dev_queue.dev, input_dev_queue.data);

		for (i = 0; i < DEVICE_NUMBER_SUPPORTED; i++) {
			if (device[i].dev_port->I_O == OUTPUT && device[i].time_left > 0) {
				output_device_data_process(i);
			}
		}
	}
}


void input_device_data_process(unsigned char dev_num, unsigned char data)
{
	if (device[dev_num].dev_port->I_O != INPUT) {
		printf("There is a problem with the device file. It is attempting to use an output device as an input device (device %d).\n\n", dev_num);
		return;
	}

	if (device[dev_num].dev_port->DBA == DISABLED) {
		device[dev_num].dev_port->DBA = ENABLED;
	}
	else {
		device[dev_num].dev_port->OV = ENABLED;
	}

	device[dev_num].dev_port->data = data;

	if (device[dev_num].dev_port->IE = ENABLED) {
		interrupt_handling_process(dev_num);
	}
}


void output_device_data_process(unsigned char dev_num)
{
	device[dev_num].time_left -= emulation.run_clk;

	if (device[dev_num].time_left <= 0) {
		fprintf(device_output_file, "Device %d has recieved the char %c on clock %d\n", dev_num, device[dev_num].dev_port->data, emulation.sys_clk);
		device[dev_num].dev_port->DBA = ENABLED;
	}
}

void interrupt_handling_process(unsigned char dev_num)
{
	if (device[dev_num].int_vector.INT_PSW->PRIORITY <= reg_file.PSW.PRIORITY) {
		interrupt_queue[dev_num] = device[dev_num].int_vector.INT_PSW->PRIORITY;
	}
	else {
		sys_reg.temp_reg.word = reg_file.PSW.PRIORITY;

		push(&reg_file.PC.word);
		push(&reg_file.PSW.word);
		push(&reg_file.LR.word);

		reg_file.PSW.word = device[dev_num].int_vector.INT_PSW->word;
		reg_file.PSW.SLP = DISABLED;
		reg_file.PSW.prev_PRIORITY = sys_reg.temp_reg.word;

		reg_file.PC.word = device[dev_num].int_vector.INT_PC->word;
		reg_file.LR.word = LAST_BYTE;
	}
}

void interrupt_return_process()
{
	int i;
	unsigned char dev_num = LOW_BYTE_MASK;	// Distinct initialization so the process can easily determine if variable hasn't been modifed.

	for (i = 0; i < DEVICE_NUMBER_SUPPORTED; i++) {
		if (interrupt_queue[i] > reg_file.PSW.prev_PRIORITY) {
			dev_num = i;
		}
	}

	if (dev_num != LOW_BYTE_MASK) {
		interrupt_handling_process(dev_num);
	}
	else {
		pull(&reg_file.LR.word);
		pull(&reg_file.PSW.word);
		reg_file.PSW.SLP = DISABLED;
		pull(&reg_file.PC.word);
	}
}

void push(unsigned short * reg)
{
	reg_file.SP.word -= WORD_STEP;
	sys_reg.MAR = reg_file.SP.word;
	sys_reg.MBR = *reg;
	bus(WORD, WRITE);
}

void pull(unsigned short * reg)
{
	sys_reg.MAR = reg_file.SP.word;
	reg_file.SP.word += WORD_STEP;
	bus(WORD, READ);
	*reg = sys_reg.MBR;
}

