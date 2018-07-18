/*
 * X-Makina Emulator Project - CPU_operations.cpp
 * CPU_operations file contains all the functions that pertain to a XMakine machine CPU cycle 
 * (except the execution for the instructions, they have been split between multiple files to keep the code organized)
 *
 * Programmer: Manuel Burnay
 *
 *  Rev 1.0: Instructions work as intended and have been properly documented.
 *
 * Date created: 30/06/2018
 */

#include "stdafx.h"
#include "CPU_operations.h"
#include "Bus_Devices_Interrupt_operations.h"

XMakina_instruction_set inst_set;

/* Notes on CPU Operation:
 * PROCESS_FAILURE/PROCESS_SUCCESS are general output flags used through the several steps of a CPU cycle.
 * This is my solution to keep the machine operating in a predictable manner, and not let it deviate to unpredicable behaviour.
 * Examples of these are when the fetch process finds an odd number in the PC counter, or when non-instruction data gets fetched and attempted to be decoded or even executed.
 * (if the bit format matches one of the instruction types, that incorrect data can go up to the execution phase)
 * Specially with the use of arrays of function pointers, it's important to only allow the emulation to work with data it's prepared for.
 * This also is used to notify the user of a problem within the program, or perhaps of the emulation.
 */

/* CPU cycle function:
 * Function encompasses the standard cpu cycle of fetch -> decode -> execute.
 * It calls the separate functions accordingly, and performs a check after each step
 * to make sure an invalid instruction doesn't go further through the cycle then it needs to.
 * 
 * NOTES:
 * For this particular emulation, each step of the process took one clock cycle to perform.
 * This is just a particular aspect of the emulation, it does not change how the system behaves otherwise.
 *
 * NORMAL_OP_CLK_INC and PROCESS_FAILURE is defined in XMakina_Emulator_entities.h
 * ENABLED is defined in Bus_Devices_Interrupt_operations.h
 */
void CPU_cycle()
{
	inst_set.opcode = &sys_reg.IX.word;

	if (reg_file.PSW.SLP == ENABLED) {
		fetch();
		emulation.sys_clk += NORMAL_OP_CLK_INC;
		emulation.run_clk += NORMAL_OP_CLK_INC;
		if (emulation.current_cycle_status == PROCESS_FAILURE) {
			printf("Problem has occurred during the Fetch process. Invalid value in PC (= 0x%04X).\n", reg_file.PC.word);
			return;
		}


		decode();
		emulation.sys_clk += NORMAL_OP_CLK_INC;
		emulation.run_clk += NORMAL_OP_CLK_INC;
		if (emulation.current_cycle_status == INVALID_INST) {
			printf("Problem has occurred during the Decode process. Invalid data was fetched (= 0x%04X).\n", sys_reg.IX.word);
			return;
		}

		execute();
		emulation.sys_clk += NORMAL_OP_CLK_INC;
		emulation.run_clk += NORMAL_OP_CLK_INC;
		if (emulation.current_cycle_status == INVALID_INST) {
			printf("Invalid data has been attempted to be executed. Invalid data fetched = 0x%04X.\n", sys_reg.IX.word);
			return;
		}
	}
}

/* Fetch function:
 * Emulates part of the CPU cycle that fetches instruction data from memory (whose address is taken from the Program Counter)
 * And places the instruction word in the instruction register, to be decoded and then executed.
 *
 * LAST_BYTE, WORD_STEP, PROCESS_FAILURE and PROCESS_SUCCESS is defined in XMakina_Emulator_entities.h
 * WORD and READ are defined in Bus_Devices_Interrupt_operations.h
 */
void fetch()
{
	if (reg_file.PC.word % 2 != 0) {			// Program counter should not have an odd value
		if (reg_file.PC.word == LAST_BYTE) {
			interrupt_return_process();			// This is the only exception to the rule
		}
		else {
			emulation.current_cycle_status = PROCESS_FAILURE;	// If the exception isn't met, the cycle is stopped and the user is notified of the error
			return;
		}
	}

	sys_reg.MAR = reg_file.PC.word;
	bus(WORD, READ);
	sys_reg.IX.word = sys_reg.MBR;
	reg_file.PC.word += WORD_STEP;

	emulation.current_cycle_status = PROCESS_SUCCESS;
}


/* Decode function:
 * Emulates part of the CPU cycle that decodes the instruction data to figure out what instruction needs to be executed, and the operands involved in the execution.
 * This decoder is quite simple, as it only decodes up to the type of instruction is being execution, and not the particular instruction itself.
 * This is because from the instruction type, the particular instruction can be easily found within the instruction code (inst_code),
 * which relates a value to each instruction of the instruction type. 
 * 
 * Notes:
 * There are 4 instruction categories. An instruction category denotes the value in the 2 MSBs (bit 15 and 14) of the instruction word.
 * Finding the category of the instruction is the first step of the decoder.
 * There are 7 instruction types. An instruction type groups instructions that have either an identical or extremely similar operand format.
 * When naming the bits in the bit-format of each instruction type, 
 * "signature bit" is used when describing a bit that allows the process to easily distinguish between two instruction types contained in the same instruction category.
 */
