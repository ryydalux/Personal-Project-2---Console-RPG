#include <iostream>
#include <conio.h>
#include <iomanip>
#include <variant>
#include <optional>

#include "menu_Character.h"
#include "menu_Inventory.h"
#include "item_ID.h"
#include "item_dataBase.h"
#include "Player.h"
#include "Utility.h"
#include "buffs.h"
using namespace std;


int manage_inventoryMenu(Player& player) 
{
	int inventory_option{0}; //Option can represent -1 for backspace, or any of the selected items by the player
	
	while (inventory_option != -1)
	{ 
		inventory_option = inventoryMenu(player);
	}

	return inventory_option;
}

void print_inventoryMenuHeader(Player player)
{
	cout << "========================================================================================================================\n						       Inventory\n"
		"========================================================================================================================\n\n\n\n\n\n";
}

int inventoryMenu(Player& player)
{

	int option{ 0 };
	int rememberItem{ 0 };

	while (option != -1)
	{ 
		sortInventory(player); //Sort player inventory by item rarity
		option = displayItemList(player, rememberItem); //returns selected item or BS

		if (option == -1)
			return option;
		else
			rememberItem = itemInfoMenu(player, option);
	}
}

int displayItemList(Player& player, int rememberItem) //returns selected item or BS
{
	int cursorIndex{ rememberItem + 1 };
	int selectedItem{ 0 };

	system("cls");
	print_inventoryMenuHeader(player);

	cout << "                         Item Name                    QTY |   Item name                    QTY\n                                                          |\n";

	while (true)
	{
		for (int i{ 1 }; i < player.inventorySize + 1; i++)
		{
			itemID ID = player.inventory[i - 1];
			int qty = player.inventory_QTY[i - 1];

			baseItem item = itemDatabase[ID];

			cout << "                       ";

			if (i == cursorIndex)
			{
				cout << "> ";
			}
			else
				cout << "  ";

			cout << left << get_itemColor(item) << setw(30) << item.name << clearColor();

			if (qty >= 1 && item.stackable == true)
				cout << setw(2) << qty;
			else
				cout << "  ";

			cout << " | ";
			i++;

			ID = player.inventory[i - 1];
			qty = player.inventory_QTY[i - 1];

			item = itemDatabase[ID];

			if (i == cursorIndex)
			{
				cout << "> ";
			}
			else
				cout << "  ";

			cout << left << get_itemColor(item) << setw(30) << item.name << clearColor();

			if (qty >= 1 && item.stackable == true)
				cout << setw(2) << qty;
			else
				cout << "  ";

			cout << endl;
		}

		int keyPressed = _getch(); //Store key pressed by user (W|S|ENTER)

		while (keyPressed != 119 && keyPressed != 87 && keyPressed != 115 && keyPressed != 83 && keyPressed != 68 && keyPressed != 100 && keyPressed != 65 && keyPressed != 97 && keyPressed != 8 && keyPressed != 13)
		{
			keyPressed = _getch();
		}

		if (keyPressed == 119 || keyPressed == 87) //W
		{
			cursorIndex -= 2;
			utility_eraseLines(player.inventorySize / 2);

			if (cursorIndex < 1)
			{
				cursorIndex += 2;
			}
		}

		if (keyPressed == 115 || keyPressed == 83) //S
		{
			cursorIndex += 2;
			utility_eraseLines(player.inventorySize / 2);

			if (cursorIndex > player.inventorySize)
			{
				cursorIndex -= 2;
			}
		}

		if (keyPressed == 68 || keyPressed == 100) //D
		{
			cursorIndex += 1;
			utility_eraseLines(player.inventorySize / 2);

			if (cursorIndex > player.inventorySize)
			{
				cursorIndex = player.inventorySize;
			}
		}

		if (keyPressed == 65 || keyPressed == 97) //A
		{
			cursorIndex -= 1;
			utility_eraseLines(player.inventorySize / 2);


			if (cursorIndex < 1)
			{
				cursorIndex = 1;
			}
		}

		if (keyPressed == 13) //ENTER
		{
			selectedItem = cursorIndex - 1;
			return selectedItem;
		}

		if (keyPressed == 8) //BS
		{
			selectedItem = -1;
			return selectedItem;
		}

	}
}

