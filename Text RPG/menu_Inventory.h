#pragma once

#include <vector>
#include <iostream>
#include <conio.h>
#include <iomanip>

#include "item_ID.h"
#include "Utility.h"
#include "item_dataBase.h"
#include "Player.h"
#include "buffs.h"
using namespace std;


int manage_inventoryMenu(Player& player);
void print_inventoryMenuHeader(Player player);

int inventoryMenu(Player& player); //rememberItem = The last item that was selected, so that cursor is on it instead of resetting to item 1.
void sortInventory(Player& player); //Sort player inventory by item rarity

int displayItemList(Player& player, int rememberItem); //returns selected item or BS
int itemInfoMenu(Player& player, int selectedItem);
void print_itemEnchants(enchantments e);
int usePotFood(Player& player, int selectedItem, consumable_stats consumable);

void displayValue(baseItem& item, bool all, int& qty); //Displays item value in copper/silver/gold
void print_buffs(buff b);