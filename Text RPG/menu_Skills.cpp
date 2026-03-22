#include <iostream>
#include <conio.h>
#include <vector>

#include "skill_ID.h"
#include "skill_dataBase.h"
#include "menu_skills.h"
#include "skill_Info.h"
#include "Player.h"
#include "Utility.h"
using namespace std;


int manage_skillMenu(Player& player)
{
	int action{ 0 };
	int skillSelected{ 0 }; 

	sortSkills(player, false);

	while (action != -1)
	{
		action = skillMenu(player, skillSelected);
		
		if (action == 1)
		{
			skillInfo(player, skillSelected);
		}
		
	}


	return 0;
}

//Player can press F to switch between physical/magical/non scaling skills
//Player can navigate between spells with cursorIndex and press enter to see skillsInfo

int skillMenu(Player& player, int& skillSelected)
{
	int cursorIndex{ skillSelected };
	system("cls");
	cout << "========================================================================================================================\n						     Learned Skills\n"
		"========================================================================================================================\n\n\n\n";

	while (true)
	{ 
		int linesToErase{ 1 }; 
		

		for (int i{ 0 }; i < player.skills.size(); i++)
		{
			cout << "     ";
	
			if (i == cursorIndex)
				cout << "> ";
			else
				cout << "  ";

			skillID ID = player.skills[i];
			baseSkill skill = skillDataBase[ID];

			cout << get_skillColor(skill) << left << setw(16) << skill.name << clearColor();

			if ((i + 1) % 5 == 0)
			{
				cout << endl;
				linesToErase++; //Increment each time a new line is drawn
			}
		}

		int keyPressed = _getch();
	
		while (keyPressed != 119 && keyPressed != 87 && keyPressed != 115 && keyPressed != 83 && keyPressed != 68 && keyPressed != 100 && keyPressed != 65 && keyPressed != 97 && keyPressed != 70 &&keyPressed != 102 && keyPressed != 8 && keyPressed != 13)
			keyPressed = _getch();


		if (keyPressed == 68 || keyPressed == 100) //D
		{
			cursorIndex++;

			if (cursorIndex == player.skills.size())
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

			if (cursorIndex >= player.skills.size())
				cursorIndex = player.skills.size() - 1;
		}

		if (keyPressed == 13) //ENTER
		{
			skillSelected = cursorIndex;
			return 1;
		}

		if (keyPressed == 8) //BS
		{
			return -1;
		}


		system("cls");
		cout << "========================================================================================================================\n						     Learned Skills\n"
			"========================================================================================================================\n\n\n\n";
	}
}

int skillInfo(Player& player, int skillSelected)
{
	system("cls");
	cout << "========================================================================================================================\n						      Skill Info\n"
		"========================================================================================================================\n\n\n\n\n\n";

	skillID ID = player.skills[skillSelected];
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
			cout << "\033[38;5;117m" << skill.cost.mpCost << " Mana" << clearColor() << ".";
		if (skill.cost.hpCost > 0)
			cout << "\x1b[32m" << skill.cost.hpCost << " Health" << clearColor() << ".";
	}

	cout << "\n\n                                     "; displaySkillCooldown(ID);
	skillInfo_Desc(player, ID);


	int keyPressed = _getch();

	while (keyPressed != 8)
		keyPressed = _getch();

	if (keyPressed == 8)
		return -1;
}

void sortSkills(Player& player, bool inCombat)
{
	vector<skillID> legendary;
	vector<skillID> epic;
	vector<skillID> rare;
	vector<skillID> uncommon;
	vector<skillID> common;

	for (int i{ 0 }; i < player.skills.size(); i++)
	{
		skillID ID = player.skills[i];
		baseSkill skill = skillDataBase[ID];

		if (inCombat == true)
		{
			if (ID == skillID::Attack)
				continue;
		}

		if (skill.rarity == skillRarity::Common)
		{
			common.push_back(ID);
			continue;
		}
		else if (skill.rarity == skillRarity::Uncommon)
		{
			uncommon.push_back(ID);
			continue;
		}
		else if (skill.rarity == skillRarity::Rare)
		{
			rare.push_back(ID);
			continue;
		}
		else if (skill.rarity == skillRarity::Epic)
		{
			epic.push_back(ID);
			continue;
		}
		else if (skill.rarity == skillRarity::Legendary)
		{
			legendary.push_back(ID);
			continue;
		}
	}

	player.skills.clear();

	for (skillID check : legendary)
		player.skills.push_back(check);
	for (skillID check : epic)
		player.skills.push_back(check);
	for (skillID check : rare)
		player.skills.push_back(check);
	for (skillID check : uncommon)
		player.skills.push_back(check);
	for (skillID check : common)
		player.skills.push_back(check);
}