#pragma once
#include <vector>
#include <iostream>
#include <conio.h>

#include "Player.h"
#include "mob_dataBase.h"
#include "combat.h"
#include "utility.h"
#include "skill_formulas.h"
#include "menu_Inventory.h"
#include "menu_Skills.h"
#include "skill_Info.h"
#include "menu_LoadSave.h"
#include "useSkill.h"
#include "menu_shop.h"
#include "buffs.h"
using namespace std;




int manage_combat(Player& player, mapID mID, string actionChoice)
{
	vector <string> logs{}; //Vector that holds the last 15 lines stored in the battle log.

	baseMob* enemyMob{ nullptr }; //Contains enemy object

	int turnCount{ 0 }; //Turn counter

	bool playerAlive{ true };
	bool mobAlive{ true };

	if (actionChoice == "Train")
	{
		generateEnemy(player, enemyMob, mID, false); //Generate normal/elite enemies
		string log = "LV." + to_string(enemyMob->get_LVL()) + " " + enemyMob->get_name() + " has appeared! ";
		logs.push_back(log);
	}
	else //generate boss
	{
		generateEnemy(player, enemyMob, mID, true);
		string log = "LV." + to_string(enemyMob->get_LVL()) + " " + enemyMob->get_name() + " has appeared! ";
		logs.push_back(log);
	}

	//Combat Loop
	while (playerAlive == true && mobAlive == true)
	{ 
		//Allow non-boss mobs to attack first
		if (actionChoice == "Train" && turnCount == 0)
		{
			int roll = rand() % 100 + 1;

			if (roll <= 50)
			{
				updateLogs(player, enemyMob, logs);

				string log = enemyMob->get_name() + " takes " + player.get_name() + " by surprise!";
				logs.push_back(log);

				updateLogs(player, enemyMob, logs);

				enemyTurn(player, enemyMob, logs, turnCount);
				playerAlive = checkPlayerAlive(player);
				mobAlive = checkMobAlive(enemyMob);

				if (playerAlive == false)
				{
					int saveSlot = player.get_saveSlot();
					gameOver(player, saveSlot);
				}
				if (mobAlive == false)
					break;
			}
		}

		while (_kbhit()) //Discard any unwanted input while logs are updating
			_getch();

		playerTurn(player, enemyMob, logs, turnCount);
		playerAlive = checkPlayerAlive(player);
		mobAlive = checkMobAlive(enemyMob);

		if (playerAlive == false)
		{
			int saveSlot = player.get_saveSlot();
			gameOver(player, saveSlot);
		}
		if (mobAlive == false)
			break;
		

		//Update logs after player action
		updateLogs(player, enemyMob, logs);

		enemyTurn(player, enemyMob, logs, turnCount);
		playerAlive = checkPlayerAlive(player);
		mobAlive = checkMobAlive(enemyMob);

		if (playerAlive == false)
		{
			int saveSlot = player.get_saveSlot();
			gameOver(player, saveSlot);
		}
		if (mobAlive == false)
			break;

		turnCount++;
	}


	updateLogs(player, enemyMob, logs);

	string log = enemyMob->get_name() + " has been slain!";
	logs.push_back(log);

	updateLogs(player, enemyMob, logs);
	
	combatRewards(player, enemyMob, logs);

	//Increase fightCount if player fought in highest unlocked area. If not in highest area, still allow small chance to increase
	mapID highestMap = player.mapsUnlocked[player.get_mapsUnlockedSize() - 1];

	bool countIncrease{ false }; //Did fightCount increase yes or no. If yes, we roll to check if we update shop

	if (mID == highestMap)
	{
		player.fightCount++;
		countIncrease = true;
	}
	else //if player is currently training/fighting in an old map, dont guarantee fightCount++, therefore dont guarantee shop reroll
	{
		int roll = rand() % 100 + 1;

		if (roll <= 10) 
		{
			player.fightCount++;
			countIncrease = true;
		}
	}

	//Check if enemy was boss, if yes -> player unlocks next map
	if (enemyMob->get_mobType() == "Boss")
		unlockMap(player);

	//If fightCount is >= 10 and we just increased it, reroll shop
	if ((player.fightCount >= 10 && countIncrease == true) || enemyMob->get_mobType() == "Boss")
	{
			rerollShop(player);
			string log = "";
			logs.push_back(log);

			updateLogs(player, enemyMob, logs);
			log = "The shop has new wares for sale!";
			logs.push_back(log);

			updateLogs(player, enemyMob, logs);
			waitS(2);

			while (kbhit() == true)
				_getch();
	}

	delete enemyMob;
		
	bool hasAttack{false};

	for (skillID check : player.skills) //Checks if player lost attack skill (because of how combat skillMenu works)
		if (check == skillID::Attack)
			hasAttack = true;

	if (hasAttack == false)
		player.skills.push_back(skillID::Attack); //Put back Attack skill in player's skillInventory if absent

	//clear ALL player buffs/debuffs/skillCD at end of combat
	clearPlayerDebuffs(player); 
	clearPlayerBuffs(player); 
	clearSkillCooldowns(player);

	return 1;
}

void generateEnemy(Player& player, baseMob*& enemyMob, mapID mID, bool boss)
{
	int roll;
	int nextEnemyChance{ 60 }; //Chance for additional enemy to be generated
	bool result; //Generate new enemy or not

	switch(mID)
	{ 
		case mapID::Meadows: 
		{
			if (boss == false)
			{
				if (player.get_lvl() < 3) //If player is level 1-2, guarantee rats
				{
					enemyMob = new Rat;
					return;
				}

				roll = rand() % 100 + 1;

				if (roll <= 50)
					enemyMob = new Rat;
				else
					enemyMob = new Boar;
			}
			else
				enemyMob = new ElderTusk;
		}
		return;

		case mapID::DarkForest:
		{
			if (boss == false)
			{
				if (player.get_lvl() < 8) 
				{
					enemyMob = new Wolf;
					return;
				}

				roll = rand() % 100 + 1;

				if (roll <= 50)
					enemyMob = new Wolf;
				else
					enemyMob = new Highwayman;
			}
			else
				enemyMob = new HighwaymanCaptain;
		}
		return;

		case mapID::UndeadCatacombs:
		{
			if (boss == false)
			{
				if (player.get_lvl() < 8)
				{
					roll = rand() % 100 + 1;

					if (roll <= 50)
						enemyMob = new SkeletonWarrior;
					else
						enemyMob = new SkeletonArcher;
					return;
				}

				roll = rand() % 100 + 1;

				if (roll <= 50)
				{
					roll = rand() % 100 + 1;

					if (roll <= 50)
						enemyMob = new SkeletonWarrior;
					else
						enemyMob = new SkeletonArcher;
					return;
				}
				else
				{
					enemyMob = new Ghoul;
					return;
				}
			}
			else
				enemyMob = new TormentedSoul;
		}
		return;

		case mapID::SunkenPassage:
		{
			if (boss == false)
			{
				if (player.get_lvl() < 8)
				{
					enemyMob = new GiantTarantula;
					return;
				}
				else
				{
					roll = rand() % 100 + 1;

					if (roll <= 50)
						enemyMob = new GiantTarantula;
					else
						enemyMob = new MossySlime;

					return;
				}
			}
			else
				enemyMob = new OvergrownThornroot;
		}
		return;

		case mapID::DeadShore:
		{
			if (boss == false)
			{
				enemyMob = new DecayingVulture;
			}
			else
				enemyMob = new Wanderer;
		}
		return;
	}
}

