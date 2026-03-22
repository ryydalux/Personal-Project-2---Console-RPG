#include <fstream>
#include <iostream>
#include <conio.h>
#include <filesystem>
#include <string>

#include "menu_LoadSave.h"
#include "Utility.h"
#include "Player.h"
#include "item_ID.h"
#include "item_dataBase.h"
#include "skill_ID.h"
#include "skill_dataBase.h"
using namespace std;

void manage_LoadMenu(Player& player)
{

	utility_clearGetchBuffer(); //Ignore any input entered during loading

	int saveSlot;
	bool newSave;

	system("cls");

	saveSlot = get_SlotChoice();
	newSave = checkSlot(saveSlot); //Checks if save slot selected exists or not


	if (newSave == true)
		create_newSave(player, saveSlot);
	else
		load_Save(player, saveSlot);
}


void print_chooseSaveHeader()
{
	cout << "========================================================================================================================\n						      Choose a Save\n"
		"========================================================================================================================\n\n\n\n\n\n\n\n\n\n\n\n";
}

void print_newSaveHeader()
{
	cout << "========================================================================================================================\n						    Creating a Save\n"
		"========================================================================================================================\n\n\n\n\n\n\n\n\n\n";
}

void print_loadSaveHeader()
{
	cout << "========================================================================================================================\n						       Loading Save\n"
		"========================================================================================================================\n\n\n\n\n\n\n\n\n\n";
}

int get_SlotChoice()
{
	int choice{ 0 };

	print_chooseSaveHeader();

	for (int i{ 3 }; i > 0 && i < 4;)
	{

		if (i == 3)
		{
			utility_eraseLines(3);

			cout << "\n						     > Save Slot #1\n						     Save Slot #2\n						     Save Slot #3";
			choice = 1;
		}

		if (i == 2)
		{
			utility_eraseLines(3);

			cout << "\n						     Save Slot #1\n						     > Save Slot #2\n						     Save Slot #3";
			choice = 2;
		}

		if (i == 1)
		{
			utility_eraseLines(3);

			cout << "\n						     Save Slot #1\n						     Save Slot #2\n						     > Save Slot #3";
			choice = 3;
		}


		int keyPressed = _getch();

		while (keyPressed != 119 && keyPressed != 87 && keyPressed != 115 && keyPressed != 83 && keyPressed != 13) //if user does not input W/S/BS/ENTER = do nothing
		{
			keyPressed = _getch();
		}

		if (keyPressed == 119 || keyPressed == 87) // W or w
		{
			i++;

			if (i == 4) //if i goes out of bounds, reset to opposite option
				i = 1;

			continue;
		}

		if (keyPressed == 115 || keyPressed == 83) // S or s
		{
			i--;

			if (i == 0) //if i goes out of bounds, reset to opposite option
				i = 3;

			continue;
		}

		if (keyPressed == 13) // ENTER
		{
			break;
		}

	}

	return choice;
}

bool checkSlot(int saveSlot) 
{
	bool newSave;
	ifstream fin;
	string savePath{ utility_getSavePath() };


	if (saveSlot == 1)
		fin.open(savePath + "\\save1.txt");
	else if (saveSlot == 2)
		fin.open(savePath + "\\save2.txt");
	else if (saveSlot == 3)
		fin.open(savePath + "\\save3.txt");

	
	if (!fin)
	{
		newSave = true;

		filesystem::create_directories(savePath); //Checks if savePath exists, if not -> create it
	}
	else
		newSave = false;

	fin.close();

	return newSave;
}

void create_newSave(Player& player ,int saveSlot)
{
	ofstream fou;
	string savePath{utility_getSavePath()};

	system("cls");
	print_newSaveHeader();

	cout << "						     Please wait...";

	waitS(2);

	if (saveSlot == 1)
		fou.open(savePath + "\\save1.txt");
	else if (saveSlot == 2)
		fou.open(savePath + "\\save2.txt");
	else if (saveSlot == 3)
		fou.open(savePath + "\\save3.txt");

	if (!fou)
		cout << "\n						     Error creating save file...";
	else
		cout << "\n					     Save file created succesfully.";

	fou.close(); //Close because utility_saveGame will need to re-open output file stream

	waitS(2);

	string nameChosen;
	utility_eraseLines(3);

	utility_clearGetchBuffer();

	cout << "\n					     Please name your character: ";
	getline(cin, nameChosen);

	cout << "\n					     Creating player character...";

	player = Player(nameChosen, saveSlot); //Creating player object
	waitS(2);

	utility_saveGame(player, saveSlot, false);

	waitS(2);
}

