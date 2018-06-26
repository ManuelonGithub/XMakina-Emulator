/*
* X-Makina Emulator Project - Loader.h
* Loader header contains all the function prototypes that the loader program utilizes
*
* Programmer: Manuel Burnay
*
*
* Date created: 26/06/2018
*/

#pragma once
#ifndef LOADER_H
#define LOADER

#define S_RECORD_MAX_SIZE 515
#define HIGH_BYTE_SHIFT (1 << 8) 

enum RECORD_VALIDATION_CODES { VALID_RECORD = 1, CHECKSUM_ERROR = -1, S_RECORD_FILE_ERROR = -2};

extern union XMakina_memory memory;
extern char program_name[20];

void loader(char * s_record_file_name);
int s_record_decoder(char * s_record);

#endif LOADER_H