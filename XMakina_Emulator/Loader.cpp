/*
 * X-Makina Emulator Project - Loader.cpp
 * Loader file contains all the functions that pertaining to the loader program
 *
 * Programmer: Manuel Burnay
 *
 *
 * Date created: 26/06/2018
 */

#include "stdafx.h"
#include "XMakina_Emulator_entities.h"
#include "Loader.h"


/* Loader Function: 
 * Checks if s-Record file can be open and opens it if able to. 
 * Retrieves line from file to decode the contents of it and check if there is a checksum error has occured. 
 */
void loader()
{
	printf("\n=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~ Loader  ~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=\n\n");

	char filename[100];

	printf("file name: ");
	scanf_s("%s", &filename, 100);

	FILE * s_record_file;
	if (fopen_s(&s_record_file, filename, "r") != 0) {
		printf("Error opening S-Record file. \n\n");
		return;
	}

	char s_record[S_RECORD_MAX_SIZE];

	while (fgets(s_record, S_RECORD_MAX_SIZE, s_record_file)) {
		if (s_record_decoder(s_record) == CHECKSUM_ERROR) {
			printf("S-Record checksum error. \n\n");
			return;
		}
	}

	fclose(s_record_file);

	printf("\n\n");
}


/* s_record_decoder Function:
 * Recieves a string that contains an S-Record.
 * Verifies the record validity and type. 
 * Decodes the data from the record according to the record type,
 * and performs checksum validation to verify the contents of the record.
 */
int s_record_decoder(char * s_record)
{

	// First get the number of amount of hex pairs in the record. 
	// This is done in the beginning so we can initialize the validation sum with this value.
	int i = 0, hex_pair_count = 0;
	sscanf_s(&s_record[2], "%2x", &hex_pair_count);

	unsigned int loaded_address = 0, data, checksum, validation_sum = hex_pair_count;

	// Simple check to avoid processing something that isn't an S-Record
	if (s_record[0] != 'S') {
		return S_RECORD_ERROR;
	}

	/* Initializing the address that the s-record is loading from.
	 * This value also indicates the starting address if the record is type 9,
	 * and is used when retrieving the program name for type 0 records, since it will always be 0 for that record type.
	 * The initialization is split in two parts so the data retrieved can be easily added to the validation sum.
	 */
	sscanf_s(&s_record[4], "%2x", &data);	// "Loading" the address high-byte
	loaded_address = HIGH_BYTE_SHIFT(data);	// This multiplication is equivalent to a left byte shift.
	validation_sum += data;

	sscanf_s(&s_record[6], "%2x", &data);	// "Loading" the address low-byte
	loaded_address += data;					// Since loaded address is initialized to 0, This addition is equivalent to a bitwise OR.
	validation_sum += data;

	sscanf_s(&s_record[(DATA_POS + (hex_pair_count - 3) * 2)], "%2x", &checksum);	// Retrieves the checksum hex pair from record.

	switch (s_record[1])
	{
	case ('9'):
		memory.word[LAST_WORD] = loaded_address;	// Program counter starting address is stored in PC word of Vector 15 (Reset Vector)
		reg_file.PC.word = loaded_address;
		break;

	case ('1'):
		for (i = DATA_POS; i < (DATA_POS + (hex_pair_count - 3) * 2); i += 2) {	// This reads the hex pairs between the s-record address and the checksum
			sscanf_s(&s_record[i], "%2x", &data);
			validation_sum += data;
			memory.byte[loaded_address++] = data;
		}
		break;

	case ('0'):
		for (i = DATA_POS; i < (DATA_POS + hex_pair_count - 3); i++) {	// This reads the characters pertaining to the program name
			emulation.program_name[loaded_address++] = s_record[i];
		}
		checksum = ~validation_sum & LOW_BYTE_MASK;
		break;

	default:
		break;
	}

	return checksum_validation(validation_sum, checksum);
}


/* checksum_validation Function:
 * Performs the operation that checks if the data in the s-record is consistent with the checksum value.
 */
int checksum_validation(unsigned int validation_sum, unsigned int checksum)
{
	validation_sum = ~validation_sum & 0xFF;	// Invert the bits of the validation sum and retrieve the lowest significant byte.

	if (validation_sum != checksum) {
		return CHECKSUM_ERROR;
	}
	else {
		return VALID_RECORD;
	}
}
