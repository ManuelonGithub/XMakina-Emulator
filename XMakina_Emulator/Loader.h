/*
 * X-Makina Emulator Project - Loader.h
 * Loader header contains all the function prototypes and definitions that the loader program utilizes
 *
 * Programmer: Manuel Burnay
 *
 * Rev 1.0: Instructions work as intended and have been properly documented.
 *
 * Date created: 26/06/2018
 */

#include "XMakina_Emulator_entities.h"

#pragma once
#ifndef LOADER_H
#define LOADER_H

#define S_RECORD_MAX_SIZE 510	// this value comes from the S-record documentation, where it states that it can assumed that an S-Record will not exceed 510 characters
#define DATA_POS 8				// this value is used to skip over the first 8 characters of the s-record that specify the record type, pair count, and record's starting address

enum RECORD_VALIDATION_CODES { VALID_RECORD = 1, CHECKSUM_ERROR = -1, S_RECORD_ERROR = -2};

extern XMakina_memory memory;
extern XMakina_register_file reg_file;
extern Emulation_properties emulation;

void loader();

int s_record_decoder(char * s_record);

int checksum_validation(unsigned int validation_sum, unsigned int checksum);

#endif //!LOADER_H