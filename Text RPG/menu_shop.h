#pragma once
#include "Player.h"

int manage_shop(Player& player);
int promptOptions(int& remember); //prompts player if they want to buy or sell items
void rerollShop(Player& player); //refreshes the shop inventory with new items depending on player level (only happens when player fought at least 10 times in the highest unlocked area, count resets to 0 after refresh)
void pushItem(itemID& ID, Player& player); //Rolls if item gets pushed into shop inventory, and determines how many (qty)
int pushItemQTY(baseItem& item); //If item has been rolled, checks how many to push depending on itemType

void buyItem(Player& player);
void sellItem(Player& player);
int displayInventory(Player& player, int& remember);
void giveCoins(Player& player, int& value); //Converts raw value to give player the according amount of copper/silver/gold coins

void displayBuyValue(baseItem& item);
int buyItemInfo(Player& player, baseItem& item, vector<int>& shopQTY, int& atIndex); //Displays info for the selected item to buy. 
bool priceCheck(Player& player, baseItem& item); //Checks if player has enough money to afford item or not
void removeCoins(Player& player, baseItem& item); //Removes item from player

void sortShop(Player& player); //Sorts the shop inventory by highest to lowest item price
void recalculateCoins(Player& player); //recalculates total player money and redistributes coins accordingly
bool checkSkillBook(Player& player, baseItem& item);

void deleteEmptySlots(Player& player, int& currentPage, int& maxPage); //used to remove the empty shop entries created by sortShop (cheap fix but works)