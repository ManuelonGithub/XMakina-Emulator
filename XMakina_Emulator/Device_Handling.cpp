/*
 * X-Makina Emulator Project - Bus_Devices_Interrupt_operations.cpp
 * Contains all the functions related to the bus, devices, and interrupt handling.
 *
 * Programmer: Manuel Burnay
 *
 * Rev 1.0: Instructions work as intended and have been properly documented.
 *
 * Date created: 10/07/2018
 */

#include "stdafx.h"
#include "Device_Handling.h"
#include "Memory_access_handling.h"


FILE * device_input_file = 0;		// It is initialized to 0 so the emulation can avoid attempting to read a file if it hasn't been opened.
FILE * device_output_file = 0;
input_device_data_queue input_dev_queue;
char interrupt_queue[DEVICE_NUMBER_SUPPORTED];
char input_file_EOF = 0;


/* Device initialization function:
 * Function that handles initialization of the devices and device ports.
 * Also handles opening the device files (input and output).
 *
 * NOTE:
 * MEMORY_ACCESS_CLK_INC is defined in XMakina_Emulator_entities.h
 */
void device_init()
{
	close_device_files();

	printf("\n=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~ Device File Loader ~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=\n\n");

	char filename[100];
	char out_filename[107] = "OUTPUT_";

	int dev, i, IO, proc_time;

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

	strcpy_s(emulation.device_file_name, sizeof(emulation.device_file_name), filename);

	for (i = 0; i < DEVICE_NUMBER_SUPPORTED; i++) {
		interrupt_queue[i] = INT_OFF;

		device[i].dev_port = &memory.dev_port[i];
		device[i].int_vector.word[0] = &memory.word[DEV_INT_VECT_PSW_WORD(i)];
		device[i].int_vector.word[1] = &memory.word[DEV_INT_VECT_PC_WORD(i)];

		fscanf_s(device_input_file, "%d %d %d", &dev, &IO, &proc_time);

		device[i].dev_port->I_O = IO;
		device[i].proc_time = proc_time;
	}

	//for (i = 0; i < DEVICE_NUMBER_SUPPORTED; i++) {
	//	printf("Dev%d: I/O = %d, Processing time = %d\n", i, device[i].dev_port->I_O, device[i].proc_time);
	//}
	input_dev_queue.data_avail = 0;
}

/* Close device file function:
 * Securely closes any opened input and output device files.
 * Re-initializes the FILE pointers to 0, 
 * so device related functions aren't executed.
 */
void close_device_files()
{
	if (device_input_file != 0) {
		fclose(device_input_file);
		fclose(device_output_file);
		device_input_file = 0;
		device_output_file = 0;
		memset(emulation.device_file_name, '\0', sizeof(emulation.device_file_name));
	}
}
/* Device management function:
 * Function is called at the end of every CPU cycle.
 * It handles data communication between XMakina and devices,
 * by reading the input device file, and managing the data insertion from the input devies,
 * to the appropriate memory location at the appropriate time,
 * or managing the data processing times of the output devices
 *
 * NOTE:
 * MEMORY_ACCESS_CLK_INC is defined in XMakina_Emulator_entities.h
 */
void device_management()
{
	int i;

	if (device_input_file != 0) {
		while (input_dev_queue.clk <= emulation.sys_clk && input_file_EOF != 1) {
			if (input_dev_queue.data_avail == 1) {
				input_device_data_process(input_dev_queue.dev, input_dev_queue.data);
				input_dev_queue.data_avail = 0;
			}
			else {
				if (fscanf_s(device_input_file, "%d %d %c", &input_dev_queue.clk, &input_dev_queue.dev, &input_dev_queue.data) == EOF) {
					input_file_EOF = 1;
				}
				input_dev_queue.data_avail = 1;
			}
		}
		//if (input_dev_queue.data_avail == 1) {
		//	if (input_dev_queue.clk <= emulation.sys_clk) {
		//		input_device_data_process(input_dev_queue.dev, input_dev_queue.data);
		//		input_dev_queue.data_avail = 0;
		//	}
		//}
		//// The second conditional check allows the system to read the file in the same run that it processed in device data
		//if (input_dev_queue.data_avail == 0) {
		//	if (fscanf_s(device_input_file, "%d %d %c", &input_dev_queue.clk, &input_dev_queue.dev, &input_dev_queue.data) == EOF) {
		//		input_file_EOF = 1;
		//	}
		//	input_dev_queue.data_avail = 1;
		//}

		//printf("device queue:\n clk for event: %d\n dev: %d \n data: %c \n", input_dev_queue.clk, input_dev_queue.dev, input_dev_queue.data);

		for (i = 0; i < DEVICE_NUMBER_SUPPORTED; i++) {
			if (device[i].dev_port->I_O == OUTPUT && device[i].time_left > 0) {
				output_device_data_process(i);
			}
		}
	}
}

/* Input device data process function:
 * Function gets called when the device management function determines 
 * that an input device is sending data to the machine.
 * It goes through the process of how an input device would interact with the machine,
 * properly setting the DBA and OV bits when needed,
 * and tiggering of the interrupt handler when appropriate.
 */
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
		fprintf(device_output_file, "Machine has failed to read the char %c from input device %d. This data has been overrun on clock %d.\n", device[dev_num].dev_port->data, dev_num, emulation.sys_clk);
	}

	device[dev_num].dev_port->data = data;

	if (device[dev_num].dev_port->IE == ENABLED) {
		interrupt_handling_process(dev_num);
	}
}

/* Output device data process function:
 * Function gets called when the device management function determines
 * that an output device is receiving data to the machine.
 * It goes through the process of how an output device would interact with the machine,
 * properly setting the DBA and OV bits when needed and the device processing time,
 * and tiggering of the interrupt handler when appropriate.
 */
