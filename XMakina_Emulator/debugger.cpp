#include "stdafx.h"
#include "XMakina_Emulator_entities.h"
#include "Loader.h"
#include "debugger.h"



char debugger()
{
	char menu_option;
	printf("=~=~=~=~=~Xmakina Debugger~=~=~=~=~=\n");

	while (1) {
		printf("Input your option (Input H for menu options):\n");
		scanf_s("%c", &menu_option);

		menu_option = toupper(menu_option);

		switch (menu_option)
		{
		case (BREAKPOINT_MENU):
			breakpoint_menu();
			break;

		case (RUN_PROGRAM):
			return RUN_PROGRAM;

		case (QUIT_PROGRAM):
			return QUIT_PROGRAM;

		case (REG_FILE_MENU):
			reg_file_menu();

		case (MEMORY_MENU):
			memory_menu();
			break;

		case (PROGRAM_STEP):
			return PROGRAM_STEP;

		case (SANITY_CHECK_MENU):
			sanity_check_menu();
			break;

		case (LOAD_PROGRAM):
			loader();
			break;

		case (MENU_HELP):
			printf("Debugger Menu options:\n");
			printf("B = Breakpoint menu | R = Run program | Q = Quit program | F = Register File Menu\n");
			printf("M = Memory Menu     | S = Sanity check menu              | L = Load program\n");
			printf("Not inputting an option and pressing ENTER will step through the program.\n\n");
			break;

		default:
			printf("Not a valid menu option.\n");
			break;
		}
	}
}

void breakpoint_menu()
{

}

void reg_file_menu()
{

}

void memory_menu()
{

}

void sanity_check_menu()
{

}

