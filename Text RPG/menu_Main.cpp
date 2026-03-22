#include <iostream>
#include <conio.h> //ENTER = 13 / W = 119 / w = 87 / S = 115 / s = 83 /BACKSPACE = 8
#include <cstdlib>
#include <string>

#include "Player.h"
#include "menu_Main.h"
#include "menu_Character.h"
#include "menu_MapSelect.h"
#include "Utility.h"
#include "menu_shop.h"
using namespace std;


void manage_MainMenu(Player& player)
{
	utility_clearGetchBuffer();

	int main_option{ 0 };
	int back_rememberOption{ 5 }; //Remember main_option if player backs out of inner menu

	while (main_option != 1)
	{ 
		system("cls");
		main_option = mainMenu(back_rememberOption); //Collect user Input

		if (main_option == 5)
			manage_mapMenu(player);
		else if (main_option == 4)
			manage_characterMenu(player);
		else if (main_option == 3)
			manage_shop(player);
		else if (main_option == 2)
		{
			int saveSlot{ player.get_saveSlot() };
			utility_saveGame(player, saveSlot, false);
		}
	}
	cout << "\n\n\n\n\n\n";
	exit(0);
}

void print_mainMenuHeader()
{
	cout << "========================================================================================================================\n						      Hardcore RPG\n"
		"========================================================================================================================\n\n\n\n\n\n\n\n\n\n\n\n\n";
}

void openingMenu()
{
	print_mainMenuHeader();

	cout << "\n						      PRESS ENTER\n";

	while (_getch() != 13) //Forces player to only press ENTER
	{}
}

int mainMenu(int& back_rememberOption)
{
	int choice{0}; //Stores choice player makes in mainMenu

	print_mainMenuHeader();
	cout << "\n\n";

	for (int i{ back_rememberOption }; i < 6 && i > 0;)
	{

		if (i == 5)
		{
			utility_eraseLines(5);

			cout << "\n							> Explore\n							Character\n							Shop\n							Save\n							Quit";
		}

		if (i == 4)
		{
			utility_eraseLines(5);

			cout << "\n							Explore\n							> Character\n							Shop\n							Save\n							Quit";
		}

		if (i == 3)
		{
			utility_eraseLines(5);

			cout << "\n							Explore\n							Character\n							> Shop\n							Save\n							Quit";
		}

		if (i == 2)
		{
			utility_eraseLines(5);

			cout << "\n							Explore\n							Character\n							Shop\n							> Save\n							Quit";
		}

		if (i == 1)
		{
			utility_eraseLines(5);

			cout << "\n							Explore\n							Character\n							Shop\n							Save\n							> Quit";
		}


		int keyPressed = _getch(); //Store key pressed by user (W|S|ENTER)

		while(keyPressed != 119 && keyPressed != 87 && keyPressed != 115 && keyPressed != 83 && keyPressed != 13) //if user does not input W/S/BS/ENTER = do nothing
		{ 
			keyPressed = _getch();
		}
		

		if (keyPressed == 119 || keyPressed == 87) // W or w
		{
			i++;

			if (i == 6) //if i goes out of bounds, reset to opposite option
				i = 5;

			continue;
		}

		if (keyPressed == 115 || keyPressed == 83) // S or s
		{
			i--;

			if (i == 0) //if i goes out of bounds, reset to opposite option
				i = 1;

			continue;
		}

		if (keyPressed == 13) // ENTER
		{
			back_rememberOption = i;
			choice = i;

			break;
		}

	}

	return choice;
}

