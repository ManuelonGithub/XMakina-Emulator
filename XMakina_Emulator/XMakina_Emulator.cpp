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

	char filename[100];
	std::ifstream file_in;

	std::cout << "file name: ";
	std::cin >> filename;

	loader(filename);

	//char s_record[] = "S10B0000367681768176C17623";

	//int dummy_var = s_record_decoder(s_record);

	//printf("%d\n", dummy_var);

	printf("Program name: %s\n", program_name);
	printf("Starting Address: %04x\n", memory.word[LAST_WORD]);

	for (int i = 0; i < 5; i++) {
		printf("Word: %04x  | Contents: %x \n", i, memory.word[i]);
	}
	
	for (int i = 0x100; i < (0x100 + 5); i++) {
		printf("Word: %04x  | Contents: %x \n", i, memory.word[i]);
	}

	getchar();
	getchar();

	return 0;
}