int itemInfoMenu(Player& player, int selectedItem) //Let player perform action on item selected. (equip or use consumable)
{
	int keyPressed;

	itemID ID = player.inventory[selectedItem];
	
	if (ID == itemID::None) //If user selected an empty inventory slot = do nothing
	{
		return selectedItem;
	}

	int qty = player.inventory_QTY[selectedItem];
	baseItem item = itemDatabase[ID];

	system("cls");
	print_inventoryMenuHeader(player);

	if (item.type == itemType::Weapon)
	{
		weapon_stats wpn = get<weapon_stats>(item.stats); //weapon_stats : atk, spellPower, hitChance, critChance, twohanded

		cout << left
			<< "\n\n                                        " << get_itemColor(item) << "[" << item.name << "]" << clearColor()
			<< "\n                                        " << item.description << endl;
		if (wpn.atk != 0)
			cout << "\n                                        " << "Attack Damage: " << itemStatColor(wpn.atk) << wpn.atk  << clearColor();
		if (wpn.spellPower != 0)
			cout << "\n                                        " << "Spell Power: " << itemStatColor(wpn.spellPower) << wpn.spellPower << clearColor();
		if (wpn.hitChance != 0)
			cout << "\n                                        " << "Hit Chance: " << itemStatColor(wpn.hitChance) << wpn.hitChance << clearColor();
		if (wpn.critChance != 0)
			cout << "\n                                        " << "Critical Chance: " << itemStatColor(wpn.critChance) << wpn.critChance << clearColor();

			cout << "\n\n                                        " << "Weight: "; if (wpn.twoHanded == true) { cout << "Two-Handed"; } else { cout << "One-Handed"; }
			cout << "\n\n                                        Sell Value: "; displayValue(item, false, qty);
	  
	   if (item.enchants) //If enchant found on item
	   {
		   cout << "\n\n                                        " << "Enchantments:";
		   enchantments e = *item.enchants;
		   print_itemEnchants(e);
	   }
	}
	
	else if (item.type == itemType::Shield || item.type == itemType::Head || item.type == itemType::Shoulders || item.type == itemType::Chest || item.type == itemType::Arms || item.type == itemType::Legs || item.type == itemType::Feet)
	{
		armor_stats armor = get<armor_stats>(item.stats); //weapon_stats : atk, spellPower, hitChance, critChance, twohanded

		cout << left
			<< "\n\n                                        " << get_itemColor(item) << "[" << item.name << "]" << clearColor()
			<< "\n                                        " << item.description << endl;
		if (armor.def != 0)
			cout << "\n                                        " << "Defense: " << itemStatColor(armor.def) << armor.def << clearColor();
		if (armor.magicResist != 0)
			cout << "\n                                        " << "Magic Resist: " << itemStatColor(armor.magicResist) << armor.magicResist << clearColor();
		if (armor.dodgeChance != 0)
			cout << "\n                                        " << "Dodge Chance: " << itemStatColor(armor.dodgeChance) << armor.dodgeChance << clearColor();;

			cout << "\n\n                                        " << "Weight: " << armor.weight;
			cout << "\n\n                                        Sell Value: "; displayValue(item, false, qty);

		if (item.enchants) //If enchant found on item
		{
			cout << "\n\n                                        " << "Enchantments:";
			enchantments e = *item.enchants;
			print_itemEnchants(e);
		}
	}
	else if (item.type == itemType::Ring || item.type == itemType::Necklace || item.type == itemType::Cape)
	{
		cout << left
			<< "\n\n                                        " << get_itemColor(item) << "[" << item.name << "]" << clearColor()
			<< "\n                                        " << item.description;
		cout << "\n\n                                        Sell Value: "; displayValue(item, false, qty);

		if (item.enchants) //If enchant found on item
		{
			cout << "\n\n                                        " << "Enchantments:";
			enchantments e = *item.enchants;
			print_itemEnchants(e);
		}
	}
	else if (item.type == itemType::Potion || item.type == itemType::Food)
	{
		consumable_stats consumable = get<consumable_stats>(item.stats); 

		cout << left
			<< "\n\n                                        " << get_itemColor(item) << "[" << item.name << "]" << clearColor()
			<< "\n                                        " << item.description << endl;

		if (consumable.type == "Health")
			cout << "\n                                        " << "Restores " << "\x1b[0;32m" << showpos << consumable.powerValue << noshowpos << " HP" << clearColor();
		else if (consumable.type == "Mana")
			cout << "\n                                        " << "Restores " << "\x1b[0;32m" << showpos << consumable.powerValue << noshowpos << " MP" << clearColor();
		else if (consumable.type == "HealthMana")
			cout << "\n                                        " << "Restores " << "\x1b[0;32m" << showpos << consumable.powerValue << noshowpos << " HP" << clearColor() << " and " << "\x1b[0;32m" << showpos << consumable.powerValue << noshowpos << " MP" << clearColor();

		cout << "\n                                        " << "Quantity: " << qty;
		cout << "\n\n                                        Sell Value: "; displayValue(item, false, qty);

		cout << "\n\n                                        " << "> Use Item";

		int back{0}; //used to go back to inventory when play is done using the consumable

		while (back != -1)
		{ 
			keyPressed = _getch();

			while (keyPressed != 8 && keyPressed != 13)
			{
				keyPressed = _getch();
			}

			if (keyPressed == 8) //BS
			{
				keyPressed = -1;
				return selectedItem;
			}

			if (keyPressed == 13) //ENTER
			{
				back = usePotFood(player, selectedItem, consumable); //Allows the ues of potion or food item until quantity reaches 0

				if (back == -1)
					return selectedItem;
			}
		}
		

		keyPressed = _getch();

		while (keyPressed != 8 && keyPressed != 13)
		{
			keyPressed = _getch();
		}

		if (keyPressed == 13)
		{

		}

		if (keyPressed == 8) //BS
		{
			return selectedItem;
		}

	}
	else if (item.type == itemType::BuffPotion)
	{
		consumable_buffs buffPot = get<consumable_buffs>(item.stats); 
		buff buffs = buffPot.buffs;

		cout << left
			<< "                                        " << get_itemColor(item) << "[" << item.name << "]" << clearColor()
			<< "\n                                        " << item.description;
		
		cout << "\n\n                                        " << "Quantity: " << qty;
		cout << "\n\n                                        Effect lasts for " << buffPot.turns-1 << " turns.";
		cout << "\n\n                                        Sell Value: "; displayValue(item, false, qty);
		cout << "\n\n                                        " << "Buffs: ";
	

		print_buffs(buffs);
	
	}
	else if (item.type == itemType::Loot)
	{
		cout << left
			 << "\n\n                                        " << get_itemColor(item) << "[" << item.name << "]" << clearColor()
			 << "\n                                        " << item.description << endl;

		cout << "\n                                        Sell Value: "; displayValue(item, false, qty);
	}
	else if (item.type == itemType::SkillBook)
	{
		cout << left
			<< "\n\n                                        " << get_itemColor(item) << "[" << item.name << "]" << clearColor()
			<< "\n                                        " << item.description << endl;

		cout << "\n                                        Sell Value: "; displayValue(item, false, qty);

		cout << "\n\n                                        " << "> Learn Skill";

		int keyPressed = _getch();

		while (keyPressed != 8 && keyPressed != 13)
			keyPressed = _getch();

		if (keyPressed == 8)
			return selectedItem;

		if (keyPressed == 13) //consume skillbook to add skillID to player.skills
		{
			skillID ID = *item.skill;
			player.skills.push_back(ID);

			player.inventory[selectedItem] = itemID::None;
			player.inventory_QTY[selectedItem] = 0;

			return selectedItem;
		}
	}
	
	keyPressed = _getch();

	while (keyPressed != 8)
	{
		keyPressed = _getch();
	}


	if (keyPressed == 8) //BS
	{
		return selectedItem;
	}
}