int playerTurn(Player& player, baseMob*& enemyMob, vector<string>& logs, int& turnCount)
{
repeat:

	tickPlayerBuffs(player); 
	tickPlayerDebuffs(player, logs, enemyMob);
	tickSkillCooldowns(player);

	if (turnCount > 0)
		checkRegen(player, logs, enemyMob, "Player");

	int choice; //choice for battle menu options
	int rememberOption{ 0 }; //remember option for cursor in battle menu

	while(true) //Loop in case player backs out of a menu -> reprint battleLogs/battleMenu
	{ 
		system("cls");

		battleLogs(player, enemyMob, logs);

		while (_kbhit() == true)
			_getch();

		choice = battleMenu(player, enemyMob, rememberOption);

		if (choice == 0) //if player chose "Attack"
		{
			useSkill(player, enemyMob, skillID::Attack, logs);
			break;
		}
		else if (choice == 1) //if player chose "Skills"
		{
			bool usedSkill = combatSkills(player, logs, enemyMob);

			if (usedSkill == true) //if player used a skill, end turn
				break;
			else
				continue;
		}
		else if (choice == 2) //if player chose "Potions"
		{
			bool drink{ false }; 

			combatPotions(player, logs, drink);

			if (drink == true) //Did the player drink a potion? If yes, end turn
				break;
			else
				continue;
		}
		else if (choice == 3) //if player chose "Status"
		{
			combatStatus(player, enemyMob);
		}
	}

	if (enemyMob->stunned == true) //if enemy is stunned, let player go again
	{
		updateLogs(player, enemyMob, logs);

		string log = enemyMob->get_name() + " is stunned!";
		logs.push_back(log);

		updateLogs(player, enemyMob, logs);
		
		enemyMob->stunned = false;
		turnCount++;
		goto repeat;
	}

	return -1;
}


void battleLogs(Player& player, baseMob*& enemyMob, vector<string>& logs)
{
	system("cls");

	cout << "########################################################################################################################\n\n"
		"--- BATTLE LOGS ---\n\n";

	//Print battleLog indices
	for (string check : logs)
	{
		cout << check << endl;
	}

	//Determine how many lines we \n before printing battle menu. BattleLogs can only contain 15 lines
	int size = logs.size();

	for (int i{ size }; i < 15; i++)
		cout << endl;
}

int battleMenu(Player& player, baseMob*& enemyMob,int& rememberOption)
{
	cout << "\n\n--- BATTLE MENU ---\n\n";

	int cursorIndex{ rememberOption };

	while (true)
	{
		for (int i{ 0 }; i < 4; i++)
		{
			if (cursorIndex == i)
				cout << "> ";
			else
				cout << "  ";

			if (i == 0)
				cout << "Attack";
			if (i == 1)
				cout << "Skills";
			if (i == 2)
				cout << "Potions";
			if (i == 3)
				cout << "Status";

			cout << endl;
		}

		int keyPressed = _getch();

		while (keyPressed != 13 && keyPressed != 119 && keyPressed != 87 && keyPressed != 115 && keyPressed != 83)
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

			if (cursorIndex > 3)
				cursorIndex = 3;
		}
		
		if (keyPressed == 13) //Enter
		{
			rememberOption = cursorIndex;
			return cursorIndex;
		}

		utility_eraseLines(4);
	}
}

bool rollCrit(Player& player,int n)
{
	int roll = rand() % 100 + 1;

	if (roll <= player.get_critChance() + n)
		return true;
	else
		return false;
}

bool checkMiss(Player& player, baseMob*& enemyMob)
{
	int finalHitChance = player.get_hitChance() - enemyMob->get_dodgeChance();

	int roll = rand() % 100 + 1;

	if (roll <= finalHitChance)
		return true;
	else
		return false;
}

void sortLogs(vector<string>& logs)
{
	if (logs.size() >= 15)
	{
		for (int i{ 0 }; i < 14; i++)
		{
			logs[i] = logs[i + 1];
		}

		logs.resize(14); //resize to allow new log to be pushback at index 15
	}
	else
		return;
}

void checkThorns(Player& player, baseMob*& enemyMob, vector<string>& logs)
{
	if (enemyMob->get_thorns() > 0)
	{
		player.sub_HP(enemyMob->get_thorns());

		sortLogs(logs);
		string log = enemyMob->get_name() + " reflects " + to_string(enemyMob->get_thorns()) + " damage!";
		logs.push_back(log);
	}
}
	
void checkLifesteal(Player& player, baseMob*& enemyMob, vector<string>& logs, int& dmg)
{
	if (player.get_lifesteal() > 0)
	{
		double mult = player.get_lifesteal() / 100.0;
		int lifesteal = dmg * mult;

		if (lifesteal == 0)
			lifesteal = 1;

		if (lifesteal > enemyMob->get_maxHP())//cant lifesteal more than mob's maxHP
			lifesteal = enemyMob->get_maxHP();

		player.add_HP(lifesteal);

		if (player.get_HP() > player.get_maxHP()) //cant heal above maxHP
			player.set_HP(player.get_maxHP());

		sortLogs(logs);
		string log = player.get_name() + " restores " + typeColor("Health") + to_string(lifesteal) + " health" + clearColor() + "!";
		logs.push_back(log);
	}
}

void checkSpellvamp(Player& player, baseMob*& enemyMob, vector<string>& logs, int& dmg)
{
	if (player.get_spellVamp() > 0)
	{
		double mult = player.get_spellVamp() / 100.0;
		int spellVamp = dmg * mult;

		if (spellVamp == 0)
			spellVamp = 1;

		if (spellVamp > enemyMob->get_maxHP())
			spellVamp = enemyMob->get_maxHP();

		player.add_HP(spellVamp);

		if (player.get_HP() > player.get_maxHP())
			player.set_HP(player.get_maxHP());

		sortLogs(logs);
		string log = player.get_name() + " restores " + typeColor("Health") + to_string(spellVamp) + " health" + clearColor() + "!";
		logs.push_back(log);
	}
}
	
