#include <iostream>
#include <string>
#include <ctime>
#include <Windows.h>

#include "Utility.h"
#include "Player.h"
#include "menu_Main.h"
#include "menu_LoadSave.h"
#include "menu_shop.h"

int main()
{
	setRNGSeed(); //Sets seed
	cout << "\x1b[?25l"; //Hides cursor

	Player player; //Empty player variable just to pass around program
	
	openingMenu();
	manage_LoadMenu(player);

	if (player.shop.size() == 0) //If player is newly created (new save), initialize shop
	{
		rerollShop(player);
		utility_saveGame(player, player.get_saveSlot(), true);
	}
	
	manage_MainMenu(player);
}