void print_itemEnchants(enchantments e)
{
	cout << typeColor("Health");
	if (e.maxHP != 0)
		cout << " +" << e.maxHP << " HP " << endl << "                                                     ";
	
	if (e.maxMP != 0)
		cout << " +" << e.maxMP << " MP " << endl << "                                                     ";
	
	if (e.hpRegen != 0)
		cout << " +" << e.hpRegen << " HP Regeneration " << endl << "                                                     ";
	
	if (e.mpRegen != 0)
		cout << " +" << e.mpRegen << " MP Regeneration " << endl << "                                                     ";
	
	if (e.atk != 0)
		cout << " +" << e.atk << " Attack Power " << endl << "                                                     ";
	if (e.spellPower != 0)
		cout << " +" << e.spellPower << " Spell Power " << endl << "                                                     ";
	if (e.defense != 0)
		cout << " +" << e.defense << " Defense " << endl << "                                                     ";
	if (e.magicResist != 0)
		cout << " +" << e.magicResist << " Magic Resist " << endl << "                                                     ";
	if (e.hitChance != 0)
		cout << " +" << e.hitChance << "% Hit Chance " << endl << "                                                     ";
	if (e.dodgeChance != 0)
		cout << " +" << e.dodgeChance << "% Dodge Chance " << endl << "                                                     ";
	if (e.critChance != 0)
		cout << " +" << e.critChance << "% Critical Chance " << endl << "                                                     ";
	if (e.critMult != 0)
		cout << " +" << e.critMult << "x Critical Multiplier " << endl << "                                                     ";
	if (e.lifesteal != 0)
		cout << " +" << e.lifesteal << "% Life Steal " << endl << "                                                     ";
	if (e.spellVamp != 0)
		cout << " +" << e.spellVamp << "% Spell Vamp " << endl << "                                                     ";
	if (e.thorns != 0)
		cout << " +" << e.thorns << " Thorns " << endl << "                                                     ";

	cout << clearColor();
}

