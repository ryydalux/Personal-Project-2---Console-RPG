#pragma once

#include "Player.h"
#include "item_dataBase.h"

int manage_equipementMenu(Player& player);
void print_equipementMenuheader();
itemType display_equipementSlots(Player& player, int& rememberSlotChoice, int& ringSlot);
void get_equipList(Player& player, itemType typeSelected, vector<itemID>& equipList, vector<int>& itemPosition);
int print_equipList(Player& player, itemType typeSelected, vector<itemID>& equipList);
bool checkTwoHand(Player& player); //Checks if player is currently using a two-handed weapon
void unequipItem(Player& player, itemType typeSelected, int ringSlot);
void unequip_ItemEnchants(Player& player, enchantments e);
void equipItem(Player& player, itemType typeSelected, vector<int>& itemPosition, int ringSlot, int equipSelected);
void equip_ItemEnchants(Player& player, enchantments e);
bool checkEmptySlot(Player& player, itemType& typeSelected, int& ringSlot); //checks if x slot is currently empty or not

void replaceEquip(Player& player, vector<itemID>& equipList, vector<int>& itemPosition, int& equipSelected, int& ringSlot); //replaces currently equipped item with the selected one from inventory


