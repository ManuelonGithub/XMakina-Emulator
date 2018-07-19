/*
 * X-Makina Emulator Project - Branching_instructions.cpp
 * Contains all the function used when the CPU cycle is executing a Branching instruction
 *
 * In order to keep the files at a minimum, while keeping everything organized,
 * Branching with link and conditional branching are both covered in this file.
 *
 * Programmer: Manuel Burnay
 *
 * Rev 1.0: Instructions work as intended and have been properly documented.
 *
 * Date created: 10/07/2018
 *
 * General file note: Unless specified, any definitions/macros should be found the file's respective header.
 */


#include "stdafx.h"
#include "Branching_instructions.h"

char (*conditional_branching_execution[]) (signed short) = { BEQ_BZ, BNE_BNZ, BC_BHS, BNC_BLO, BN, BGE, BLT, BAL };

/* NOTE:
 * The sign extension operation isn't present in the emulation because because it wasn't needed.
 * When passing the value of the offset (an unsigned short defined with either 10 or 13 bits) to a signed short argument,
 * the sign of the offset was made to extend to the 16-bits of the signed entity "label" by default.
 * So simply passing the offset by value (after it has been converted from a memory word space to memory byte space),
 * the sign extension step is completed.
 * 
 * NO_INC and PROCESS_SUCCESS is defined in XMakina_Emulator_entities.h
 */

/****************** Branching with Link Instruction ******************/

/* BL: Branching with link
 * Instruction will branch and will place the previous program counter in the link register
 */
char BL(signed short label)
{
	printf("Executing a BL instruction.\n");
	reg_file.LR = reg_file.PC;
	reg_file.PC.word += label;
	return PROCESS_SUCCESS;
}


/***************** Conditional Branching Instructions ****************/

/* BEQ/BZ: Branching if equal/zero
 * Instruction will branch if the previous operation set the PSW.Z bit
 */
char BEQ_BZ(signed short label)
{
	printf("Executing a BEQ/BZ instruction.\n");
	reg_file.PC.word += (reg_file.PSW.Z == 1) ? label : NO_INC;
	return PROCESS_SUCCESS;
}

/* BNE/BNZ: Branching if not equal/zero
 * Instruction will branch if the previous operation cleared the PSW.Z bit
 */
char BNE_BNZ(signed short label)
{
	printf("Executing a BNE/BNZ instruction.\n");
	reg_file.PC.word += (reg_file.PSW.Z == 0) ? label : NO_INC;
	return PROCESS_SUCCESS;
}

/* BC/BHS: Branching if carry/higher or same (unsigned)
 * Instruction will branch if the previous operation set the PSW.C bit
 */
char BC_BHS(signed short label)
{
	printf("Executing a BC/BHS instruction.\n");
	reg_file.PC.word += (reg_file.PSW.C == 1) ? label : NO_INC;
	return PROCESS_SUCCESS;
}

/* BNC/BLO: Branching if no carry/lower
 * Instruction will branch if the previous operation cleared the PSW.C bit
 */
char BNC_BLO(signed short label)
{
	printf("Executing a BNC/BLO instruction.\n");
	reg_file.PC.word += (reg_file.PSW.C == 0) ? label : NO_INC;
	return PROCESS_SUCCESS;
}

/* BN: Branching if negative
 * Instruction will branch if the previous operation set the PSW.N bit
 */
char BN(signed short label)
{
	printf("Executing a BN instruction.\n");
	reg_file.PC.word += (reg_file.PSW.Z == 1) ? label : NO_INC;
	return PROCESS_SUCCESS;
}

/* BGE: Branching if greater or equal (signed)
 * We can determine if the comparison instruction made before equalled to be greater or equal
 * by performing a XOR between the PSW.Z and PSW.V bits.
 * Instruction will branch if the result of the XOR is 0
 */
char BGE(signed short label)
{
	printf("Executing a BGE instruction.\n");
	reg_file.PC.word += ((reg_file.PSW.Z ^ reg_file.PSW.V) == 0) ? label : NO_INC;
	return PROCESS_SUCCESS;
}

/* BGE: Branching if less (signed)
 * We can determine if the comparison instruction made before equalled to be less
 * by performing a XOR between the PSW.Z and PSW.V bits.
 * Instruction will branch if the result of the XOR is 1
 */
char BLT(signed short label)
{
	printf("Executing a BLT instruction.\n");
	reg_file.PC.word += ((reg_file.PSW.Z ^ reg_file.PSW.V) == 1) ? label : NO_INC;
	return PROCESS_SUCCESS;
}

/* BAL: Branching always
 * Instruction will always branch
 */
char BAL(signed short label)
{
	printf("Executing a BAL instruction.\n");
	reg_file.PC.word += label;
	return PROCESS_SUCCESS;
}