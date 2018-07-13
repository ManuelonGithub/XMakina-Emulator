
#include "stdafx.h"
#include "XMakina_Emulator_entities.h"

XMakina_memory memory;
//XMakina_instruction_set instruction;
XMakina_register_file reg_file;
System_registers sys_reg;
Emulation_properties emulation;

unsigned carry[2][2][2] = {		// SRC, DST, RES
	0, 0,	// 0 0 0, 0 0 1
	1, 0,	// 0 1 0, 0 1 1
	1, 0,	// 1 0 0, 1 0 1
	1, 1	// 1 1 0, 1 1 1
};
unsigned overflow[2][2][2] = {	// SRC, DST, RES
	0, 1,	// 0 0 0, 0 0 1
	0, 0,	// 0 1 0, 0 1 1
	0, 0,	// 1 0 0, 1 0 1 
	1, 0	// 1 1 0, 1 1 1
};

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