void checkOnHit(Player& player, baseMob*& enemyMob, vector<string>& logs, skillID skill, int& dmg)
{
	baseSkill skillUsed = skillDataBase[skill];

	if (skillUsed.source == skillSource::Physical && skillUsed.type == skillType::Damage)
	{
		checkThorns(player, enemyMob, logs);
		checkLifesteal(player, enemyMob, logs, dmg);
	}
	else if (skillUsed.source == skillSource::Magical && skillUsed.type == skillType::Damage)
		checkSpellvamp(player, enemyMob, logs, dmg);
}

void combatStatus(Player& player, baseMob*& enemyMob)
{
	system("cls");

	cout << "########################################################################################################################\n\n"
		"\n\n\n                                                      --- " << player.get_name() << " ---"
		"\n\n                                                      Level " << player.get_lvl() <<
		"\n                                                      XP " << player.get_currentEXP() << "/" << player.get_reqEXP() << endl <<

		"\n                                                      Health " << player.get_HP() << "/" << player.get_maxHP() <<
		"\n                                                      Mana " << player.get_MP() << "/" << player.get_maxMP() << endl;

	if (enemyMob->get_name() != "Highwayman Captain") //adapt output formatting to longer names
		cout << "\n\n\n\n                                                      --- " << enemyMob->get_name() << " ---";
	else
		cout << "\n\n\n\n                                                --- " << enemyMob->get_name() << " ---";

	cout <<	"\n\n                                                      Level " << enemyMob->get_LVL() <<
		    "\n                                                      Health " << enemyMob->get_HP() << "/" << enemyMob->get_maxHP();

	int keyPressed = _getch();

	while (keyPressed != 8)
		keyPressed = _getch();

	if (keyPressed == 8)
		return;
}

void combatPotions(Player& player, vector<string>& logs, bool& drink)
{
	
	vector<itemID> items{}; //to display potions
	vector<int> itemsIndex{}; //since only potions are displayed, need to remember what index they are in the actual inventory
	vector<int> itemsQTY{};

	for (int i{ 0 }; i < player.inventorySize; i++) //Get potions into items vector. and remember their position/qty in the player's inventory. (sorted by rarity)
	{
		itemID ID = player.inventory[i];
		baseItem item = itemDatabase[ID];

		if ((item.type == itemType::Potion || item.type == itemType::BuffPotion) && item.rarity == itemRarity::Legendary)
		{
			items.push_back(ID);
			itemsIndex.push_back(i);
			itemsQTY.push_back(player.inventory_QTY[i]);
		}
	}
	for (int i{ 0 }; i < player.inventorySize; i++) 
	{
		itemID ID = player.inventory[i];
		baseItem item = itemDatabase[ID];

		if ((item.type == itemType::Potion || item.type == itemType::BuffPotion) && item.rarity == itemRarity::Epic)
		{
			items.push_back(ID);
			itemsIndex.push_back(i);
			itemsQTY.push_back(player.inventory_QTY[i]);
		}
	}
	for (int i{ 0 }; i < player.inventorySize; i++)
	{
		itemID ID = player.inventory[i];
		baseItem item = itemDatabase[ID];

		if ((item.type == itemType::Potion || item.type == itemType::BuffPotion) && item.rarity == itemRarity::Rare)
		{
			items.push_back(ID);
			itemsIndex.push_back(i);
			itemsQTY.push_back(player.inventory_QTY[i]);
		}
	}
	for (int i{ 0 }; i < player.inventorySize; i++)
	{
		itemID ID = player.inventory[i];
		baseItem item = itemDatabase[ID];

		if ((item.type == itemType::Potion || item.type == itemType::BuffPotion) && item.rarity == itemRarity::Uncommon)
		{
			items.push_back(ID);
			itemsIndex.push_back(i);
			itemsQTY.push_back(player.inventory_QTY[i]);
		}
	}
	for (int i{ 0 }; i < player.inventorySize; i++)
	{
		itemID ID = player.inventory[i];
		baseItem item = itemDatabase[ID];

		if ((item.type == itemType::Potion || item.type == itemType::BuffPotion) && item.rarity == itemRarity::Common)
		{
			items.push_back(ID);
			itemsIndex.push_back(i);
			itemsQTY.push_back(player.inventory_QTY[i]);
		}
	}

	int cursorIndex{ 0 };

	while (true)
	{ 
		system("cls");
		cout << "########################################################################################################################\n\n"
		     << "\n                                                   --- Potions ---         - QTY -\n\n";

		for (int i{ 0 }; i < items.size(); i++)
		{
			cout << "                                                 ";

			if (cursorIndex == i)
				cout << "> ";
			else
				cout << "  ";

			itemID ID = items[i];
			baseItem item = itemDatabase[ID];

			cout << get_itemColor(item) << left << setw(25) << item.name << "  " << clearColor() << itemsQTY[i] << endl;
		}


		int keyPressed = _getch();

		while (keyPressed != 8 && keyPressed != 13 && keyPressed != 8 && keyPressed != 119 && keyPressed != 87 && keyPressed != 115 && keyPressed != 83)
			keyPressed = _getch();

		if (keyPressed == 8) //BS
			return;

		if (keyPressed == 119 || keyPressed == 87) //W
		{
			cursorIndex--;

			if (cursorIndex < 0)
				cursorIndex = 0;
		}

		if (keyPressed == 115 || keyPressed == 83) //S
		{
			cursorIndex++;

			if (cursorIndex > items.size() - 1)
				cursorIndex = items.size() - 1;
		}

		if (keyPressed == 13) //Enter
		{
			int atIndex = itemsIndex[cursorIndex]; //Stores the real inventory's index of selected potion

			int option = usePotion(player, items, itemsQTY, atIndex, cursorIndex, logs, drink);

			if (option == -1)
				continue;
			else
				break;
		}

		utility_eraseLines(items.size());
	}
}