int usePotFood(Player& player, int selectedItem, consumable_stats consumable)
{
	int keyPressed{0};
	itemID ID = player.inventory[selectedItem];
	baseItem item = itemDatabase[ID];

	if (consumable.type == "Health") 
	{
		for (int quantityLeft{ player.inventory_QTY[selectedItem]}; quantityLeft > 0;)
		{
			player.add_HP(consumable.powerValue);
			player.inventory_QTY[selectedItem] -= 1;
			quantityLeft--;

			if (player.get_HP() >= player.get_maxHP()) //Cant go past maxHP
				player.set_HP(player.get_maxHP());

			if (quantityLeft == 0) //If qty reaches 0, delete item from inventory and back out
			{
				player.inventory[selectedItem] = itemID::None;
				keyPressed = -1;
				break;
			}

			utility_eraseLines(5);

			cout << "\n                                        " << "Quantity: " << player.inventory_QTY[selectedItem];
			cout << "\n\n                                        Sell Value: "; displayValue(item, false, player.inventory_QTY[selectedItem]);

			cout << "\n\n                                        " << "> Use Item";

			keyPressed = _getch();

			while (keyPressed != 8 && keyPressed != 13)
				keyPressed = _getch();

			if (keyPressed == 13) //Enter = use item again
				continue;

			if (keyPressed == 8) //BS = go back to inventory
			{
				keyPressed = -1;
				return keyPressed;
			}
		}
	}
	else if (consumable.type == "Mana")
	{
		for (int quantityLeft{ player.inventory_QTY[selectedItem] }; quantityLeft > 0;)
		{
			player.add_MP(consumable.powerValue);
			player.inventory_QTY[selectedItem] -= 1;
			quantityLeft--;
			
			if (player.get_MP() >= player.get_maxMP()) //Cant go past maxMP
				player.set_MP(player.get_maxMP());

			if (quantityLeft == 0) //If qty reaches 0, delete item from inventory and back out
			{
				player.inventory[selectedItem] = itemID::None;
				keyPressed = -1;
				break;
			}

			utility_eraseLines(5);

			cout << "\n                                        " << "Quantity: " << player.inventory_QTY[selectedItem];
			cout << "\n\n                                        Sell Value: "; displayValue(item, false, player.inventory_QTY[selectedItem]);

			cout << "\n\n                                        " << "> Use Item";

			keyPressed = _getch();

			while (keyPressed != 8 && keyPressed != 13)
				keyPressed = _getch();

			if (keyPressed == 13) //Enter = use item again
				continue;

			if (keyPressed == 8) //BS = go back to inventory
			{
				keyPressed = -1;
				return keyPressed;
			}
		}
	}
	else if (consumable.type == "HealthMana")
	{
		for (int quantityLeft{ player.inventory_QTY[selectedItem] }; quantityLeft > 0;)
		{
			player.add_HP(consumable.powerValue);
			player.add_MP(consumable.powerValue);
			player.inventory_QTY[selectedItem] -= 1;
			quantityLeft--;

			if (player.get_HP() >= player.get_maxHP()) //Cant go past maxHP
				player.set_HP(player.get_maxHP());

			if (player.get_MP() >= player.get_maxMP()) //Cant go past maxMP
				player.set_MP(player.get_maxMP());

			if (quantityLeft == 0) //If qty reaches 0, delete item from inventory and back out
			{
				player.inventory[selectedItem] = itemID::None;
				keyPressed = -1;
				break;
			}

			utility_eraseLines(5);

			cout << "\n                                        " << "Quantity: " << player.inventory_QTY[selectedItem];
			cout << "\n\n                                        Sell Value: "; displayValue(item, false, player.inventory_QTY[selectedItem]);

			cout << "\n\n                                        " << "> Use Item";

			keyPressed = _getch();

			while (keyPressed != 8 && keyPressed != 13)
				keyPressed = _getch();

			if (keyPressed == 13) //Enter = use item again
				continue;

			if (keyPressed == 8) //BS = go back to inventory
			{
				keyPressed = -1;
				return keyPressed;
			}
		}
	}


	return keyPressed;
}

