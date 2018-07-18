/*
 * X-Makina Emulator Project - XMakina_Emulator.cpp
 * Contains the Main body and the ctrl C signal catcher.
 *
 * Programmer: Manuel Burnay
 *
 * Rev 1.0: Instructions work as intended and have been properly documented.
 *
 * Date created: 26/06/2018
 */

// NOTE: All C standard libraries used are specified in stdafc.h

#include "stdafx.h"
#include "XMakina_Emulator_entities.h"
#include "Loader.h"
#include "debugger.h"
#include "CPU_operations.h"
#include "Bus_Devices_Interrupt_operations.h"

//void default_program();
void test_program();
//void test_interrupt_handling();
void ctrl_c_signal_detection();

/* Main:
 * Initializes the ctrl C signal catcher, 
 * and starts the XMakina emulation.
 * Once the user is done, it safely closes all entities of the emulation.
 */
int main(int argc, char * argv[])
{
	signal(SIGINT, (_crt_signal_t) ctrl_c_signal_detection);

	clear_emulation_properties();	// Resetting the emulation properties to ensure they have predictable initial conditions
	debugger_main_menu();			// The emulation starts with the debugger opening. From there the user can load the desired program
									// And enable any breakpoints to allow the emulation to go back to the debugger.
	while (emulation.system_status != 'Q')
	{
		CPU_cycle();
		device_management();
		debugger_triggers();
		emulation.run_clk = 0;	// System has a cpu cycle run clock that runs parallel to the system clock.
	}							// It is mainly used to manage the processing time of the output devices.

	//test_program();

	printf("XMakina emulation has been shut down. Press ENTER to exit. . . .");
	getchar();
	getchar();

	return 0;
}

/* Ctrl C signal detection function:
 * It's called when the system has detected the Ctrl C signal.
 * It simply enables a flag that gets detected by the debugger_triggers function,
 * which will cause the debugger to open.
 *
 * NOTE: TRUE is specifed in XMakina_Emulator_entities.h
 */
void ctrl_c_signal_detection()
{
	emulation.ctrl_C_detected = TRUE;
	signal(SIGINT, (_crt_signal_t)ctrl_c_signal_detection);	// Re-enabling the system to look for the signal.
}

//void default_program()
//{
//	program_name[0] = '\0';	// Initializing the program name to contain nothing in order for some prompts in the debugger to work as intended
//	char run_status;
//	char decoded_inst_type = 0;
//
//	while (emulation.system_status != QUIT_EMULATOR) {
//		emulation.system_status = debugger_main_menu();
//
//		switch (emulation.system_status)
//		{
//		case (RUN_PROGRAM):
//			run_status = fetch();
//			if (run_status == PROCESS_FAILURE) {
//				printf("Problem has occurred during the Fetch process. Invalid value in PC (= 0x%04X).\n", reg_file.PC);
//				printf("IX = %04X\n", sys_reg.IX);
//				break;
//			}
//
//			decoded_inst_type = decode();
//			if (decoded_inst_type == INVALID_INST) {
//				printf("Problem has occurred during the Decode process. Invalid value in IX (= 0x%04X).\n", sys_reg.IX);
//				break;
//			}
//
//			run_status = execute(decoded_inst_type);
//			if (run_status == INVALID_INST) {
//				printf("Invalid data has been attempted to be executed. Data fetched for this cycle = 0x%04X.\n", sys_reg.IX);
//				break;
//			}
//			//device_management();
//			debugger_tiggers();
//
//		default:
//			break;
//		}
//	}
//}

//void test_program()
//{
//	reg_file.R0 = 1;
//	reg_file.R1 = 2;
//	reg_file.R2 = 3;
//	reg_file.R3 = 4;
//	reg_file.LR = 5;
//	reg_file.SP = 6;
//	reg_file.PSW.C = 501;
//	reg_file.PC = 8;
//
//	for (int i = 0; i < 8; i++)
//	{
//		printf("R%d = %04X\n", i, reg_file.REG[i]);
//	}
//}

