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

#define S_RECORD_MAX_SIZE 1000
#define CHECKSUM_ERROR -1

extern char memory[65536];

void loader(char * s_record_file_name);
int s_record_decoder(char * s_record);

#endif LOADER_H