void sortInventory(Player& player)
{

	int itemAmount{ 0 }; //How many total items does player have? used for while loop condition below

	for (int i{ 0 }; i < player.inventorySize; i++)
	{
		if (player.inventory[i] == itemID::None)
			continue;
		else
			itemAmount++;
	}

	//temporary containers to hold items by rarity and store their quantity
	vector<itemID> sort_legendary;
	vector<int> sort_legendaryQTY;
	vector<itemID> sort_epic;
	vector<int> sort_epicQTY;
	vector<itemID> sort_rare;
	vector<int> sort_rareQTY;
	vector<itemID> sort_uncommon;
	vector<int> sort_uncommonQTY;
	vector<itemID> sort_common;
	vector<int> sort_commonQTY;

	//send items to respective rarity containers
	for (int i{ 0 }; i < player.inventorySize; i++)
	{
		itemID ID = player.inventory[i]; //get key (itemID)
		int itemQTY = player.inventory_QTY[i]; //get item quantity

		if (ID == itemID::None) //skip iteration if index is empty
			continue;

		baseItem item = itemDatabase[ID]; //get item from itemDatabase using itemID key

		
		if (item.rarity == itemRarity::Legendary)
		{
			sort_legendary.push_back(ID);
			sort_legendaryQTY.push_back(itemQTY);
		}
		else if (item.rarity == itemRarity::Epic)
		{
			sort_epic.push_back(ID);
			sort_epicQTY.push_back(itemQTY);
		}
		else if (item.rarity == itemRarity::Rare)
		{
			sort_rare.push_back(ID);
			sort_rareQTY.push_back(itemQTY);
		}
		else if (item.rarity == itemRarity::Uncommon)
		{
			sort_uncommon.push_back(ID);
			sort_uncommonQTY.push_back(itemQTY);
		}
		else if (item.rarity == itemRarity::Common)
		{
			sort_common.push_back(ID);
			sort_commonQTY.push_back(itemQTY);
		}
	}

	int sortedItems{ 0 }; //amount of items sorted.

	//push items from containers into sorted array by order of rarity
	for (int i{ 0 }; i < sort_legendary.size(); i++)
	{
		player.inventory[sortedItems] = sort_legendary[i];
		player.inventory_QTY[sortedItems] = sort_legendaryQTY[i];
		sortedItems++;
	}
	for (int i{ 0 }; i < sort_epic.size(); i++)
	{
		player.inventory[sortedItems] = sort_epic[i];
		player.inventory_QTY[sortedItems] = sort_epicQTY[i];
		sortedItems++;
	}
	for (int i{ 0 }; i < sort_rare.size(); i++)
	{
		player.inventory[sortedItems] = sort_rare[i];
		player.inventory_QTY[sortedItems] = sort_rareQTY[i];
		sortedItems++;
	}
	for (int i{ 0 }; i < sort_uncommon.size(); i++)
	{
		player.inventory[sortedItems] = sort_uncommon[i];
		player.inventory_QTY[sortedItems] = sort_uncommonQTY[i];
		sortedItems++;
	}
	for (int i{ 0 }; i < sort_common.size(); i++)
	{
		player.inventory[sortedItems] = sort_common[i];
		player.inventory_QTY[sortedItems] = sort_commonQTY[i];
		sortedItems++;
	}

	//fill remaining slots within sorted with itemID::None 
	for (int i{ sortedItems }; i < player.inventorySize; i++)
	{
		player.inventory[i] = itemID::None;
		player.inventory_QTY[i] = 0;
	}
}

