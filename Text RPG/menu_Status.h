#pragma once

#include "Player.h"
#include "item_ID.h"
#include "item_dataBase.h"


int manage_statusMenu(Player& player);
void print_statusMenuHeader();
int statusScreen(Player& player);
void getEquipItem(Player& player, baseItem& e_weapon, baseItem& e_shield, baseItem& e_head, baseItem& e_shoulders, baseItem& e_cape, baseItem& e_chest, baseItem& e_arms, baseItem& e_legs, baseItem& e_feet, baseItem& e_ring1, baseItem& e_ring2, baseItem& e_necklace);
void statInfo();