int usePotion(Player& player, vector<itemID>& items, vector<int>& itemsQTY, int atIndex, int cursorIndex, vector<string>& logs, bool& drink)
{
	system("cls");
	cout << "########################################################################################################################\n\n\n\n\n\n";

	itemID ID = items[cursorIndex];
	int qty = itemsQTY[cursorIndex];

	baseItem item = itemDatabase[ID];
	consumable_stats consumable;
	consumable_buffs buffPot;

	if (item.type != itemType::BuffPotion)
		consumable = get<consumable_stats>(item.stats);
	else
		buffPot = get<consumable_buffs>(item.stats);

	cout << left
		<< "\n\n                                        " << get_itemColor(item) << "[" << item.name << "]" << clearColor()
		<< "\n                                        " << item.description << endl;

	if (item.type != itemType::BuffPotion)
	{ 
		if (consumable.type == "Health")
			cout << "\n                                        " << "Restores " << typeColor("Health") << showpos << consumable.powerValue << noshowpos << " HP" << clearColor();
		else if (consumable.type == "Mana")
			cout << "\n                                        " << "Restores " << typeColor("Health") << showpos << consumable.powerValue << noshowpos << " MP" << clearColor();

		cout << "\n                                        " << "Quantity: " << qty;

		cout << "\n\n                                        " << "> Drink";
	}
	else
	{
		consumable_buffs buffPot = get<consumable_buffs>(item.stats);
		buff buffs = buffPot.buffs;

		cout << "\n\n                                        " << "Quantity: " << qty;
		cout << "\n\n                                        Effect lasts for " << buffPot.turns-1 << " turns.";
		cout << "\n\n                                        " << "Buffs: ";

		print_buffs(buffs);

		cout << "\n\n                                        " << "> Drink";
	}
	

	int keyPressed = _getch();

	while (keyPressed != 13 && keyPressed != 8)
		keyPressed = _getch();

	if (keyPressed == 8)
		return -1;

	if (keyPressed == 13)
	{
		drink = true;

		if (item.type != itemType::BuffPotion)
		{
			if (consumable.type == "Health")
			{
				player.add_HP(consumable.powerValue);

				if (player.get_HP() > player.get_maxHP())
					player.set_HP(player.get_maxHP());

				itemsQTY[cursorIndex]--;
				player.inventory_QTY[atIndex]--;

				if (itemsQTY[cursorIndex] == 0 || player.inventory_QTY[atIndex] == 0)
				{
					items[cursorIndex] = itemID::None;
					player.inventory[atIndex] = itemID::None;
				}

				sortLogs(logs);
				string log = player.get_name() + " drank a " + get_itemColor(item) + item.name + clearColor() + " and restored " + typeColor("Health") + to_string(consumable.powerValue) + " health" + clearColor() + "!";
				logs.push_back(log);
			}
			else if (consumable.type == "Mana")
			{
				player.add_MP(consumable.powerValue);

				if (player.get_MP() > player.get_maxMP())
					player.set_MP(player.get_maxMP());

				itemsQTY[cursorIndex]--;
				player.inventory_QTY[atIndex]--;

				if (itemsQTY[cursorIndex] == 0 || player.inventory_QTY[atIndex] == 0)
				{
					items[cursorIndex] = itemID::None;
					player.inventory[atIndex] = itemID::None;
				}

				sortLogs(logs);
				string log = player.get_name() + " drank a " + get_itemColor(item) + item.name + clearColor() + " and restored " + typeColor("Mana") + to_string(consumable.powerValue) + " mana" + clearColor() + "!";
				logs.push_back(log);
			}
		}
		else
		{
			itemsQTY[cursorIndex]--;
			player.inventory_QTY[atIndex]--;

			if (itemsQTY[cursorIndex] == 0 || player.inventory_QTY[atIndex] == 0)
			{
				items[cursorIndex] = itemID::None;
				player.inventory[atIndex] = itemID::None;
			}

			applyBuffPot(player, buffPot);

			sortLogs(logs);
			string log = player.get_name() + " drank a " + get_itemColor(item) + item.name + clearColor() + "!";
			logs.push_back(log);
		}

		return 1;
	}
}

bool combatSkills(Player& player, vector<string>& logs, baseMob*& enemyMob)
{

	sortSkills(player, true);
	
	int cursorIndex{ 0 };

	while (true)
	{ 
		system("cls");
		cout << "########################################################################################################################\n\n\n";

		for (int i{ 0 }; i < player.skills.size(); i++)
		{
			skillID ID = player.skills[i];
			baseSkill skill = skillDataBase[ID];

			cout << "     ";

			if (i == cursorIndex)
				cout << "> ";
			else
				cout << "  ";
			
			cout << get_skillColor(skill) << left << setw(16) << skill.name << clearColor();

			if ((i + 1) % 5 == 0)
			{
				cout << endl;
			}
		}

		int keyPressed = _getch();

		while (keyPressed != 119 && keyPressed != 87 && keyPressed != 115 && keyPressed != 83 && keyPressed != 68 && keyPressed != 100 && keyPressed != 65 && keyPressed != 97 && keyPressed != 70 && keyPressed != 102 && keyPressed != 8 && keyPressed != 13)
			keyPressed = _getch();


		if (keyPressed == 68 || keyPressed == 100) //D
		{
			cursorIndex++;

			if (cursorIndex >= player.skills.size() - 1)
				cursorIndex = player.skills.size() - 1;
		}

		if (keyPressed == 65 || keyPressed == 97) //A
		{
			cursorIndex--;

			if (cursorIndex < 0)
				cursorIndex = 0;
		}

		if (keyPressed == 119 || keyPressed == 87) //W
		{
			cursorIndex -= 5;

			if (cursorIndex < 0)
				cursorIndex = 0;
		}

		if (keyPressed == 115 || keyPressed == 83) //S
		{
			cursorIndex += 5;

			if (cursorIndex >= player.skills.size() - 1)
				cursorIndex = player.skills.size() - 1;
		}

		if (keyPressed == 8) //BS
		{
			return false;
		}

		if (keyPressed == 13) //ENTER
		{
			skillID ID = player.skills[cursorIndex];
			bool use =  combatSkillInfo(player, ID);

			if (use == false) //if player backs out
				continue;
			else				   //if player chooses to use the skill
			{
				//Check if player has enough MP or HP to cast skill
				baseSkill skill = skillDataBase[ID];

				bool cost = checkCost(player, skill);
				bool cooldown = checkCooldown(player, ID);


				if (cooldown == false) //skill is NOT on cooldown
				{
					if (cost == true)
					{
						//push skill into cooldown timer
						if (skill.cd != 0)
						{
							player.CDskill.push_back(ID);
							player.CDskillTimer.push_back(skill.cd);
						}

						useSkill(player, enemyMob, ID, logs);
						return true;
					}
					else
					{
						system("cls");
						cout << "########################################################################################################################\n\n\n\n\n\n\n\n"
							<< "\n\n                                      You do not have enough MP/HP to use this skill.";
						cout << "\n\n                                                   > [I Understand]";

						int keyPressed = _getch();

						while (keyPressed != 8 && keyPressed != 13)
							keyPressed = _getch();

						return false;
					}
				}
				else //skill IS on cooldown
				{
					int turnsLeft{ 0 };

					for (int i{ 0 }; i < player.CDskill.size(); i++)
					{
						if (player.CDskill[i] == ID)
						{
							turnsLeft = player.CDskillTimer[i];
							break;
						}
					}

					system("cls");
					cout << "########################################################################################################################\n\n\n\n\n\n\n\n"
						<< "\n\n                                      " << get_skillColor(skill) << skill.name << clearColor() << " is currently on cooldown! [";

					if (turnsLeft == 99)
						cout << "Rest of combat]";
					else if (turnsLeft == 1)
						cout << turnsLeft << " turn]";
					else
						cout << turnsLeft << " turns]";

					cout << "\n\n                                                     > [I Understand]";

					int keyPressed = _getch();

					while (keyPressed != 8 && keyPressed != 13)
						keyPressed = _getch();

					return false;
				}

			}
		}
	}
}

