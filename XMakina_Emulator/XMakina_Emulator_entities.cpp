/*
 * X-Makina Emulator Project - XMakina_Emulator_entities.cpp
 * Initializes most system global variables
 * and calls update_PSW function algorithm.
 *
 * Programmer: Manuel Burnay
 *
 * Rev 1.0: Instructions work as intended and have been properly documented.
 *
 * Date created: 12/07/2018
 */

#include "stdafx.h"
#include "XMakina_Emulator_entities.h"

// Initializes the main entities of the machine that are used through it all.
XMakina_memory memory;
//XMakina_instruction_set instruction;
XMakina_register_file reg_file;
System_registers sys_reg;
Emulation_properties emulation;

// Table determines the value of the carry bit based on the MSBi of src, dst, and res.
unsigned carry[2][2][2] = {		// SRC, DST, RES
	0, 0,						// 0 0 0, 0 0 1
	1, 0,						// 0 1 0, 0 1 1
	1, 0,						// 1 0 0, 1 0 1
	1, 1						// 1 1 0, 1 1 1
};

// Table determines the value of the overflow bit based on the MSBi of src, dst, and res.
unsigned overflow[2][2][2] = {	// SRC, DST, RES
	0, 1,						// 0 0 0, 0 0 1
	0, 0,						// 0 1 0, 0 1 1
	0, 0,						// 1 0 0, 1 0 1 
	1, 0						// 1 1 0, 1 1 1
};

/* Update PSW function:
 * Contains the general algorithm that updates the PSW based on the values of the destination, source, and the result of the operation
 * Algorithm changes slight based on if the operation was performed in a "byte-space" or "word-space".
 */
void update_PSW(unsigned short src, unsigned short dst, unsigned short res, unsigned char W_B_ctrl)
{
	unsigned short mss, msd, msr; /* Most significant src, dst, and res bits */

	if (W_B_ctrl == 0)
	{
		mss = BIT_CHECK(src, WORD_MSBi);
		msd = BIT_CHECK(dst, WORD_MSBi);
		msr = BIT_CHECK(res, WORD_MSBi);
	}
	else /* Byte */
	{
		mss = BIT_CHECK(src, BYTE_MSBi);
		msd = BIT_CHECK(dst, BYTE_MSBi);
		msr = BIT_CHECK(res, BYTE_MSBi);
	}

	/* Carry */
	reg_file.PSW.C = carry[mss][msd][msr];
	/* Zero */
	reg_file.PSW.Z = (res == 0);
	/* Negative */
	reg_file.PSW.N = (msr == 1);
	/* oVerflow */
	reg_file.PSW.V = overflow[mss][msd][msr];
}
