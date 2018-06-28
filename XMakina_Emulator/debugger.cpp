#include "stdafx.h"
#include "XMakina_Emulator_entities.h"
#include "Loader.h"
#include "debugger.h"



char debugger()
{
	char menu_option;
	printf("=~=~=~=~=~Xmakina Debugger~=~=~=~=~=\n");

	while (1) {
		printf("Select an option:\n");
		scanf_s("%c", menu_option);

		switch (menu_option)
		{
		case (BREAKPOINTS):
			breakpoint_menu();

		case (RUN_PROGRAM):
			return RUN_PROGRAM;

		case (QUIT_PROGRAM):
			return QUIT_PROGRAM;

		case (VIEW_REG_FILE):
			reg_file_menu();

		case (VIEW_MEMORY):

		case (PROGRAM_STEP):
			return PROGRAM_STEP;

		case (SANITY_CHECK):

		case (LOAD_PROGRAM):

		case (MENU_OPTIONS):

		default:
			printf("Not a valid menu option.");
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