void output_device_data_process(unsigned char dev_num)
{
	device[dev_num].time_left -= emulation.run_clk;

	if (device[dev_num].dev_port->OV == ENABLED) {
		fprintf(device_output_file, "Output device %d has detected a data overrun on clock %d\n", dev_num, emulation.sys_clk);
		device[dev_num].dev_port->OV = DISABLED;
	}
	if (device[dev_num].time_left <= 0) {
		fprintf(device_output_file, "Output device %d has recieved the char %c on clock %d\n", dev_num, device[dev_num].dev_port->data, emulation.sys_clk);
		device[dev_num].dev_port->DBA = ENABLED;

		if (device[dev_num].dev_port->IE == ENABLED) {
			interrupt_handling_process(dev_num);
		}
	}
}

/* Interrupt handling process function:
 * Function goes through the process that occurs when a device triggers an interrupt.
 * It analyzes the current program priority, and manages the interrupt trigger accordingly,
 * by either queueing it, or transfering control to the interrupt vector.
 * 
 * NOTES:
 * 1) Interrupt queue is set a simple 8 element char array.
 *	  Each element represents a device interrupt vector, and the content of the element
 *    signal the system that the interrupt vector isn't queued (if the content value = INT_OFF (-1))
 *    or if it is queued, the value is the priority value of the interrupt vector.
 *
 * 2) The current emulated interrupt process contains elements of XMakina v2, 
 *	  where 3 bits of the PSW's reserved bits are now used to save the priority of the previous PSW
 *    that was pushed to the stack when a new interrupt vector is being serviced.
 *	  This allows the process to be far less expensive when returning from an ISR.
 *
 * NOTE:
 * LAST_BYTE is defined in XMakina_Emulator_entities.h
 */
void interrupt_handling_process(unsigned char dev_num)
{
	if (device[dev_num].int_vector.INT_PSW->PRIORITY <= reg_file.PSW.PRIORITY) {
		interrupt_queue[dev_num] = device[dev_num].int_vector.INT_PSW->PRIORITY;
	}
	else {
		sys_reg.temp_reg_A.word = reg_file.PSW.PRIORITY;

		push(&reg_file.PC.word);
		push(&reg_file.PSW.word);
		push(&reg_file.LR.word);

		reg_file.PSW.word = device[dev_num].int_vector.INT_PSW->word;
		reg_file.PSW.SLP = DISABLED;
		reg_file.PSW.prev_PRIORITY = sys_reg.temp_reg_A.word;

		reg_file.PC.word = device[dev_num].int_vector.INT_PC->word;
		reg_file.LR.word = LAST_BYTE;
	}
}

/* Interrupt return process function:
 * This function handles the process of returning from an interrupt vector.
 * First it analyses the interrupt queue to see if there's an interrupt vector
 * with higher priority than the previous PSW priority.
 * As mentioned above, this functionality was taken from XMakina v2,
 * where the PSW now stores the priority value of the previous PSW that was pushed to the stack,
 * so the process of returning from an interrupt doesn't require a heavy access to memory, which is expensive.
 * If an interrupt in the queue doesn't match the criteria, then the process will pull LR, PSW, and PC from the stack,
 * as it was determined to be the machine status with highest priority.
 *
 * NOTE:
 * LOW_BYTE_MASK and DEVICE_NUMBER_SUPPORTED are defined in XMakina_Emulator_entities.h
 */
void interrupt_return_process()
{
	int i;
	unsigned char dev_num = LOW_BYTE_MASK;	// Distinct initialization so the process can easily determine if variable hasn't been modifed.

	for (i = 0; i < DEVICE_NUMBER_SUPPORTED; i++) {
		if (interrupt_queue[i] > reg_file.PSW.prev_PRIORITY) {	// Because the interrupt queue is a signed char data type,
			dev_num = i;										// it registers INT_OFF as -1, so it will always be lower than any priority value.
		}
	}

	if (dev_num != LOW_BYTE_MASK) {
		interrupt_queue[dev_num] = INT_OFF;		// When an interrupt is serviced, its spot in the queue is cleared.
		interrupt_handling_process(dev_num);
	}
	else {
		pull(&reg_file.LR.word);
		pull(&reg_file.PSW.word);
		reg_file.PSW.SLP = DISABLED;
		pull(&reg_file.PC.word);
	}
}

/* Push function:
 * Exact same functionality as ST Reg,-SP.
 * But using that instruction would cause conflicts when attempting to include the header of that file,
 * since memory_access_instruction.cpp already has Bus_Devices_Interrupt_operations.h as a dependacy.
 * Remaking this function allows the code above to be more organized and readible,
 * since it now follows the process laid out in XMakina's ISA with the same language (Push and Pull).
 *
 * NOTE:
 * WORD_STEP is defined in XMakina_Emulator_entities.h
 */
void push(unsigned short * reg)
{
	reg_file.SP.word -= WORD_STEP;
	sys_reg.MAR = reg_file.SP.word;
	sys_reg.MBR = *reg;
	bus(WORD, WRITE);
}

/* Pull function:
 * Exact same functionality as LD +SP,Reg.
 * But using that instruction would cause conflicts when attempting to include the header of that file,
 * since memory_access_instruction.cpp already has Bus_Devices_Interrupt_operations.h as a dependacy.
 * Remaking this function allows the code above to be more organized and readible,
 * since it now follows the process laid out in XMakina's ISA with the same language (Push and Pull).
 *
 * NOTE:
 * WORD_STEP is defined in XMakina_Emulator_entities.h
 */
void pull(unsigned short * reg)
{
	sys_reg.MAR = reg_file.SP.word;
	reg_file.SP.word += WORD_STEP;
	bus(WORD, READ);
	*reg = sys_reg.MBR;
}