bool combatSkillInfo(Player& player, skillID ID)
{
	system("cls");
	cout << "########################################################################################################################\n\n\n\n\n\n";

	baseSkill skill = skillDataBase[ID];

	string type;

	if (skill.source == skillSource::Physical)
		type = "Physical";
	else if (skill.source == skillSource::Magical)
		type = "Magical";

	cout << "                                     " << get_skillColor(skill) << "[" << skill.name << "]" << clearColor();
	cout << "\n                                     " << skill.description;

	if (skill.cost.mpCost > 0 || skill.cost.hpCost > 0) //Print skill's cost.
	{
		cout << "\n\n                                     " << "Costs ";

		if (skill.cost.mpCost > 0)
			cout << "\033[38;5;117m" << skill.cost.mpCost << " Mana  " << clearColor();
		if (skill.cost.hpCost > 0)
			cout << "\x1b[32m" << skill.cost.hpCost << " Health  " << clearColor();
	}

	cout << "\n\n                                     "; displaySkillCooldown(ID);
	skillInfo_Desc(player, ID);

	cout << "\n\n\n\n                                     > Use Skill";

	int keyPressed = _getch();

	while (keyPressed != 8 && keyPressed != 13)
		keyPressed = _getch();

	if (keyPressed == 8)
		return false;

	if (keyPressed == 13)
		return true;
}

bool checkCost(Player& player, baseSkill skill)
{
	skillCost cost = skill.cost;

	if (cost.mpCost > 0 && cost.hpCost == 0) //if skill only costs MP
	{
		if (player.get_MP() >= cost.mpCost)
		{
			return true;
		}
		else
			return false;
	}
	else if (cost.mpCost == 0 && cost.hpCost > 0) //if skill only costs HP
	{
		if (player.get_HP() > cost.hpCost)
		{
			return true;
		}
		else
			return false;
	}
	else if (cost.mpCost > 0 && cost.hpCost > 0) //if skill costs MP and HP
	{
		if (player.get_MP() >= cost.mpCost && player.get_HP() > cost.hpCost)
		{
			return true;
		}
		else
			return false;
	}
	else if (cost.mpCost == 0) //if skill is free
		return true;
}

bool checkPlayerAlive(Player& player)
{
	if (player.get_HP() <= 0)
		return false; //player is not alive
	else
		return true; //player is alive
}

bool checkMobAlive(baseMob*& enemyMob)
{
	if (enemyMob->get_HP() <= 0)
		return false; //mob is not alive
	else
		return true; //mob is alive
}

void enemyTurn(Player& player, baseMob*& enemyMob, vector<string>& logs, int& turnCount)
{
	while (true)
	{
		if (turnCount > 1)
			checkRegen(player, logs, enemyMob, "Mob");

		enemyMob->takeTurn(player, logs, turnCount);

		if (player.stunned == true)
		{
			updateLogs(player, enemyMob, logs);

			string log = player.get_name() + " is stunned!";
			logs.push_back(log);

			updateLogs(player, enemyMob, logs);

			turnCount++;
			player.stunned = false;
			tickPlayerBuffs(player); //tickdown player buffs here cuz stunned = skipped turn
			tickPlayerDebuffs(player, logs, enemyMob);
			tickSkillCooldowns(player);
		}
		else
			break;
	}
}

void updateLogs(Player& player, baseMob*& enemyMob, vector<string>& logs)
{
	sortLogs(logs);
	battleLogs(player, enemyMob, logs);

	cout << "\n\n--- BATTLE MENU ---\n\n";

	for (int i{ 0 }; i < 4; i++)
	{
		cout << "  ";

		if (i == 0)
			cout << "Attack";
		if (i == 1)
			cout << "Skills";
		if (i == 2)
			cout << "Potions";
		if (i == 3)
			cout << "Status";

		cout << endl;
	}

	waitS(1);
}

