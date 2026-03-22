#pragma once

#include "Player.h"
#include "skill_ID.h"
#include "skill_dataBase.h"

int manage_skillMenu(Player& player);
int skillMenu(Player& player, int& skillSelected);
int skillInfo(Player& player, int skillSelected);
void sortSkills(Player& player, bool inCombat); //Sorts skills in order of rarity