//void test_program()
//{
//
//	union mini_mem {
//		unsigned char byte[10];
//		unsigned short word[5];
//		Device_port dev_mem[5];
//	};
//
//	mini_mem mem;
//
//	Device dev[5];
//
//	for (int i = 0; i < 5; i++) {
//		dev[i].dev_port = &mem.dev_mem[i];
//		dev[i].proc_time = 0;
//		dev[i].time_left = 0;
//	}
//	
//
//	mem.word[0] = 0xAA00;
//	mem.byte[0] = 1;
//	mem.word[1] = 0x0103;
//	mem.word[2] = 0x0001;
//	mem.word[3] = 0xFF02;
//	mem.word[4] = 0x3C03;
//
//	for (int i = 0; i < 5; i++) {
//		printf("Device %d memory : %02X %02X\n", i, dev[i].dev_port->data, dev[i].dev_port->control);
//		dev[i].dev_port->IE = ~(dev[i].dev_port->IE);
//	}
//
//	for (int i = 0; i < 5; i++) {
//		printf("Word %d = %04X\n", i, mem.word[i]);
//	}
//
//
//	//printf("Dev status = %02X %02X \n", mem.dev_mem.data, mem.dev_mem.control);
//
//	//dev_0.dev_mem->IE = 1;
//
//	//printf("Dev status = %02X %02X \n", mem.dev_0_mem.data, mem.dev_0_mem.control);
//
//	//printf("word = %04X\n", mem.word[0]);
//}

//void test_program()
//{
//	union IX_bit_format {
//		unsigned short word;
//		struct {
//			unsigned short undescriptive_0 : 8;
//			unsigned short ALU_cat_sig_bit : 1;
//			unsigned short undescriptive_1 : 2;
//			unsigned short multi_purporse : 3;
//			unsigned short category : 2;
//		} bits;
//	};
//
//	IX_bit_format test;
//
//	test.word = 0;
//	test.bits.category = 3;
//	test.bits.multi_purporse = 3;
//	//test.Inst.ALU_cat_sig_bit = 1;
//	//test.Inst.a = 5;
//
//	int a = BRANCH_SIG(test.bits.multi_purporse);
//
//	printf("branch sig = %d\n", a);
//	printf("word = %04X\n", test.word);
//}

//void test_program()
//{
//	typedef union reg {
//		unsigned short word;
//		struct {
//			unsigned char LSB;
//			unsigned char MSB;
//		};
//	} reg;
//
//	typedef union register_file {
//		struct {
//			reg R0, R1, R2, R3, LR, SP;
//			PSW_reg_format PSW;
//			reg PC;
//		};
//		reg REG[XMAKINA_CPU_REG_COUNT];
//	} register_file;
//
//	register_file RF;
//	unsigned short x = 0xfffe, y = 0x1234;
//
//	RF.R0.word = x;
//	RF.REG[7].word = y;
//
//	printf("WORD = %04X, MSB = %02X, LSB = %02X\n", RF.REG[0].word, RF.REG[0].MSB, RF.REG[0].LSB);
//	printf("WORD = %04X, MSB = %02X, LSB = %02X\n", RF.REG[7].word, RF.REG[7].MSB, RF.REG[7].LSB);
//
//	RF.REG[0].LSB = SINGLE_RIGHT_SHIFT(RF.REG[0].LSB);
//	RF.REG[7].LSB = x;
//
//	printf("WORD = %04X, MSB = %02X, LSB = %02X\n", RF.REG[0].word, RF.REG[0].MSB, RF.REG[0].LSB);
//	printf("WORD = %04X, MSB = %02X, LSB = %02X\n", RF.REG[7].word, RF.REG[7].MSB, RF.REG[7].LSB);
//}

//void test_program()
//{
//	unsigned short test_a = 0xf00f, test_b;
//
//	reg_file.PSW.PRIORITY = 7;
//
//	test_b = reg_file.PSW.PRIORITY;
//
//	printf("a = %X, b= %X\n", test_a, test_b);
//	return;
//}

void test_program()
{
	device_init();
	device_management();
	device_management();
	emulation.sys_clk = 10;
	device_management();
	device_management();
}

//void test_interrupt_handling()
//{
//	device_interrupt_vector vect;
//
//	vect.word[0] = &memory.word[0x7fc0];
//	vect.word[1] = &memory.word[0x7fc2];
//
//	vect.INT_PC->word = 0x100;
//	vect.INT_PSW->PRIORITY = 7;
//
//	printf("Vector PSW: %04X | Vector PC: %04X\n", memory.word[0x7fc0], memory.word[0x7fc2]);
//}