void combatRewards(Player& player, baseMob*& enemyMob, vector<string>& logs)
{
	string log = ""; //Create empty space in battle log
	logs.push_back(log);
	updateLogs(player, enemyMob, logs);

	//Give player XP reward + print log
	int xp = enemyMob->get_XP();
	player.add_currentXP(xp);

	log = player.get_name() + " receives " + to_string(xp) + " XP!";
	logs.push_back(log);
	updateLogs(player, enemyMob, logs);

	//Print money reward log(s)
	if (enemyMob->get_gold() > 0)
	{
		player.add_gold(enemyMob->get_gold());

		string log = player.get_name() + " receives " + typeColor("Gold") + to_string(enemyMob->get_gold()) + " gold" + clearColor() + "!";
		logs.push_back(log);
		updateLogs(player, enemyMob, logs);
	}
	if (enemyMob->get_silver() > 0)
	{
		player.add_silver(enemyMob->get_silver());

		string log = player.get_name() + " receives " + typeColor("Silver") + to_string(enemyMob->get_silver()) + " silver" + clearColor() + "!";
		logs.push_back(log);
		updateLogs(player, enemyMob, logs);
	}
	if (enemyMob->get_copper() > 0)
	{
		player.add_copper(enemyMob->get_copper());

		string log = player.get_name() + " receives " + typeColor("Copper") + to_string(enemyMob->get_copper()) + " copper" + clearColor() + "!";
		logs.push_back(log);
		updateLogs(player, enemyMob, logs);
	}

	recalculateCoins(player);

	//Check if player gets mobLoot + print log
	int threshold = enemyMob->get_lootChance();
	int roll = rand() % 100 + 1;


	if (roll <= threshold)
	{
		//Get item from mob
		itemID ID = enemyMob->get_loot();
		baseItem item = itemDatabase[ID];

		int atIndex;
		bool hasItem{ false };

		//Check if player already has item
		for (int i{ 0 }; i < player.inventorySize; i++)
		{
			if (player.inventory[i] == ID && player.inventory_QTY[i] < 99)
			{
				atIndex = i;
				hasItem = true;
			}
		}

		if (hasItem == true) //If player already has a stack of the loot item, and it's not at max capacity (99), add it to the stack.
		{
			//Give item to player
			player.inventory[atIndex] = ID;
			player.inventory_QTY[atIndex]++;

			log = player.get_name() + " receives " + get_itemColor(item) + item.name + clearColor() + "!";
			logs.push_back(log);
			updateLogs(player, enemyMob, logs);

			return;
		}


		bool full{ true }; //is inventory full

		for (int i{ 0 }; i < player.inventorySize; i++)
		{
			if (player.inventory[i] == itemID::None)
			{
				atIndex = i;
				full = false;
			}
		}

		if (full == true) //If inventory is full, discard item.
		{
			log = player.get_name() + "'s inventory is full! " + get_itemColor(item) + item.name + clearColor() + " is discarded.";
			logs.push_back(log);
			updateLogs(player, enemyMob, logs);
		}
		else //If inventory is not full, place item atIndex
		{
			player.inventory[atIndex] = ID;
			player.inventory_QTY[atIndex]++;

			log = player.get_name() + " receives " + get_itemColor(item) + item.name + clearColor() + "!";
			logs.push_back(log);
			updateLogs(player, enemyMob, logs);
		}
	}

	//Check for levelUp
	if (player.get_currentEXP() >= player.get_reqEXP())
	{
		levelUp(player);
		
		
		string log = player.get_name() + " reaches level " + to_string(player.get_lvl()) + "!";
		logs.push_back(log);
		updateLogs(player, enemyMob, logs);

		int cursorIndex{ 0 };

		while (_kbhit())
			_getch();



		while (true) //Display level up choice (hp/mp/atk/sp)
		{
			system("cls");
			cout << "########################################################################################################################\n\n\n\n\n\n";
			cout << "                                             --- Choose a Level Up reward ---\n\n\n";

			for (int i{ 0 }; i < 4; i++)
			{
				cout << "                                               ";

				if (cursorIndex == i)
					cout << "> ";
				else
					cout << "  ";

				if (i == 0)
					cout << "Increase " << typeColor("Health") << " Health by 5\n\n" << clearColor();
				if (i == 1)
					cout << "Increase " << typeColor("Mana") << " Mana by 3\n\n" << clearColor();
				if (i == 2)
					cout << "Increase " << typeColor("Physical") << " Attack Damage by 1\n\n" << clearColor();
				if (i == 3)
					cout << "Increase " << typeColor("Magical") << " Spell Power by 1\n\n" << clearColor();
			}

			int keyPressed = _getch();

			while (keyPressed != 13 && keyPressed != 119 && keyPressed != 87 && keyPressed != 115 && keyPressed != 83)
				keyPressed = _getch();

			if (keyPressed == 119 || keyPressed == 87)
			{
				cursorIndex--;

				if (cursorIndex < 0)
					cursorIndex = 0;
			}
			if (keyPressed == 115 || keyPressed == 82)
			{
				cursorIndex++;

				if (cursorIndex == 4)
					cursorIndex = 3;
			}
			if (keyPressed == 13)
			{
				if (cursorIndex == 0)
				{
					player.add_maxHP(5);
					player.add_HP(5);
				}
				if (cursorIndex == 1)
				{
					player.add_maxMP(3);
					player.add_MP(3);
				}
				if (cursorIndex == 2)
					player.add_atk(1);
				if (cursorIndex == 3)
					player.add_spellPower(1);

				break;
			}
		}
	}
	else
	{
		waitS(2);
		while (_kbhit())
			_getch();
	}
}

void levelUp(Player& player)
{
	int topOff{0};

	if (player.get_currentEXP() > player.get_reqEXP())
		topOff = player.get_currentEXP() - player.get_reqEXP(); //extra exp above expReq. To add on next level
	
	player.add_lvl(1);
	player.set_requiredXP((15 * player.get_lvl()) + player.get_reqEXP()); //add 15*lvl to current reqEXP
	player.set_currentXP(topOff);

}

void checkRegen(Player& player, vector<string>& logs, baseMob*& enemyMob, string s)
{
	string log;

	updateLogs(player, enemyMob, logs); //print logs before apply regen for display purposes

	if (s == "Player")
	{
		if (player.get_hpRegen() > 0 && player.get_mpRegen() > 0) //if player has both hp and mp regen, print regen on same log
		{
			player.add_HP(player.get_hpRegen());

			if (player.get_HP() > player.get_maxHP())
				player.set_HP(player.get_maxHP());

			player.add_MP(player.get_mpRegen());

			if (player.get_MP() > player.get_maxMP())
				player.set_MP(player.get_maxMP());

			updateLogs(player, enemyMob, logs);

			log = player.get_name() + " regenerates " + typeColor("Health") + to_string(player.get_hpRegen()) + " health" + clearColor() + " and " 
				  + typeColor("Mana") + to_string(player.get_mpRegen()) + " mana" + clearColor() + "!";
			logs.push_back(log);

			updateLogs(player, enemyMob, logs);

			return;
		}

		if (player.get_hpRegen() > 0)
		{
			player.add_HP(player.get_hpRegen());

			if (player.get_HP() > player.get_maxHP())
				player.set_HP(player.get_maxHP());

			updateLogs(player, enemyMob, logs);

			log = player.get_name() + " regenerates " + typeColor("Health") + to_string(player.get_hpRegen()) + " health" + clearColor() + "!";
			logs.push_back(log);

			updateLogs(player, enemyMob, logs);
		}

		if (player.get_mpRegen() > 0)
		{
			player.add_MP(player.get_mpRegen());

			if (player.get_MP() > player.get_maxMP())
				player.set_MP(player.get_maxMP());

			updateLogs(player, enemyMob, logs);

			log = player.get_name() + " regenerates " + typeColor("Mana") + to_string(player.get_mpRegen()) + " mana" + clearColor() + "!";
			logs.push_back(log);

			updateLogs(player, enemyMob, logs);
		}
	}
	else if (s == "Mob")
	{
		if (enemyMob->get_hpRegen() > 0)
		{
			enemyMob->add_HP(enemyMob->get_hpRegen());

			if (enemyMob->get_HP() > enemyMob->get_maxHP())
				enemyMob->set_HP(enemyMob->get_maxHP());

			updateLogs(player, enemyMob, logs);

			log = enemyMob->get_name() + " regenerates " + typeColor("Health") + to_string(enemyMob->get_hpRegen()) + " health" + clearColor() + "!";
			logs.push_back(log);

			updateLogs(player, enemyMob, logs);
		}
	}

	
}