void decode()
{
	switch (sys_reg.IX.bits.category)
	{
	case (BRANCHING):
		if (IX_BRANCH_SIG == 0) {
			emulation.current_cycle_inst_type = BRANCH_WITH_LINK_INST;
		}
		else {
			emulation.current_cycle_inst_type = CONDITIONAL_BRANCH_INST;
		}
		break;

	case (ALU):
		if (sys_reg.IX.bits.ALU_cat_sig_bit == 0) {
			emulation.current_cycle_inst_type = TWO_OPERAND_INST;
		}
		else {
			emulation.current_cycle_inst_type = SINGLE_REGISTER_INST;
		}
		break;

	case (MEM_ACCESS_AND_REG_INIT):
		if (IX_MEM_ACCESS_REG_INIT_INST_CODE < REG_INIT_INST_CODE_START) {
			emulation.current_cycle_inst_type = DIRECT_MEMORY_ACCESS_INST;
		}
		else {
			emulation.current_cycle_inst_type = REGISTER_INITIALIZATION_INST;
		}
		break;

	case (MEM_ACCESS_REL):
		emulation.current_cycle_inst_type = RELATIVE_MEMORY_ACCESS_INST;
		break;

	default:
		emulation.current_cycle_inst_type = INVALID_INST;
		break;
	}
}


/* Execute function:
 * Emulates part of the CPU cycle that executes the instruction decoded, 
 * using the operands laid out in the instruction word as described in the instruction type bit-format described in XMakina_Emulator_entities.h
 * Having sole Execution function allows the CPU cycle process to be laid out nicely,
 * with the "fetch -> decode -> execute" sequence being present the main (where the machine's cycle is performed).
 * The execution function also deals with filtering out any possible non-instruction data 
 * that might've slipped through the decoder, due to having a similar bit-format.
 */
void execute()
{
	switch (emulation.current_cycle_inst_type)
	{
	case (BRANCH_WITH_LINK_INST):
		emulation.current_cycle_status = BL(BRANCHING_OFFSET_WORD(inst_set.br_link->offset));
		break;

	case (CONDITIONAL_BRANCH_INST):
		emulation.current_cycle_status = 
			(*conditional_branching_execution[inst_set.cond_br->inst_code]) (BRANCHING_OFFSET_WORD(inst_set.cond_br->offset));
		break;

	case (TWO_OPERAND_INST):
		if (inst_set.two_op->inst_code > TWO_OP_INST_CODE_LIMIT)
		{
			emulation.current_cycle_status = INVALID_INST;
		}
		else {
			emulation.current_cycle_status =
				(two_operand_execution[inst_set.two_op->inst_code]) (inst_set.two_op->REG_CON_ctrl, inst_set.two_op->W_B_ctrl, inst_set.two_op->source, inst_set.two_op->dst_reg);

		}
		break;

	case (SINGLE_REGISTER_INST):
		if (inst_set.single_reg->inst_code > SINGLE_REG_INST_CODE_LIMIT) {
			emulation.current_cycle_status = INVALID_INST;
		}
		else{
			emulation.current_cycle_status =
				(*single_register_execution[inst_set.single_reg->inst_code]) (inst_set.single_reg->W_B_ctrl, inst_set.single_reg->dst_reg);
		}
		
		break;
																										// Register initialization instruction codes start go from 2 to 4,
	case (REGISTER_INITIALIZATION_INST):																// so there needs to be a check if the code goes above (meaning invalid data slipped through the decoder)
		if ((inst_set.reg_init->inst_code - REG_INIT_INST_CODE_START) > REG_INIT_INST_CODE_START) {		// Inst_code gets subtracted by 2 to shift the code from 2...4, to 0...2,
			emulation.current_cycle_status = INVALID_INST;												// which lets the function pointer array avoid having a complicated solution, 
		}																								// and allows the code to re-use an existing and fitting defined value (REG_INIT_INST_CODE_START)
		else {																							
			emulation.current_cycle_status =
				(*register_initialization_execution[(inst_set.reg_init->inst_code - REG_INIT_INST_CODE_START)]) (inst_set.reg_init->dst_reg, inst_set.reg_init->value);
		}
		break;

	case (DIRECT_MEMORY_ACCESS_INST):
		emulation.current_cycle_status =
			(*direct_memory_access_execution[inst_set.DMA->inst_code]) (inst_set.DMA->action, inst_set.DMA->W_B_ctrl, inst_set.DMA->source, inst_set.DMA->dst_reg);
		break;

	case (RELATIVE_MEMORY_ACCESS_INST):
		emulation.current_cycle_status =
			(*relative_memory_access_execution[inst_set.RMA->inst_code]) (inst_set.RMA->offset, inst_set.RMA->W_B_ctrl, inst_set.RMA->source, inst_set.RMA->dst_reg);
		break;

	default:
		emulation.current_cycle_status = INVALID_INST;
		break;
	}
}
