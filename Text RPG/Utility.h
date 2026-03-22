#pragma once
#pragma warning (disable:4996)
#pragma warning (disable:6031)

#include <thread>
#include <chrono>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <filesystem>
#include <conio.h>
#include <string>

#include "menu_LoadSave.h"
#include "Player.h"
#include "item_ID.h"
#include "item_dataBase.h"
#include "skill_ID.h"
#include "skill_dataBase.h"
#include "map_ID.h"
#include "map_dataBase.h"
using namespace std;
//This header file contains general purpose functions, to be used in multiple cpp files.

//Macro to make program wait
#define waitS(x) this_thread :: sleep_for(chrono::seconds(x)) 
#define waitMS(x) this_thread :: sleep_for(chrono::milliseconds(x)) 
#define waitNS(x) this_thread :: sleep_for(chrono::nanoseconds(x)) 

//Sets RNG Seed
inline void setRNGSeed()
{
	srand(time(NULL));
}

//Type letters instead of instant output
inline void Type(string x) 
{
	for (char print : x)
	{
		cout << print;
		waitNS(500);
	}
}

//Prevents user from entering input while program is in sleep mode or displaying info
inline void utility_clearGetchBuffer()
{
	while (_kbhit() == true)
		_getch();
}


//Erases x amount of lines, from bottom up.
inline void utility_eraseLines(int x) 
{
	for (int i{ 1 }; i <= x; i++)
	{
		cout << "\x1b[2K"; //Erase line
		cout << "\x1b[1A"; //Go up one line
		cout << "\r"; //Makes cursor go to start of line
	}
}


//Returns -> C:\Users\USERNAME\Documents\HardcoreRPG\Saves
inline string utility_getSavePath() 
{
	string user;
	string p;

	user = getenv("USERPROFILE");

	p = user + "\\Documents\\HardcoreRPG\\Saves";

	return p;
}


//Saves game
inline void utility_saveGame(Player& player, int saveSlot, bool initializeShop)
{
	if (initializeShop == false)
	{ 
		system("cls");
		cout << "========================================================================================================================\n						     Currently Saving...\n"
			"========================================================================================================================\n\n\n\n\n\n\n\n\n\n\n\n\n";
	}

	ofstream fou;
	string savePath = utility_getSavePath();

	if (saveSlot == 1)
		fou.open(savePath + "\\save1.txt");
	else if (saveSlot == 2)
		fou.open(savePath + "\\save2.txt");
	else if (saveSlot == 3)
		fou.open(savePath + "\\save3.txt");

	//Writes Player stats
	fou << "[Player Info]\n\n"
		<< "SaveSlot " << player.get_saveSlot() << "\nName " << player.get_name() << "\nStatus " << player.get_status() << "\nLevel " << player.get_lvl() << "\nRequiredXP "
		<< player.get_reqEXP() << "\nCurrentXP " << player.get_currentEXP() << "\nMaxHP " << player.get_maxHP() << "\nCurrentHP "
		<< player.get_HP() << "\nMaxMP " << player.get_maxMP() << "\nCurrentMP " << player.get_MP() << "\nhpRegen " << player.get_hpRegen() << "\nmpRegen " << player.get_mpRegen()
		<< "\nAttack " << player.get_atk() << "\nSpellPower " << player.get_spellPower() << "\nDefense " << player.get_defense() << "\nMagicResist " << player.get_magicResist()
		<< "\nLifeSteal " << player.get_lifesteal() << "\nSpellVamp " << player.get_spellVamp() << "\nCritChance " << player.get_critChance() << "\nCritMult "
		<< player.get_critMult() << "\nHitChance " << player.get_hitChance() << "\nDodgeChance " << player.get_dodgeChance() << "\nThorns " << player.get_thorns()
		<< "\nCopperCoins " << player.get_copper() << "\nSilverCoins " << player.get_silver() << "\nGoldCoins " << player.get_gold() << "\nFightCount " << player.fightCount;
		
	//Writes Unlocked Maps
	fou << "\n\n[Maps Unlocked]\n\n";

	for (int i{0}; i < player.get_mapsUnlockedSize(); i++)
	{
		mapID ID = player.mapsUnlocked[i];
		
		if (ID == mapID::Meadows)
			fou << "Meadows" << endl;
		else if (ID == mapID::DarkForest)
			fou << "DarkForest" << endl;
		else if (ID == mapID::UndeadCatacombs)
			fou << "UndeadCatacombs" << endl;
		else if (ID == mapID::SunkenPassage)
			fou << "SunkenPassage" << endl;
		else if (ID == mapID::DeadShore)
			fou << "DeadShore" << endl;
	
	}

	fou << "\n\n[Equipement]\n\n"
		<< "Weapon " << static_cast<int>(player.e_weapon) << "\nShield " << static_cast<int>(player.e_shield) << "\nHead " << static_cast<int>(player.e_head) << "\nShoulders "
		<< static_cast<int>(player.e_shoulders) << "\nCape " << static_cast<int>(player.e_cape) << "\nChest " << static_cast<int>(player.e_chest) << "\nArms " << static_cast<int>(player.e_arms) 
		<< "\nLegs " << static_cast<int>(player.e_legs) << "\nFeet " << static_cast<int>(player.e_feet) << "\nRing1 " << static_cast<int>(player.e_ring1) << "\nRing2 " << static_cast<int>(player.e_ring2) 
		<< "\nNecklace " << static_cast<int>(player.e_necklace);

	fou << "\n\n[Inventory]\n\n";

	for (int i{ 0 }; i < player.inventorySize; i++)
	{
		fou << static_cast<int>(player.inventory[i]) << " " << static_cast<int>(player.inventory_QTY[i]) << endl;
	}

	//Writes unlocked physical skills
	fou << "\n\n[Skills]\n\n";

	for (int i{ 0 }; i < player.skills.size(); i++)
	{
		fou << static_cast<int>(player.skills[i]) << endl;
	}

	fou << "\n\n[Shop]\n\n";

	for (int i{ 0 }; i < player.shop.size(); i++)
	{
		fou << static_cast<int>(player.shop[i]) << " " << static_cast<int>(player.shopQTY[i]) << endl;
	}
	for (int i{ 0 }; i < player.shop2.size(); i++)
	{
		fou << static_cast<int>(player.shop2[i]) << " " << static_cast<int>(player.shopQTY2[i]) << endl;
	}
	for (int i{ 0 }; i < player.shop3.size(); i++)
	{
		fou << static_cast<int>(player.shop3[i]) << " " << static_cast<int>(player.shopQTY3[i]) << endl;
	}

	fou << "\n\n[EOF]\n\n";
	if (initializeShop == false)
	{ 
		waitS(2);

		cout << "\n					              Save Succesful!";
	
		waitS(2);
	}

	fou.close();

	while (_kbhit() == true)
		_getch();
}