void unlockMap(Player& player)
{
	mapID highestMap = player.mapsUnlocked[player.get_mapsUnlockedSize() - 1];

	switch (highestMap)
	{
		case::mapID::Meadows:
		{
			player.mapsUnlocked.push_back(mapID::DarkForest);
			return;
		}
		case::mapID::DarkForest:
		{
			player.mapsUnlocked.push_back(mapID::UndeadCatacombs);
			return;
		}
		case::mapID::UndeadCatacombs:
		{
			player.mapsUnlocked.push_back(mapID::SunkenPassage);
			return;
		}
		case::mapID::SunkenPassage:
		{
			player.mapsUnlocked.push_back(mapID::DeadShore);
			return;
		}

	}

}

void applyBuffPot(Player& player, consumable_buffs b)
{
	buff buffs = b.buffs;

	if (buffs.maxHP > 0)
	{
		player.add_maxHP(buffs.maxHP);
		player.pbuffs.push_back(buffs);
		player.pbuffsTimer.push_back(b.turns);
	}
	if (buffs.maxMP > 0)
	{
		player.add_maxMP(buffs.maxMP);
		player.pbuffs.push_back(buffs);
		player.pbuffsTimer.push_back(b.turns);
	}
	if (buffs.hpRegen > 0)
	{
		player.add_hpRegen(buffs.hpRegen);
		player.pbuffs.push_back(buffs);
		player.pbuffsTimer.push_back(b.turns);
	}
	if (buffs.mpRegen > 0)
	{
		player.add_mpRegen(buffs.mpRegen);
		player.pbuffs.push_back(buffs);
		player.pbuffsTimer.push_back(b.turns);
	}
	if (buffs.atk > 0)
	{
		player.add_atk(buffs.atk);
		player.pbuffs.push_back(buffs);
		player.pbuffsTimer.push_back(b.turns);
	}
	if (buffs.spellPower > 0)
	{
		player.add_spellPower(buffs.spellPower);
		player.pbuffs.push_back(buffs);
		player.pbuffsTimer.push_back(b.turns);
	}
	if (buffs.defense > 0)
	{
		player.add_def(buffs.defense);
		player.pbuffs.push_back(buffs);
		player.pbuffsTimer.push_back(b.turns);
	}
	if (buffs.magicResist > 0)
	{
		player.add_magicResist(buffs.magicResist);
		player.pbuffs.push_back(buffs);
		player.pbuffsTimer.push_back(b.turns);
	}
	if (buffs.hitChance > 0)
	{
		player.add_hitChance(buffs.hitChance);
		player.pbuffs.push_back(buffs);
		player.pbuffsTimer.push_back(b.turns);
	}
	if (buffs.dodgeChance > 0)
	{
		player.add_dodgeChance(buffs.dodgeChance);
		player.pbuffs.push_back(buffs);
		player.pbuffsTimer.push_back(b.turns);
	}
	if (buffs.critChance > 0)
	{
		player.add_critChance(buffs.critChance);
		player.pbuffs.push_back(buffs);
		player.pbuffsTimer.push_back(b.turns);
	}
	if (buffs.critMult > 0)
	{
		player.add_critMult(buffs.critMult);
		player.pbuffs.push_back(buffs);
		player.pbuffsTimer.push_back(b.turns);
	}
	if (buffs.lifesteal > 0)
	{
		player.add_lifesteal(buffs.lifesteal);
		player.pbuffs.push_back(buffs);
		player.pbuffsTimer.push_back(b.turns);
	}
	if (buffs.spellVamp > 0)
	{
		player.add_spellVamp(buffs.spellVamp);
		player.pbuffs.push_back(buffs);
		player.pbuffsTimer.push_back(b.turns);
	}
	if (buffs.thorns > 0)
	{
		player.add_thorns(buffs.thorns);
		player.pbuffs.push_back(buffs);
		player.pbuffsTimer.push_back(b.turns);
	}
}

void clearPlayerBuffs(Player& player)
{
	//lower player stats for each remaining buffs
	for (int i{ 0 }; i < player.pbuffs.size(); i++)
	{
		buff b = player.pbuffs[i];

		player.sub_maxHP(b.maxHP);
		player.sub_maxMP(b.maxMP);

		player.sub_HP(b.maxHP);

		if (player.get_HP() > player.get_maxHP()) //if removing hp buff results in currentHP being over maxHP
			player.set_HP(player.get_maxHP());

		player.sub_MP(b.maxMP);

		if (player.get_MP() > player.get_maxMP())
			player.set_MP(player.get_maxMP());

		player.sub_hpRegen(b.hpRegen);
		player.sub_mpRegen(b.mpRegen);
		player.sub_atk(b.atk);
		player.sub_spellPower(b.spellPower);
		player.sub_def(b.defense);
		player.sub_magicResist(b.magicResist);
		player.sub_hitChance(b.hitChance);
		player.sub_dodgeChance(b.dodgeChance);
		player.sub_critChance(b.critChance);
		player.sub_critMult(b.critMult);
		player.sub_lifesteal(b.lifesteal);
		player.sub_spellVamp(b.spellVamp);
		player.sub_thorns(b.thorns);
	}

	//empty buff vectors when done
	player.pbuffs.clear();
	player.pbuffsTimer.clear();
}

void tickPlayerBuffs(Player& player)
{
	for (int i{ 0 }; i < player.pbuffs.size(); i++)
	{
		if (player.pbuffsTimer[i] != 99) //buffs with 99 counter is meant to be infinite
			player.pbuffsTimer[i]--;

		if (player.pbuffsTimer[i] <= 0)
		{
			buff b = player.pbuffs[i];

			//Remove buff's effect from player
			player.sub_maxHP(b.maxHP);
			player.sub_maxMP(b.maxMP);

			player.sub_HP(b.maxHP);

			if (player.get_HP() > player.get_maxHP()) 
				player.set_HP(player.get_maxHP());

			player.sub_MP(b.maxMP);

			if (player.get_MP() > player.get_maxMP())
				player.set_MP(player.get_maxMP());

			player.sub_hpRegen(b.hpRegen);
			player.sub_mpRegen(b.mpRegen);
			player.sub_atk(b.atk);
			player.sub_spellPower(b.spellPower);
			player.sub_def(b.defense);
			player.sub_magicResist(b.magicResist);
			player.sub_hitChance(b.hitChance);
			player.sub_dodgeChance(b.dodgeChance);
			player.sub_critChance(b.critChance);
			player.sub_critMult(b.critMult);
			player.sub_lifesteal(b.lifesteal);
			player.sub_spellVamp(b.spellVamp);
			player.sub_thorns(b.thorns);


			//Erase buff from vector
			player.pbuffs.erase(player.pbuffs.begin() + i);
			player.pbuffsTimer.erase(player.pbuffsTimer.begin() + i);
		}
	}
}

