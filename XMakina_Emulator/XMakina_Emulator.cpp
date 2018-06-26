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

char memory[65536];
char * program_name;

int main(int argc, char * argv[])
{
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

	char text[] = "ffff";
	int pair_length = 0;
	sscanf_s(text, "%x", &pair_length);

	printf("value read: %x (%d)\n", pair_length, pair_length);

	getchar();
	getchar();

	return 0;
}
