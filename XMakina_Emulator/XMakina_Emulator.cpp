/*
* X-Makina Emulator Project - main
* Main body checks for a file to be opened,
* starts the loader program
* and calls the debugger manu once the loading process has complete.
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

union XMakina_memory memory;
union XMakina_register_file register_file;
char program_name[MAX_PROG_NAME_SIZE];


int main(int argc, char * argv[])
{
	//register_file.ID.R0 = 1;
	//register_file.ID.PC = 2000;

	//for (int i = 0; i < 8; i++) {
	//	printf(" Register %d: %d \n", i, register_file.reg[i]);
	//}

	//int value;
	//std::string filename;
	//std::ifstream file_in;

	//std::cout << "file name: ";
	//std::cin >> filename;

	//file_in.open(filename);

	//file_in >> std::hex >> value;
	////std::cout << "input an hex value: ";
	////std::cin >> std::hex >> value;

	////std::cout << "\nvalue inputted: " << std::hex << value;
	//std::cout << value  << std::endl;

	//char text[] = "ffff";
	//int pair_length = 0;
	//sscanf_s(text, "%x", &pair_length);

	//printf("value read: %x (%d)\n", pair_length, pair_length);

	char s_record[] = "S10B0000367681768176C17623";

	int dummy_var = s_record_decoder(s_record);

	for (int i = 0; i < 15; i++)
	printf("Word: %04x  | Contents: %x \n", i, memory.word[i]);

	getchar();

	return 0;
}