inline string get_itemColor(baseItem item)
{
	if (item.rarity == itemRarity::Legendary)
		return "\x1b[33m";
	else if (item.rarity == itemRarity::Epic)
		return "\x1b[35m";
	else if (item.rarity == itemRarity::Rare)
		return "\x1b[34m";
	else if (item.rarity == itemRarity::Uncommon)
		return "\x1b[32m";
	else if (item.rarity == itemRarity::Common)
		return "";
	else if (item.rarity == itemRarity::None)
		return "";
}

inline string clearColor()
{
	return "\x1b[37m";
}

inline string itemStatColor(int stat) //enables either green or red depending if item stat is + or -
{
	if (stat > 0)
		return "\x1b[0;32m";
	else if (stat < 0)
		return "\x1b[0;31m";
	else if (stat == 0)
		return "\x1b[0;37m";
}

inline string get_skillColor(baseSkill skill)
{
	if (skill.rarity == skillRarity::Legendary)
		return "\x1b[33m";
	else if (skill.rarity == skillRarity::Epic)
		return "\x1b[35m";
	else if (skill.rarity == skillRarity::Rare)
		return "\x1b[34m";
	else if (skill.rarity == skillRarity::Uncommon)
		return "\x1b[32m";
	else if (skill.rarity == skillRarity::Common)
		return "";
	else if (skill.rarity == skillRarity::None)
		return "";
}

inline string typeColor(string s)
{
	if (s == "Physical")
		return "\033[38;5;209m";
	else if (s == "Magical")
		return "\033[38;5;141m";
	else if (s == "Critical")
		return "\033[38;5;196m";
	else if (s == "Health")
		return "\x1b[32m";
	else if (s == "Mana")
		return "\033[38;5;117m";
	else if (s == "Copper")
		return "\033[38;5;94m";
	else if (s == "Silver")
		return "\033[38;5;245m";
	else if (s == "Gold")
		return "\x1b[33m";
	else if (s == "Stun")
		return "\033[38;5;18m";
	else if (s == "Bleed")
		return "\033[38;5;203m";
	else if (s == "Poison")
		return "\033[38;5;53m";
	else if (s == "Thorns")
		return "\033[38;5;94m";
}
