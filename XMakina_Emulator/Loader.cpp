/*
* X-Makina Emulator Project - Loader.cpp
* Loader file contains all the functions that consist the loader program
*
* Programmer: Manuel Burnay
*
*
* Date created: 26/06/2018
*/

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include "Loader.h"
#include "XMakina_entities.h"

/* Loader Function: 
 * Checks if s-Record file can be open and opens it if able to. 
 * Retrieves line from file to decode the contents of it and check if there is a checksum error has occured. 
 */
void loader(char * s_record_file_name)
{
	FILE * s_record_file;
	if (!fopen_s(&s_record_file, s_record_file_name, "r")) {
		printf("Error opening S-Record file. \n");
		return;
	}

	char s_record[S_RECORD_MAX_SIZE];
	while (fgets(s_record, S_RECORD_MAX_SIZE, s_record_file)) {
		if (s_record_decoder(s_record) == CHECKSUM_ERROR) {
			printf("S-Record checksum error. \n");
			return;
		}
	}

	fclose(s_record_file);
}

int s_record_decoder(char * s_record)
{
	int pair_length = 0;
	sscanf_s(&s_record[2], "%2x", &pair_length);

	unsigned int loaded_address = 0, data, checksum = 0;
	int pos = 8, validation_sum = pair_length;

	if (s_record[0] != 'S') {
		return S_RECORD_FILE_ERROR;
	}

	sscanf_s(&s_record[4], "%2x", &data);
	loaded_address = data * HIGH_BYTE_SHIFT;
	validation_sum += data;

	sscanf_s(&s_record[6], "%2x", &data);
	loaded_address += data;
	validation_sum += data;

	switch (s_record[1])
	{
	case ('9'):
		memory.word[LAST_WORD] = loaded_address;
		break;

	case ('1'):

		for (int i = 0; i < ((pair_length - 3) * 2); i += 2) {
			sscanf_s(&s_record[pos], "%2x", &data);
			validation_sum += data;
			memory.byte[loaded_address++] = data;
			pos+=2;
		}
		break;

	case ('0'):
		for (int i = 0; i < (pair_length - 3); i++) {
			program_name[i] = s_record[pos++];
		}
		break;

	default:
		break;
	}

	return 0;
}
