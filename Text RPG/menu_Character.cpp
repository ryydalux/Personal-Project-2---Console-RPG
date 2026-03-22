#include <iostream>
#include <conio.h>

#include "Player.h"
#include "menu_Main.h"
#include "menu_Character.h"
#include "menu_Inventory.h"
#include "Utility.h"
#include "menu_Status.h"
#include "menu_equipement.h"
#include "menu_Skills.h"
using namespace std;


int manage_characterMenu(Player& player)
{
	int character_option{0};
	int back_rememberOption{4};//Remember character_option if player backs out of inner menu

	while (character_option != -1)
	{ 
		system("cls");
		character_option = characterMenu(back_rememberOption);

		if (character_option == 4)
			manage_statusMenu(player);

		if (character_option == 3)
			manage_equipementMenu(player);

		if (character_option == 2)
			manage_inventoryMenu(player);

		if (character_option == 1)
			manage_skillMenu(player);
	}

	return character_option;
}

void print_characterMenuHeader()
{
	cout << "========================================================================================================================\n						     Character Menu\n"
		"========================================================================================================================\n\n\n\n\n\n\n\n\n\n\n\n";
}

int characterMenu(int& back_rememberOption)
{
	int choice{ 0 }; //Stores choice player makes in mainMenu

	print_characterMenuHeader();
	cout << "\n\n";

	for (int i{ back_rememberOption }; i < 5 && i > 0;)
	{

		if (i == 4)
		{
			utility_eraseLines(4);

			cout << "\n							> Status\n							Equipement\n							Inventory\n							Skills";
		}
		if (i == 3)
		{
			utility_eraseLines(4);

			cout << "\n							Status\n							> Equipement\n							Inventory\n							Skills";
		}
		if (i == 2)
		{
			utility_eraseLines(4);

			cout << "\n							Status\n							Equipement\n							> Inventory\n							Skills";
		}
		if (i == 1)
		{
			utility_eraseLines(4);

			cout << "\n							Status\n							Equipement\n							Inventory\n							> Skills";
		}


		int keyPressed = _getch(); //Store key pressed by user (W|S|ENTER)

		while (keyPressed != 119 && keyPressed != 87 && keyPressed != 115 && keyPressed != 83 && keyPressed != 13 && keyPressed != 8) //if user does not input W/S/BS/ENTER = do nothing
		{
			keyPressed = _getch();
		}


		if (keyPressed == 119 || keyPressed == 87) // W or w
		{
			i++;

			if (i == 5) //if i goes out of bounds, reset to opposite option
				i = 1;

			continue;
		}

		if (keyPressed == 115 || keyPressed == 83) // S or s
		{
			i--;

			if (i == 0) //if i goes out of bounds, reset to opposite option
				i = 4;

			continue;
		}

		if (keyPressed == 13) // ENTER
		{
			back_rememberOption = i;
			choice = i;

			break;
		}

		if (keyPressed == 8) //BS
		{
			choice = -1;
			break;
		}

	}

	return choice;

}