void displayValue(baseItem& item, bool all, int& qty)
{

	int value;
	double temp = item.priceValue;

	//If item is not loot, it sells for 20% of value
	if (item.type == itemType::Loot)
		value = item.priceValue;
	else
	{
		temp /= 100;

		if (temp < 0.05) //to ensure being rounded to 1
			temp = 0.05;

		value = temp * 20;
	}

	if (all == true)
		value *= qty;

	if (value < 100)
		cout << typeColor("Copper") << value << "c" << clearColor();
	else if (value >= 100 && value < 10000)
	{
		int silver{0};
		int copper{0};

		for (int i{ 100 }; i < value;)
		{
			silver++;
			i += 100;
		}

		value -= silver * 100;

		for (int i{ 1 }; i < value; i++)
		{
			copper++;
		}

		cout << typeColor("Silver") << silver << "s " << typeColor("Copper") << value << "c" << clearColor();
	}
	else if (value >= 10000)
	{
		int gold = value / 10000;
		value -= gold * 10000;

		int silver = value / 100;
		value -= silver * 100;

		int copper = value;

		cout << typeColor("Gold") << gold << "g " << typeColor("Silver") << silver << "s " << typeColor("Copper") << value << "c" << clearColor();
	}
	
}

void print_buffs(buff b)
{
	cout << typeColor("Health");
	if (b.maxHP != 0)
		cout << " +" << b.maxHP << " HP " << endl << "                                                     ";
	if (b.maxMP != 0)
		cout << " +" << b.maxMP << " MP ";
	if (b.hpRegen != 0)
		cout << " +" << b.hpRegen << " HP Regeneration " << endl << "                                                     ";
	if (b.mpRegen != 0)
		cout << " +" << b.mpRegen << " MP Regeneration " << endl << "                                                     ";
	if (b.spellPower != 0)
		cout << " +" << b.spellPower << " Spell Power " << endl << "                                                     ";
	if (b.atk != 0)
		cout << " +" << b.atk << " Attack " << endl << "                                                     ";
	if (b.defense != 0)
		cout << " +" << b.defense << " Defense " << endl << "                                                     ";
	if (b.magicResist != 0)
		cout << " +" << b.magicResist << " Magic Resist " << endl << "                                                     ";
	if (b.hitChance != 0)
		cout << " +" << b.hitChance << "% Hit Chance " << endl << "                                                     ";
	if (b.dodgeChance != 0)
		cout << " +" << b.dodgeChance << "% Dodge Chance " << endl << "                                                     ";
	if (b.critChance != 0)
		cout << " +" << b.critChance << "% Critical Chance " << endl << "                                                     ";
	if (b.critMult != 0)
		cout << " +" << b.critMult << "x Critical Multiplier " << endl << "                                                     ";
	if (b.lifesteal != 0)
		cout << " +" << b.lifesteal << "% Life Steal " << endl << "                                                     ";
	if (b.spellVamp != 0)
		cout << " +" << b.spellVamp << "% Spell Vamp " << endl << "                                                     ";
	if (b.thorns != 0)
		cout << " +" << b.thorns << " Thorns " << endl << "                                                     ";

	cout << clearColor();
}





