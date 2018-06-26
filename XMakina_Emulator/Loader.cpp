#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include "Loader.h"

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


	return 0;
}