void clearPlayerDebuffs(Player& player)
{
	for (int i{ 0 }; i < player.pdebuffs.size(); i++)
	{
		debuff d = player.pdebuffs[i];

		player.add_atk(d.weak);
		player.add_spellPower(d.nullify);
		player.add_def(d.frail);
		player.add_magicResist(d.shatter);
		player.add_hitChance(d.blind);
		player.add_dodgeChance(d.immobile);
	}

	player.pdebuffs.clear();
	player.pdebuffsTimer.clear();
}

void tickPlayerDebuffs(Player& player, vector<string>& logs, baseMob*& enemyMob)
{
	for (int i{ 0 }; i < player.pdebuffs.size(); i++)
	{
		debuff d = player.pdebuffs[i];

		if (player.pdebuffsTimer[i] > 0 && player.pdebuffsTimer[i] != 99) //debuffs with 99 counter is meant to be infinite
		{
			string log;

			if (d.bleed > 0)
			{
				player.sub_HP(d.bleed);

				updateLogs(player, enemyMob, logs);

				log = player.get_name() + " takes " + typeColor("Bleed") + to_string(d.bleed) + " bleed damage" + clearColor() + "!";
				logs.push_back(log);
			}
			if (d.poison > 0)
			{
				player.sub_HP(d.poison);

				updateLogs(player, enemyMob, logs);

				log = player.get_name() + " takes " + typeColor("Poison") + to_string(d.poison) + " poison damage" + clearColor() + "!";
				logs.push_back(log);
			}
			if (d.drain > 0)
			{
				player.sub_HP(d.drain);

				updateLogs(player, enemyMob, logs);
	
				log = player.get_name() + " loses " + typeColor("Mana") + to_string(d.drain) + " mana" + clearColor() + "!";
				logs.push_back(log);
			}
		}
		else
		{
			player.add_atk(d.weak);
			player.add_spellPower(d.nullify);
			player.add_def(d.frail);
			player.add_magicResist(d.shatter);
			player.add_hitChance(d.blind);
			player.add_dodgeChance(d.immobile);

			player.pdebuffs.erase(player.pdebuffs.begin() + i);
			player.pdebuffsTimer.erase(player.pdebuffsTimer.begin() + i);
			break;
		}

		player.pdebuffsTimer[i]--;
	}
}

void applyBuffSkill(Player& player, skillBuff sBuff)
{
	buff b = sBuff.buffs;

	if (b.maxHP > 0)
	{
		player.add_maxHP(b.maxHP);
		player.pbuffs.push_back(b);
		player.pbuffsTimer.push_back(sBuff.turns);
	}
	if (b.maxMP > 0)
	{
		player.add_maxMP(b.maxMP);
		player.pbuffs.push_back(b);
		player.pbuffsTimer.push_back(sBuff.turns);
	}
	if (b.hpRegen > 0)
	{
		player.add_hpRegen(b.hpRegen);
		player.pbuffs.push_back(b);
		player.pbuffsTimer.push_back(sBuff.turns);
	}
	if (b.mpRegen > 0)
	{
		player.add_mpRegen(b.mpRegen);
		player.pbuffs.push_back(b);
		player.pbuffsTimer.push_back(sBuff.turns);
	}
	if (b.atk > 0)
	{
		player.add_atk(b.atk);
		player.pbuffs.push_back(b);
		player.pbuffsTimer.push_back(sBuff.turns);
	}
	if (b.spellPower > 0)
	{
		player.add_spellPower(b.spellPower);
		player.pbuffs.push_back(b);
		player.pbuffsTimer.push_back(sBuff.turns);
	}
	if (b.defense > 0)
	{
		player.add_def(b.defense);
		player.pbuffs.push_back(b);
		player.pbuffsTimer.push_back(sBuff.turns);
	}
	if (b.magicResist > 0)
	{
		player.add_magicResist(b.magicResist);
		player.pbuffs.push_back(b);
		player.pbuffsTimer.push_back(sBuff.turns);
	}
	if (b.hitChance > 0)
	{
		player.add_hitChance(b.hitChance);
		player.pbuffs.push_back(b);
		player.pbuffsTimer.push_back(sBuff.turns);
	}
	if (b.dodgeChance > 0)
	{
		player.add_dodgeChance(b.dodgeChance);
		player.pbuffs.push_back(b);
		player.pbuffsTimer.push_back(sBuff.turns);
	}
	if (b.critChance > 0)
	{
		player.add_critChance(b.critChance);
		player.pbuffs.push_back(b);
		player.pbuffsTimer.push_back(sBuff.turns);
	}
	if (b.critMult > 0)
	{
		player.add_critMult(b.critMult);
		player.pbuffs.push_back(b);
		player.pbuffsTimer.push_back(sBuff.turns);
	}
	if (b.lifesteal > 0)
	{
		player.add_lifesteal(b.lifesteal);
		player.pbuffs.push_back(b);
		player.pbuffsTimer.push_back(sBuff.turns);
	}
	if (b.spellVamp > 0)
	{
		player.add_spellVamp(b.spellVamp);
		player.pbuffs.push_back(b);
		player.pbuffsTimer.push_back(sBuff.turns);
	}
	if (b.thorns > 0)
	{
		player.add_thorns(b.thorns);
		player.pbuffs.push_back(b);
		player.pbuffsTimer.push_back(sBuff.turns);
	}

}
                      
bool checkCooldown(Player& player, skillID& ID)
{
	bool onCD{ false }; //is skill currently on cooldown

	for (skillID check : player.CDskill)
		if (check == ID)
			onCD = true;

	return onCD;
}

void clearSkillCooldowns(Player& player)
{
	player.CDskill.clear();
	player.CDskillTimer.clear();
}

void tickSkillCooldowns(Player& player)
{
	for (int i{ 0 }; i < player.CDskill.size(); i++)
	{
		if (player.CDskillTimer[i] != 99)
			player.CDskillTimer[i]--;

		if (player.CDskillTimer[i] <= 0)
		{
			player.CDskill.erase(player.CDskill.begin() + i);
			player.CDskillTimer.erase(player.CDskillTimer.begin() + i);
		}
	}
}