void load_Save(Player& player, int saveSlot)
{
	ifstream fin;
	string savePath{ utility_getSavePath() };

	system("cls");
	print_loadSaveHeader();

	cout << "\n						    Please wait... ";

	waitS(2);

	//Character Stats
	int save{saveSlot};
	string name{ "" };
	string status{ "" };

	int lvl{ 0 };
	int requiredXP{ 0 };
	int currentXP{ 0 };

	int maxHP{ 0 };
	int currentHP{ 0 };

	int maxMP{ 0 };
	int currentMP{ 0 };

	int hpRegen{ 0 };
	int mpRegen{ 0 };

	int atk{ 0 };
	int spellPower{ 0 };
	int defense{ 0 };
	int magicResist{ 0 };

	int lifesteal{ 0 };
	int spellVamp{ 0 };

	int critChance{ 0 };
	double critMult{ 0 };
	int hitChance{ 0 };
	int dodgeChance{ 0 };

	int thorns{ 0 };

	int copper{ 0 };
	int silver{ 0 };
	int gold{ 0 };

	int fightCount{ 0 };

	if (saveSlot == 1)
		fin.open(savePath + "\\save1.txt");
	else if (saveSlot == 2)
		fin.open(savePath + "\\save2.txt");
	else if (saveSlot == 3)
		fin.open(savePath + "\\save3.txt");

	if (!fin)
		cout << "\n						    Error Loading save";
	else
		cout << "\n						    Fetching character stats... ";

	waitS(1);



	//Loading character stats
	string check;

	while (true)
	{
		//If save file corrupted or empty = delete file and shut down program.
		if (!fin) 
		{
			fin.close();

			system("cls");
			print_loadSaveHeader();

			string savePath = utility_getSavePath();
			
			waitS(1);
			cout << "\n					   Error: Save file empty or corrupted.";
	
			if (saveSlot == 1)
			{
				filesystem::path corrSave = savePath + "\\save1.txt";
				filesystem::remove(corrSave);
			}
			else if (saveSlot == 2)
			{
				filesystem::path corrSave = savePath + "\\save2.txt";
				filesystem::remove(corrSave);
			}
			else if (saveSlot == 3)
			{
				filesystem::path corrSave = savePath + "\\save3.txt";
				filesystem::remove(corrSave);
			}

			waitS(2);
			cout << "\n					          Save file deleted safely.\n\n\n\n\n";
			

			exit(0);
		}


		fin >> check;

		if (check == "Name")
		{
			fin >> check;
			name = check;

			while (check != "Status") //Get full name
			{
				fin >> check;

				if (check == "Status")
					break;
				else
					name += " " + check;
			}
		}
		if (check == "Status")
		{
			fin >> check;
			status = check;
		}
		if (check == "Level")
		{
			fin >> check;
			lvl = stoi(check);
		}
		if (check == "RequiredXP")
		{
			fin >> check;
			requiredXP = stoi(check);
		}
		if (check == "CurrentXP")
		{
			fin >> check;
			currentXP = stoi(check);
		}
		if (check == "MaxHP")
		{
			fin >> check;
			maxHP = stoi(check);
		}
		if (check == "CurrentHP")
		{
			fin >> check;
			currentHP = stoi(check);
		}
		if (check == "MaxMP")
		{
			fin >> check;
			maxMP = stoi(check);
		}
		if (check == "CurrentMP")
		{
			fin >> check;
			currentMP = stoi(check);
		}
		if (check == "hpRegen")
		{
			fin >> check;
			hpRegen = stoi(check);
		}
		if (check == "mpRegen")
		{
			fin >> check;
			mpRegen = stoi(check);
		}
		if (check == "Attack")
		{
			fin >> check;
			atk = stoi(check);
		}
		if (check == "SpellPower")
		{
			fin >> check;
			spellPower = stoi(check);
		}
		if (check == "Defense")
		{
			fin >> check;
			defense = stoi(check);
		}
		if (check == "MagicResist")
		{
			fin >> check;
			magicResist = stoi(check);
		}
		if (check == "LifeSteal")
		{
			fin >> check;
			lifesteal = stoi(check);
		}
		if (check == "SpellVamp")
		{
			fin >> check;
			spellVamp = stoi(check);
		}
		if (check == "CritChance")
		{
			fin >> check;
			critChance = stoi(check);
		}
		if (check == "CritMult")
		{
			fin >> check;
			critMult = stod(check);
		}
		if (check == "HitChance")
		{
			fin >> check;
			hitChance = stoi(check);
		}
		if (check == "DodgeChance")
		{
			fin >> check;
			dodgeChance = stoi(check);
		}
		if (check == "Thorns")
		{
			fin >> check;
			thorns = stoi(check);
		}
		if (check == "CopperCoins")
		{
			fin >> check;
			copper = stoi(check);
		}
		if (check == "SilverCoins")
		{
			fin >> check;
			silver = stoi(check);
		}
		if (check == "GoldCoins")
		{
			fin >> check;
			gold = stoi(check);
		}
		if (check == "FightCount")
		{
			fin >> check;
			fightCount = stoi(check);
			break;
		}

	}

	player = Player(save, name, status, lvl, requiredXP, currentXP, maxHP, currentHP, maxMP, currentMP, hpRegen, mpRegen, atk, spellPower, defense, magicResist, lifesteal, spellVamp, 
		            critChance, critMult, hitChance, dodgeChance, thorns, copper, silver, gold, fightCount);

	cout << "\n						    Character stats retrieved...";

	//Loading character's unlocked maps
	while(true)
	{
		fin >> check;

		if (check == "[Equipement]") //If reached next section of save file, stop reading
			break;

		if (check == "Meadows")
		{
			player.mapsUnlocked.push_back(mapID::Meadows);
		}
		if (check == "DarkForest")
		{
			player.mapsUnlocked.push_back(mapID::DarkForest);
		}
		if (check == "UndeadCatacombs")
		{
			player.mapsUnlocked.push_back(mapID::UndeadCatacombs);
		}
		if (check == "SunkenPassage")
		{
			player.mapsUnlocked.push_back(mapID::SunkenPassage);
		}
		if (check == "DeadShore")
		{
			player.mapsUnlocked.push_back(mapID::DeadShore);
		}
	
	}

	int idRead; //To convert string to int (and then convert int to itemID)

	//Loading character's equipped items
	while (true)
	{
		fin >> check;

		if (check == "[Inventory]") //If reached next section of save file, stop reading
			break;

		if (check == "Weapon")
		{
			fin >> check;
			idRead = stoi(check);
			player.e_weapon = static_cast<itemID>(idRead);
		}
		if (check == "Shield")
		{
			fin >> check;
			idRead = stoi(check);
			player.e_shield = static_cast<itemID>(idRead);
		}
		if (check == "Head")
		{
			fin >> check;
			idRead = stoi(check);
			player.e_head = static_cast<itemID>(idRead);
		}
		if (check == "Shoulders")
		{
			fin >> check;
			idRead = stoi(check);
			player.e_shoulders = static_cast<itemID>(idRead);
		}
		if (check == "Cape")
		{
			fin >> check;
			idRead = stoi(check);
			player.e_cape = static_cast<itemID>(idRead);
		}
		if (check == "Chest")
		{
			fin >> check;
			idRead = stoi(check);
			player.e_chest = static_cast<itemID>(idRead);
		}
		if (check == "Arms")
		{
			fin >> check;
			idRead = stoi(check);
			player.e_arms = static_cast<itemID>(idRead);
		}
		if (check == "Legs")
		{
			fin >> check;
			idRead = stoi(check);
			player.e_legs = static_cast<itemID>(idRead);
		}
		if (check == "Feet")
		{
			fin >> check;
			idRead = stoi(check);
			player.e_feet = static_cast<itemID>(idRead);
		}
		if (check == "Ring1")
		{
			fin >> check;
			idRead = stoi(check);
			player.e_ring1 = static_cast<itemID>(idRead);
		}
		if (check == "Ring2")
		{
			fin >> check;
			idRead = stoi(check);
			player.e_ring2 = static_cast<itemID>(idRead);
		}
		if (check == "Necklace")
		{
			fin >> check;
			idRead = stoi(check);
			player.e_necklace = static_cast<itemID>(idRead);
		}
	}
	
	//Loading character's inventory
	int quantityRead;

	for (int i{ 0 }; i < player.inventorySize; i++)
	{
		fin >> idRead;
		fin >> quantityRead;
		
		player.inventory[i] = static_cast<itemID>(idRead);
		player.inventory_QTY[i] = quantityRead;
	}

	skillID sID;

	//Loading player's skills
	while (check != "[Shop]")
	{
		fin >> check;

		if (check == "[Shop]")
			break;

		if (check == "[Skills]")
			continue;
		
		idRead = stoi(check);
		sID = static_cast<skillID>(idRead);

		player.skills.push_back(sID);
	}

	itemID iID;

	//Loading shop inventory
	while (check != "[EOF]")
	{
		fin >> check;

		if (check == "[EOF]")
			break;

		if (check == "[Shop]")
			continue;

		idRead = stoi(check);
		iID = static_cast<itemID>(idRead);

		player.shop.push_back(iID);
	
		fin >> check;
		
		player.shopQTY.push_back(stoi(check));
	}

	fin.close();

	//Check if character is dead
	gameOver(player, saveSlot);

	waitS(1);
	cout << "\n						    Character Built. ";
	waitS(1);

}

void gameOver(Player& player, int saveSlot)
{
	if (player.get_status() == "Dead" || player.get_HP() <= 0)
	{
		system("cls");
		cout << "========================================================================================================================\n						        Game Over\n"
			"========================================================================================================================\n\n\n\n\n\n\n\n\n\n\n";

		string savePath = utility_getSavePath();

		if (saveSlot == 1)
		{
			filesystem::path deadSave = savePath + "\\save1.txt";
			filesystem::remove(deadSave);
		}
		else if (saveSlot == 2)
		{
			filesystem::path deadSave = savePath + "\\save2.txt";
			filesystem::remove(deadSave);
		}
		else if (saveSlot == 3)
		{
			filesystem::path deadSave = savePath + "\\save3.txt";
			filesystem::remove(deadSave);
		}

		cout << "\n						    Character is Dead ";
		waitS(1);
		cout << "\n						    Save Deleted. ";
		waitS(1);
		cout << "\n						    Game shutting down..\n\n\n\n\n\n";
		waitS(2);

		exit(0);
	}
}



	


