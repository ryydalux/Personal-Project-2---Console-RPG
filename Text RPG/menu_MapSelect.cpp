#include <iostream>

#include "menu_MapSelect.h"
#include "map_dataBase.h"
#include "map_ID.h"
#include "conio.h"
#include "Utility.h"
#include "Player.h"
#include "combat.h"
using namespace std;


int manage_mapMenu(Player& player)
{
	int rememberMap{ 0 };
	int mapChoice;

	system("cls");

	while (true)
	{
		 //If player backs from mapAction menu, put cursor back to map selected.
		mapChoice = mapMenu(player, rememberMap);

		if (mapChoice == -1)
			break;

		mapID ID = static_cast<mapID>(mapChoice);

		if (ID == mapID::None)
			break;

		baseMap map = mapDataBase[ID];

		string actionChoice{ "" };
		actionChoice = mapAction(player, map, ID); //Prompt user with "Train" or "Fight Boss"

		if (actionChoice == "Back")
			continue;

		manage_combat(player, ID, actionChoice);

		//manage_combat(player, ID, actionChoice); //Go back to main menu by returning -1 at the end of combat          TO DECLARE IN COMBAT.H WHEN DEVELOPPING COMBAT
	}

	return -1;
}

void print_mapMenuHeader()
{
	cout << "========================================================================================================================\n						       Map Select\n"
		"========================================================================================================================\n\n\n\n";
}

void print_mapHeader(baseMap map)
{
	cout << "========================================================================================================================\n						     " << map.name <<
		"\n========================================================================================================================\n\n\n\n";
}

int mapMenu(Player& player, int& rememberMap)
{
	int cursorIndex{ rememberMap };
	
	while(true)
	{ 
		system("cls");
		print_mapMenuHeader();

		cout << "\n\n                                                 ------ Map List ------\n\n";

		for (int i{ 0 }; i < player.mapsUnlocked.size(); i++)
		{
			mapID ID = player.mapsUnlocked[i];
			baseMap map = mapDataBase[ID];

			cout << "                                                  ";

			if (cursorIndex == i)
				cout << "> ";
			else
				cout << "  ";

			cout << map.name << endl;
		}

		int keyPressed = _getch();

		while (keyPressed != 119 && keyPressed != 87 && keyPressed != 115 && keyPressed != 83 && keyPressed != 13 && keyPressed != 8)
			keyPressed = _getch();
	
		
		if (keyPressed == 119 || keyPressed == 87) //W
		{
			cursorIndex--;

			if (cursorIndex < 0)
				cursorIndex = 0;
		}
		if (keyPressed == 115 || keyPressed == 83) //S
		{
			cursorIndex++;

			if (cursorIndex >= player.mapsUnlocked.size())
				cursorIndex = player.mapsUnlocked.size() - 1;
		}

		if (keyPressed == 13)
		{
			rememberMap = cursorIndex;
			return cursorIndex + 1;
		}

		if (keyPressed == 8) //BS
			return -1;
		
	}
}

string mapAction(Player& player, baseMap& map, mapID& ID)
{
	int cursorIndex{ 0 };

	while (true)
	{
		system("cls");
		print_mapHeader(map);

		mapID highestMap = player.mapsUnlocked[player.get_mapsUnlockedSize() - 1];


		if (ID == highestMap) //If player selected highest unlocked map, give option to fight boss.
		{ 
			for (int i{ 0 }; i < 2; i++)
			{
				cout << "                                  ";

				if (cursorIndex == i)
					cout << "> ";
				else
					cout << "  ";

				cout << "Train                                  ";

				i++;

				if (cursorIndex == i)
					cout << "> ";
				else
					cout << "  ";

				cout << "Fight Boss                               ";
			}
		}
		else
		{
			cout << "                                                      > Train";

			int keyPressed = _getch();

			while (keyPressed != 8 && keyPressed != 13)
				keyPressed = _getch();

			if (keyPressed == 8) //BS
				return "Back";

			if (keyPressed == 13) //ENTER
				return "Train";
		}

		int keyPressed = _getch();

		while (keyPressed != 8 && keyPressed != 13 && keyPressed != 68 && keyPressed != 100 && keyPressed != 97 && keyPressed != 65)
			keyPressed = _getch();

		if (keyPressed == 8) //BS
			return "Back";

		if (keyPressed == 13) //ENTER
		{
			if (cursorIndex == 0)
				return "Train";
			else
				return "Boss";
		}

		if (keyPressed == 100 || keyPressed == 68) //D
		{
			cursorIndex++;

			if (cursorIndex > 1)
				cursorIndex = 1;
		}
		if (keyPressed == 65 || keyPressed == 97) //D
		{
			cursorIndex--;

			if (cursorIndex < 0)
				cursorIndex = 0;
		}
	}
}


