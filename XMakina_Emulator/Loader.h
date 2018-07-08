/*
* X-Makina Emulator Project - Loader.h
* Loader header contains all the function prototypes and definitions that the loader program utilizes
*
* Programmer: Manuel Burnay
*
*
* Date created: 26/06/2018
*/

#include "XMakina_Emulator_entities.h"

#pragma once
#ifndef LOADER_H
#define LOADER_H

#define S_RECORD_MAX_SIZE 510			// this value comes from the S-record document, where it states that it can assumed that an S-Record will not exceed 510 characters
#define HIGH_BYTE_SHIFT(x) (x*(1 << 8))
#define DATA_POS 8

enum RECORD_VALIDATION_CODES { VALID_RECORD = 1, CHECKSUM_ERROR = -1, S_RECORD_ERROR = -2};

extern union XMakina_memory memory;
extern unsigned short REG_CON_table[REG_OR_CON][XMAKINA_CPU_REG_COUNT];
extern char program_name[MAX_PROG_NAME_SIZE];

void loader();

int s_record_decoder(char * s_record);

int checksum_validation(unsigned int validation_sum, unsigned int checksum);

#endif //!LOADER_H