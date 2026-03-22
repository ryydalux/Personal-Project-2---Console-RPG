#include <iostream>
#include <vector>
#include <conio.h>
#include <iomanip>

#include "menu_equipement.h"
#include "menu_Character.h"
#include "Player.h"
#include "Utility.h"
#include "item_ID.h"
#include "menu_Status.h" //for getEquipItem function
#include "item_dataBase.h"
using namespace std;


int manage_equipementMenu(Player& player)
{
	vector<itemID> equipList; //Holds any compatible equipement items found depending on itemType selected
	vector<int> itemPosition; //Parallel to equipList. It remembers at which index the displayed equipements are located in the player's inventory. (will be useful to equip and unequip functions)
	itemType typeSelected {itemType::None};
	int back{ 0 };
	int rememberSlotChoice{ 12 }; //Makes cursor lands on previously selected slot if player decides to cancel equip
	int ringSlot{0}; //Lets program know if user selected ring slot 1 or 2

	while (typeSelected != itemType::Potion) //I use itemType::Potion as a back because its an impossible result
	{
		ignore = false; //Reset
		ringSlot = 0; //Reset
		typeSelected = display_equipementSlots(player, rememberSlotChoice, ringSlot);

		if (typeSelected != itemType::Potion || typeSelected != itemType::None) //If user selected a slot instead of pressing backspace or DELETE
		{
			if (typeSelected == itemType::Shield)
			{
				if (checkTwoHand(player) == true)
				{
					system("cls");
					print_equipementMenuheader();
					cout << "                                        Cannot equip shields with a two-handed weapon";
					cout << "\n\n                                                    > [I Understand]";

					int keyPressed = _getch();

					while (keyPressed != 13 && keyPressed != 8)
						keyPressed = _getch();

					continue;
				}
			}

			
			
			equipList.clear(); //Reset
			itemPosition.clear(); //Reset

			get_equipList(player, typeSelected, equipList, itemPosition); //Get items compatible with equip slot chosen

			if (equipList.size() > 0)
			{
				int equipSelected = print_equipList(player, typeSelected, equipList); //Returns the selected item to equip (equipSelected represent index in equipList && itemPosition)
		
				if (equipSelected != -1)
				{
					bool emptySlot = checkEmptySlot(player, typeSelected, ringSlot);

					if (emptySlot == false) //equip slot selected is NOT empty 
					{
						bool fullInventory{ true }; //assume inventory is full until proven otherwise with loop below

						for (itemID check : player.inventory)
						{
							if (check == itemID::None)
							{
								fullInventory = false;
								break;
							}
						}

						if (fullInventory == true) //if inventory is full + equip slot != empty, call replaceEquip() because unequipItem/equipItem functions are not designed for this scenario
						{
							//Check if player is trying to equip a two hander while having a shield. If yes = prompt that they need to free some space in inventory.
							itemID ID = equipList[equipSelected];
							baseItem equip = itemDatabase[ID];

							if (equip.type == itemType::Weapon && player.e_shield != itemID::None)
							{
								weapon_stats wpn = get<weapon_stats>(equip.stats);

								if (wpn.twoHanded == true)
								{
									system("cls");
									print_equipementMenuheader();
									cout << "                  Cannot equip two-handed weapon because inventory is full and cannot retrieve your shield.";
									cout << "\n\n                                                    > [I Understand]";

									int keyPressed = _getch();
										
									while (keyPressed != 13 && keyPressed != 8)
										keyPressed = _getch();

									continue;
								}
							}
							else
							{
								replaceEquip(player, equipList, itemPosition, equipSelected, ringSlot);
								continue; //(to go back to equip slot list)
							}
						}
						else //If inventory has space, but equip slot is full = unequip slot first, then equip item
						{ 
							unequipItem(player, typeSelected, ringSlot);
							equipItem(player, typeSelected, itemPosition, ringSlot, equipSelected);
						}
					}
					else if (emptySlot == true) // if equipSlot is empty + inv has space = just equip item
					{
						equipItem(player, typeSelected, itemPosition, ringSlot, equipSelected);
					}
				}
			}
		}

	}
	return back;
}

	


bool checkTwoHand(Player& player) 
{
	itemID ID = player.e_weapon;

	if (ID == itemID::None)
		return false;

	baseItem item = itemDatabase[ID];
	weapon_stats wpn = get<weapon_stats>(item.stats);

	if (wpn.twoHanded == false)
		return false;
	else
		return true;
}

void print_equipementMenuheader()
{
	cout << "========================================================================================================================\n						     Equipement Menu\n"
		"========================================================================================================================\n\n\n\n\n\n";
}

itemType display_equipementSlots(Player& player, int& rememberSlotChoice, int& ringSlot)
{
	system("cls");
	print_equipementMenuheader();

	baseItem e_weapon, e_shield, e_head, e_shoulders, e_cape, e_chest, e_arms, e_legs, e_feet, e_ring1, e_ring2, e_necklace;
	getEquipItem(player, e_weapon, e_shield, e_head, e_shoulders, e_cape, e_chest, e_arms, e_legs, e_feet, e_ring1, e_ring2, e_necklace);


	for (int i { rememberSlotChoice }; i < 13;)
	{ 
		
		cout << "                                "; 
		if (i == 12) { cout << ">"; }
		else { cout << " "; }
		cout << " Weapon ------ " << get_itemColor(e_weapon) << e_weapon.name << clearColor() << endl;

		cout << "                                "; 
		if (i == 11) { cout << ">"; }
		else { cout << " "; }
		cout << " Shield ------ " << get_itemColor(e_shield) << e_shield.name << clearColor() << endl << endl;

		cout << "                                "; 
		if (i == 10) { cout << ">"; }
		else { cout << " "; }
		cout << " Head -------- " << get_itemColor(e_head) << e_head.name << clearColor() << endl;

		cout << "                                "; 
		if (i == 9) { cout << ">"; }
		else { cout << " "; }
		cout << " Shoulders --- " << get_itemColor(e_shoulders) << e_shoulders.name << clearColor() << endl;

		cout << "                                "; 
		if (i == 8) { cout << ">"; }
		else { cout << " "; }
		cout << " Chest ------- " << get_itemColor(e_chest) << e_chest.name << clearColor() << endl;

		cout << "                                "; 
		if (i == 7) { cout << ">"; }
		else { cout << " "; }
		cout << " Arms -------- " << get_itemColor(e_arms) << e_arms.name << clearColor() << endl;

		cout << "                                "; 
		if (i == 6) { cout << ">"; }
		else { cout << " "; }
		cout << " Legs -------- " << get_itemColor(e_legs) << e_legs.name << clearColor() << endl;

		cout << "                                "; 
		if (i == 5) { cout << ">"; }
		else { cout << " "; }
		cout << " Feet -------- " << get_itemColor(e_feet) << e_feet.name << clearColor() << endl << endl;

		cout << "                                "; 
		if (i == 4) { cout << ">"; }
		else { cout << " "; }
		cout << " Cape -------- " << get_itemColor(e_cape) << e_cape.name << clearColor() << endl;

		cout << "                                "; 
		if (i == 3) { cout << ">"; }
		else { cout << " "; }
		cout << " Ring 1 ------ " << get_itemColor(e_ring1) << e_ring1.name << clearColor() << endl;

		cout << "                                "; 
		if (i == 2) { cout << ">"; }
		else { cout << " "; }
		cout << " Ring 2 ------ " << get_itemColor(e_ring2) << e_ring2.name << clearColor() << endl;

		cout << "                                "; 
		if (i == 1) { cout << ">"; }
		else { cout << " "; }
		cout << " Neck -------- " << get_itemColor(e_necklace) << e_necklace.name << clearColor() << endl;
	
		cout << "\n                                  F = Unequip item";

		int keyPressed = _getch();

		while (keyPressed != 119 && keyPressed != 87 && keyPressed != 115 && keyPressed != 83 && keyPressed != 13 && keyPressed != 8 && keyPressed != 102 && keyPressed != 70) //if user does not input W/S/BS/ENTER = do nothing
		{
			keyPressed = _getch();
		}

		if (keyPressed == 119 || keyPressed == 87) //w
		{
			i++;

			if (i == 13)
				i = 1;

			utility_eraseLines(15);
		}

		if (keyPressed == 115 || keyPressed == 83) //s
		{
			i--;
	
			if (i == 0)
				i = 12;

			utility_eraseLines(15);
		}

		if (keyPressed == 13) //ENTER
		{
			if (i == 12)
			{
				rememberSlotChoice = i;
				return itemType::Weapon;
			}
			else if (i == 11)
			{
				rememberSlotChoice = i;
				return itemType::Shield;
			}
			else if (i == 10)
			{
				rememberSlotChoice = i;
				return itemType::Head;
			}
			else if (i == 9)
			{
				rememberSlotChoice = i;
				return itemType::Shoulders;
			}
			else if (i == 8)
			{
				rememberSlotChoice = i;
				return itemType::Chest;
			}
			else if (i == 7)
			{
				rememberSlotChoice = i;
				return itemType::Arms;
			}
			else if (i == 6)
			{
				rememberSlotChoice = i;
				return itemType::Legs;
			}
			else if (i == 5)
			{
				rememberSlotChoice = i;
				return itemType::Feet;
			}
			else if (i == 4)
			{
				rememberSlotChoice = i;
				return itemType::Cape;
			}
			else if (i == 3)
			{
				ringSlot = 1;
				rememberSlotChoice = i;
				return itemType::Ring;
			}
			else if (i == 2)
			{
				ringSlot = 2;
				rememberSlotChoice = i;
				return itemType::Ring;
			}
			else if (i == 1)
			{
				rememberSlotChoice = i;
				return itemType::Necklace;
			}
		}

		if (keyPressed == 102 || keyPressed == 70) //F (unequip option)
		{
			if (i == 12)
			{
				rememberSlotChoice = i;
				unequipItem(player, itemType::Weapon, 0);
				return itemType::None;
			}
			else if (i == 11)
			{
				rememberSlotChoice = i;
				unequipItem(player, itemType::Shield, 0);
				return itemType::None;
			}
			else if (i == 10)
			{
				rememberSlotChoice = i;
				unequipItem(player, itemType::Head, 0);
				return itemType::None;
			}
			else if (i == 9)
			{
				rememberSlotChoice = i;
				unequipItem(player, itemType::Shoulders, 0);
				return itemType::None;
			}
			else if (i == 8)
			{
				rememberSlotChoice = i;
				unequipItem(player, itemType::Chest, 0);
				return itemType::None;
			}
			else if (i == 7)
			{
				rememberSlotChoice = i;
				unequipItem(player, itemType::Arms, 0);
				return itemType::None;
			}
			else if (i == 6)
			{
				rememberSlotChoice = i;
				unequipItem(player, itemType::Legs, 0);
				return itemType::None;
			}
			else if (i == 5)
			{
				rememberSlotChoice = i;
				unequipItem(player, itemType::Feet, 0);
				return itemType::None;
			}
			else if (i == 4)
			{
				rememberSlotChoice = i;
				unequipItem(player, itemType::Cape, 0);
				return itemType::None;
			}
			else if (i == 3)
			{
				ringSlot = 1;
				rememberSlotChoice = i;
				unequipItem(player, itemType::Ring, 1);
				return itemType::None;
			}
			else if (i == 2)
			{
				ringSlot = 2;
				rememberSlotChoice = i;
				unequipItem(player, itemType::Ring, 2);
				return itemType::None;
			}
			else if (i == 1)
			{
				rememberSlotChoice = i;
				unequipItem(player, itemType::Necklace, 0);
				return itemType::None;
			}
		}

		if (keyPressed == 8) //BS
		{
			return itemType::Potion; //cant put itemType::None or else player wont be able to select an empty slot. itemType::Potion is an impossible result so it's safe to use as an exit option
		}
	}
}

void get_equipList(Player& player, itemType typeSelected, vector<itemID>& equipList, vector<int>& itemPosition)
{
	if (typeSelected == itemType::Weapon)
	{
		for (int i{ 0 }; i < player.inventorySize; i++)
		{
			itemID ID = player.inventory[i];
			baseItem item = itemDatabase[ID];

			if (item.type == itemType::Weapon)
			{
				equipList.push_back(ID);
				itemPosition.push_back(i);
			}
		}
	}
	else if (typeSelected == itemType::Shield)
	{
		for (int i{ 0 }; i < player.inventorySize; i++)
		{
			itemID ID = player.inventory[i];
			baseItem item = itemDatabase[ID];

			if (item.type == itemType::Shield)
			{
				equipList.push_back(ID);
				itemPosition.push_back(i);
			}
		}
	}
	else if (typeSelected == itemType::Head)
	{
		for (int i{ 0 }; i < player.inventorySize; i++)
		{
			itemID ID = player.inventory[i];
			baseItem item = itemDatabase[ID];

			if (item.type == itemType::Head)
			{
				equipList.push_back(ID);
				itemPosition.push_back(i);
			}
		}
	}
	else if (typeSelected == itemType::Shoulders)
	{
		for (int i{ 0 }; i < player.inventorySize; i++)
		{
			itemID ID = player.inventory[i];
			baseItem item = itemDatabase[ID];

			if (item.type == itemType::Shoulders)
			{
				equipList.push_back(ID);
				itemPosition.push_back(i);
			}
		}
	}
	else if (typeSelected == itemType::Chest)
	{
		for (int i{ 0 }; i < player.inventorySize; i++)
		{
			itemID ID = player.inventory[i];
			baseItem item = itemDatabase[ID];

			if (item.type == itemType::Chest)
			{
				equipList.push_back(ID);
				itemPosition.push_back(i);
			}
		}
	}
	else if (typeSelected == itemType::Arms)
	{
		for (int i{ 0 }; i < player.inventorySize; i++)
		{
			itemID ID = player.inventory[i];
			baseItem item = itemDatabase[ID];

			if (item.type == itemType::Arms)
			{
				equipList.push_back(ID);
				itemPosition.push_back(i);
			}
		}
	}
	else if (typeSelected == itemType::Legs)
	{
		for (int i{ 0 }; i < player.inventorySize; i++)
		{
			itemID ID = player.inventory[i];
			baseItem item = itemDatabase[ID];

			if (item.type == itemType::Legs)
			{
				equipList.push_back(ID);
				itemPosition.push_back(i);
			}
		}
	}
	else if (typeSelected == itemType::Feet)
	{
		for (int i{ 0 }; i < player.inventorySize; i++)
		{
			itemID ID = player.inventory[i];
			baseItem item = itemDatabase[ID];

			if (item.type == itemType::Feet)
			{
				equipList.push_back(ID);
				itemPosition.push_back(i);
			}
		}
	}
	else if (typeSelected == itemType::Cape)
	{
		for (int i{ 0 }; i < player.inventorySize; i++)
		{
			itemID ID = player.inventory[i];
			baseItem item = itemDatabase[ID];

			if (item.type == itemType::Cape)
			{
				equipList.push_back(ID);
				itemPosition.push_back(i);
			}
		}
	}
	else if (typeSelected == itemType::Ring)
	{
		for (int i{ 0 }; i < player.inventorySize; i++)
		{
			itemID ID = player.inventory[i];
			baseItem item = itemDatabase[ID];

			if (item.type == itemType::Ring)
			{
				equipList.push_back(ID);
				itemPosition.push_back(i);
			}
		}
	}
	else if (typeSelected == itemType::Necklace)
	{
		for (int i{ 0 }; i < player.inventorySize; i++)
		{
			itemID ID = player.inventory[i];
			baseItem item = itemDatabase[ID];

			if (item.type == itemType::Necklace)
			{
				equipList.push_back(ID);
				itemPosition.push_back(i);
			}
		}
	}
}


int print_equipList(Player& player, itemType typeSelected, vector<itemID>& equipList) //prints the items that are only compatible with the selected equip slot
{
	int cursorIndex{ 0 };

	system("cls");
	print_equipementMenuheader();
	utility_eraseLines(2);
	cout << "                                     Select an item to equip:\n\n";

	while (true)
	{
		for (int i{ 0 }; i < equipList.size(); i++)
		{
			if (typeSelected == itemType::Weapon)
			{

				itemID ID = equipList[i];
				baseItem item = itemDatabase[ID];

				cout << "                                     ";

				if (cursorIndex == i)
					cout << "> ";
				else
					cout << "  ";

				cout << get_itemColor(item) << item.name << clearColor() << endl;
			}
			else if (typeSelected == itemType::Shield)
			{

				itemID ID = equipList[i];
				baseItem item = itemDatabase[ID];

				cout << "                                     ";

				if (cursorIndex == i)
					cout << "> ";
				else
					cout << "  ";

				cout << get_itemColor(item) << item.name << clearColor() << endl;
			}
			else if (typeSelected == itemType::Head)
			{

				itemID ID = equipList[i];
				baseItem item = itemDatabase[ID];

				cout << "                                     ";

				if (cursorIndex == i)
					cout << "> ";
				else
					cout << "  ";

				cout << get_itemColor(item) << item.name << clearColor() << endl;
			}
			else if (typeSelected == itemType::Shoulders)
			{

				itemID ID = equipList[i];
				baseItem item = itemDatabase[ID];

				cout << "                                     ";

				if (cursorIndex == i)
					cout << "> ";
				else
					cout << "  ";

				cout << get_itemColor(item) << item.name << clearColor() << endl;
			}
			else if (typeSelected == itemType::Chest)
			{

				itemID ID = equipList[i];
				baseItem item = itemDatabase[ID];

				cout << "                                     ";

				if (cursorIndex == i)
					cout << "> ";
				else
					cout << "  ";

				cout << get_itemColor(item) << item.name << clearColor() << endl;
			}
			else if (typeSelected == itemType::Arms)
			{

				itemID ID = equipList[i];
				baseItem item = itemDatabase[ID];

				cout << "                                     ";

				if (cursorIndex == i)
					cout << "> ";
				else
					cout << "  ";

				cout << get_itemColor(item) << item.name << clearColor() << endl;
			}
			else if (typeSelected == itemType::Legs)
			{

				itemID ID = equipList[i];
				baseItem item = itemDatabase[ID];

				cout << "                                     ";

				if (cursorIndex == i)
					cout << "> ";
				else
					cout << "  ";

				cout << get_itemColor(item) << item.name << clearColor() << endl;
			}
			else if (typeSelected == itemType::Feet)
			{

				itemID ID = equipList[i];
				baseItem item = itemDatabase[ID];

				cout << "                                     ";

				if (cursorIndex == i)
					cout << "> ";
				else
					cout << "  ";

				cout << get_itemColor(item) << item.name << clearColor() << endl;
			}
			else if (typeSelected == itemType::Cape)
			{

				itemID ID = equipList[i];
				baseItem item = itemDatabase[ID];

				cout << "                                     ";

				if (cursorIndex == i)
					cout << "> ";
				else
					cout << "  ";

				cout << get_itemColor(item) << item.name << clearColor() << endl;
			}
			else if (typeSelected == itemType::Ring)
			{

				itemID ID = equipList[i];
				baseItem item = itemDatabase[ID];

				cout << "                                     ";

				if (cursorIndex == i)
					cout << "> ";
				else
					cout << "  ";

				cout << get_itemColor(item) << item.name << clearColor() << endl;
			}
			else if (typeSelected == itemType::Necklace)
			{

				itemID ID = equipList[i];
				baseItem item = itemDatabase[ID];

				cout << "                                     ";

				if (cursorIndex == i)
					cout << "> ";
				else
					cout << "  ";

				cout << get_itemColor(item) << item.name << clearColor() << endl;
			}
		}

		int keyPressed = _getch();

		while (keyPressed != 119 && keyPressed != 87 && keyPressed != 115 && keyPressed != 83 && keyPressed != 13 && keyPressed != 8) //if user does not input W/S/BS/ENTER = do nothing
		{
			keyPressed = _getch();
		}

		if (keyPressed == 119 || keyPressed == 87) //W
		{
			cursorIndex--;

			if (cursorIndex < 0)
				cursorIndex = 0;

			utility_eraseLines(equipList.size());
		}

		if (keyPressed == 115 || keyPressed == 83) //S
		{
			cursorIndex++;

			if (cursorIndex == equipList.size())
				cursorIndex = equipList.size()-1;

			utility_eraseLines(equipList.size());
		}

		if (keyPressed == 8) //BS
		{
			return -1;
		}

		if (keyPressed == 13) //ENTER
		{
			return cursorIndex; //Reprents index to look at in itemPosition array
		}
	}
}


void unequipItem(Player& player, itemType typeSelected, int ringSlot)
{
	//Check if inventory is full before unequipping
	for (int i{ 0 }; i < player.inventorySize; i++)
	{
		if (player.inventory[i] == itemID::None)
		{
			break; //Empty space found = proceed with the unequipping
		}

		if (i == player.inventorySize - 1) //If no free space was found in inventory, cancel unequipping
		{
			system("cls");
			print_equipementMenuheader();
			cout << "                                           Inventory is full, cannot unequip item";
			cout << "\n\n                                                    > [I Understand]";

			int keyPressed = _getch();

			while (keyPressed != 13 && keyPressed != 8)
				keyPressed = _getch();

			return;
		}
	}
	
	if (typeSelected == itemType::Weapon)
	{
		itemID ID = player.e_weapon;

		//if (ID == itemID::None) //If player is trying to unequip nothing OR has no equipement for that type yet = ignore rest of function
		//	return;

		//Remove weapon from equip slot
		player.e_weapon = itemID::None;

		//Add weapon back into player's inventory
		
		for (int i{0}; i < player.inventorySize; i++)
		{
			if (player.inventory[i] == itemID::None)
			{
				player.inventory[i] = ID;
				player.inventory_QTY[i] = 1;
				break;
			}
		}
		
		//Remove weapon stats on player
		baseItem item = itemDatabase[ID];
		weapon_stats equip = get<weapon_stats>(item.stats);
		
		if (equip.atk > 0)
			player.sub_atk(equip.atk);
		else if (equip.atk < 0)
			player.add_atk(equip.atk * -1);

		if (equip.spellPower > 0)
			player.sub_spellPower(equip.spellPower);
		else if (equip.spellPower < 0)
			player.add_spellPower(equip.spellPower * -1);

		if (equip.hitChance > 0)
			player.sub_hitChance(equip.hitChance);
		else if (equip.hitChance < 0)
			player.add_hitChance(equip.hitChance * -1);

		if (equip.critChance > 0)
			player.sub_critChance(equip.critChance);
		else if (equip.critChance < 0)
			player.add_critChance(equip.critChance * -1);

		//Remove enchants if any
		if (item.enchants)
		{
			enchantments e = *item.enchants;
			unequip_ItemEnchants(player, e);
		}
		
	}
	else if (typeSelected == itemType::Shield || typeSelected == itemType::Head || typeSelected == itemType::Shoulders || typeSelected == itemType::Chest || typeSelected == itemType::Arms || typeSelected == itemType::Legs || typeSelected == itemType::Feet)
	{
		itemID ID;

		if (typeSelected == itemType::Shield)
		{ 
			ID = player.e_shield;

			if (ID == itemID::None) 
				return;

			player.e_shield = itemID::None;
		}
		else if (typeSelected == itemType::Head)
		{
			ID = player.e_head;

			if (ID == itemID::None) 
				return; 

			player.e_head = itemID::None;
		}
		else if (typeSelected == itemType::Shoulders)
		{
			ID = player.e_shoulders;

			if (ID == itemID::None) 
				return;

			player.e_shoulders = itemID::None;
		}
		else if (typeSelected == itemType::Chest)
		{
			ID = player.e_chest;

			if (ID == itemID::None) 
				return;

			player.e_chest = itemID::None;
		}
		else if (typeSelected == itemType::Arms)
		{
			ID = player.e_arms;

			if (ID == itemID::None) 
				return;

			player.e_arms = itemID::None;
		}
		else if (typeSelected == itemType::Legs)
		{
			ID = player.e_legs;

			if (ID == itemID::None) 
				return;

			player.e_legs = itemID::None;
		}
		else if (typeSelected == itemType::Feet)
		{
			ID = player.e_feet;

			if (ID == itemID::None) 
				return;

			player.e_feet = itemID::None;
		}

		for (int i{ 0 }; i < player.inventorySize; i++)
		{
			if (player.inventory[i] == itemID::None)
			{
				player.inventory[i] = ID;
				player.inventory_QTY[i] = 1;
				break;
			}
		}

		baseItem item = itemDatabase[ID];
		armor_stats equip = get<armor_stats>(item.stats);

		if (equip.def > 0)
			player.sub_def(equip.def);
		else if (equip.def < 0)
			player.add_def(equip.def * -1);

		if (equip.magicResist > 0)
			player.sub_magicResist(equip.magicResist);
		else if (equip.magicResist < 0)
			player.add_magicResist(equip.magicResist * -1);

		if (equip.dodgeChance > 0)
			player.sub_dodgeChance(equip.dodgeChance);
		else if (equip.dodgeChance < 0)
			player.add_dodgeChance(equip.dodgeChance * -1);

		if (item.enchants)
		{
			enchantments e = *item.enchants;
			unequip_ItemEnchants(player, e);
		}

	}
	else if (typeSelected == itemType::Cape || typeSelected == itemType::Necklace)
	{
		itemID ID;

		if (typeSelected == itemType::Cape)
		{
			ID = player.e_cape;

			if (ID == itemID::None) 
				return;

			player.e_cape = itemID::None;
		}
		else if (typeSelected == itemType::Necklace)
		{
			ID = player.e_necklace;

			if (ID == itemID::None)
				return;

			player.e_necklace = itemID::None;
		}

		for (int i{ 0 }; i < player.inventorySize; i++)
		{
			if (player.inventory[i] == itemID::None)
			{
				player.inventory[i] = ID;
				player.inventory_QTY[i] = 1;
				break;
			}
		}

		baseItem item = itemDatabase[ID];

		if (item.enchants)
		{
			enchantments e = *item.enchants;
			unequip_ItemEnchants(player, e);
		}

	}
	else if (typeSelected == itemType::Ring)
	{
		itemID ID;

		if (ringSlot == 1)
		{
			ID = player.e_ring1;
			player.e_ring1 = itemID::None;
		}
		else if (ringSlot == 2)
		{
			ID = player.e_ring2;
			player.e_ring2 = itemID::None;
		}

		if (ID == itemID::None) 
			return;
		
		for (int i{ 0 }; i < player.inventorySize; i++)
		{
			if (player.inventory[i] == itemID::None)
			{
				player.inventory[i] = ID;
				player.inventory_QTY[i] = 1;
				break;
			}
		}

		baseItem item = itemDatabase[ID];

		if (item.enchants)
		{
			enchantments e = *item.enchants;
			unequip_ItemEnchants(player, e);
		}
	}
}


void equipItem(Player& player, itemType typeSelected, vector<int>& itemPosition, int ringSlot, int equipSelected)
{
	if (typeSelected == itemType::Weapon)
	{
		//Store weapon we want to equip
		int index = itemPosition[equipSelected];
		itemID ID = player.inventory[index]; 

		//Equip Weapon
		player.e_weapon = ID;
		
		//Remove weapon from player's inventory
		player.inventory[index] = itemID::None;
		player.inventory_QTY[index] = 0;
		
		//Apply weapon stats on player
		baseItem item = itemDatabase[ID];
		weapon_stats equip = get<weapon_stats>(item.stats);
		
	
		player.add_atk(equip.atk);
		player.add_spellPower(equip.spellPower);
		player.add_hitChance(equip.hitChance);
		player.add_critChance(equip.critChance);
		
		//Apply weapon enchants if any
		if (item.enchants)
		{
			enchantments e = *item.enchants;
			equip_ItemEnchants(player, e);
		}

		//Check if weapon is two handed, if yes = unequip shield
		if (equip.twoHanded == true && player.e_shield != itemID::None)
		{
			unequipItem(player, itemType::Shield, 0);
		}
	}
	else if (typeSelected == itemType::Shield || typeSelected == itemType::Head || typeSelected == itemType::Shoulders || typeSelected == itemType::Chest || typeSelected == itemType::Arms || typeSelected == itemType::Legs || typeSelected == itemType::Feet)
	{
		int index = itemPosition[equipSelected];
		itemID ID = player.inventory[index];

		if (typeSelected == itemType::Shield)
			player.e_shield = ID;
		else if (typeSelected == itemType::Head)
			player.e_head = ID;
		else if (typeSelected == itemType::Shoulders)
			player.e_shoulders = ID;
		else if (typeSelected == itemType::Chest)
			player.e_chest = ID;
		else if (typeSelected == itemType::Arms)
			player.e_arms = ID;
		else if (typeSelected == itemType::Legs)
			player.e_legs = ID;
		else if (typeSelected == itemType::Feet)
			player.e_feet = ID;

		player.inventory[index] = itemID::None;
		player.inventory_QTY[index] = 0;

		baseItem item = itemDatabase[ID];
		armor_stats equip = get<armor_stats>(item.stats);

		player.add_def(equip.def);
		player.add_magicResist(equip.magicResist);
		player.add_dodgeChance(equip.dodgeChance);

		if (item.enchants)
		{
			enchantments e = *item.enchants;
			equip_ItemEnchants(player, e);
		}
	}
	else if (typeSelected == itemType::Cape || typeSelected == itemType::Necklace)
	{
		int index = itemPosition[equipSelected];
		itemID ID = player.inventory[index];

		if (typeSelected == itemType::Cape)
			player.e_cape = ID;
		else if (typeSelected == itemType::Necklace)
			player.e_necklace = ID;

		player.inventory[index] = itemID::None;
		player.inventory_QTY[index] = 0;

		baseItem item = itemDatabase[ID];

		if (item.enchants)
		{
			enchantments e = *item.enchants;
			equip_ItemEnchants(player, e);
		}
	}
	else if (typeSelected == itemType::Ring)
	{
		int index = itemPosition[equipSelected];
		itemID ID = player.inventory[index];

		if (ringSlot == 1)
			player.e_ring1 = ID;
		else if (ringSlot == 2)
			player.e_ring2 = ID;

		player.inventory[index] = itemID::None;
		player.inventory_QTY[index] = 0;

		baseItem item = itemDatabase[ID];

		if (item.enchants)
		{
			enchantments e = *item.enchants;
			equip_ItemEnchants(player, e);
		}
	}
}

void unequip_ItemEnchants(Player& player, enchantments e)
{
	if (e.maxHP > 0)
	{
		player.sub_maxHP(e.maxHP);
		player.sub_HP(e.maxHP);
	}
	else if (e.maxHP < 0)
	{
		player.add_maxHP(e.maxHP * -1);
		player.add_HP(e.maxHP * -1);
	}

	if (e.maxMP > 0)
	{
		player.sub_maxMP(e.maxMP);
		player.sub_MP(e.maxHP);
	}
	else if (e.maxMP < 0)
	{
		player.add_maxMP(e.maxMP * -1);
		player.add_MP(e.maxMP * -1);
	}

	if (e.hpRegen > 0)
		player.sub_hpRegen(e.hpRegen);
	else if (e.hpRegen < 0)
		player.add_hpRegen(e.hpRegen * -1);

	if (e.mpRegen > 0)
		player.sub_mpRegen(e.mpRegen);
	else if (e.mpRegen < 0)
		player.add_mpRegen(e.mpRegen * -1);

	if (e.atk > 0)
		player.sub_atk(e.atk);
	else if (e.atk < 0)
		player.add_atk(e.atk * -1);

	if (e.spellPower > 0)
		player.sub_spellPower(e.spellPower);
	else if (e.spellPower < 0)
		player.add_spellPower(e.spellPower * -1);

	if (e.defense > 0)
		player.sub_def(e.defense);
	else if (e.defense < 0)
		player.add_def(e.defense * -1);

	if (e.magicResist > 0)
		player.sub_magicResist(e.magicResist);
	else if (e.magicResist < 0)
		player.add_magicResist(e.magicResist * -1);

	if (e.hitChance > 0)
		player.sub_hitChance(e.hitChance);
	else if (e.hitChance < 0)
		player.add_hitChance(e.hitChance * -1);

	if (e.dodgeChance > 0)
		player.sub_dodgeChance(e.dodgeChance);
	else if (e.dodgeChance < 0)
		player.add_dodgeChance(e.dodgeChance * -1);

	if (e.critChance > 0)
		player.sub_critChance(e.critChance);
	else if (e.critChance < 0)
		player.add_critChance(e.critChance * -1);

	if (e.critMult > 0)
		player.sub_critMult(e.critMult);
	else if (e.critMult < 0)
		player.add_critMult(e.critMult * -1);

	if (e.lifesteal > 0)
		player.sub_lifesteal(e.lifesteal);
	else if (e.lifesteal < 0)
		player.add_lifesteal(e.lifesteal * -1);

	if (e.spellVamp > 0)
		player.sub_spellVamp(e.spellVamp);
	else if (e.spellVamp < 0)
		player.add_spellVamp(e.spellVamp * -1);

	if (e.thorns > 0)
		player.sub_thorns(e.thorns);
	else if (e.thorns < 0)
		player.add_thorns(e.thorns * -1);
}

void equip_ItemEnchants(Player& player, enchantments e)
{
	player.add_maxHP(e.maxHP);
	player.add_HP(e.maxHP);
	player.add_maxMP(e.maxMP);
	player.add_MP(e.maxMP);
	player.add_hpRegen(e.hpRegen);
	player.add_mpRegen(e.mpRegen);
	player.add_atk(e.atk);
	player.add_spellPower(e.spellPower);
	player.add_def(e.defense);
	player.add_magicResist(e.magicResist);
	player.add_hitChance(e.hitChance);
	player.add_dodgeChance(e.dodgeChance);
	player.add_critChance(e.critChance);
	player.add_critMult(e.critMult);
	player.add_lifesteal(e.lifesteal);
	player.add_spellVamp(e.spellVamp);
	player.add_thorns(e.thorns);
}

bool checkEmptySlot(Player& player, itemType& typeSelected, int& ringSlot)
{
	if (typeSelected == itemType::Weapon)
	{
		if (player.e_weapon == itemID::None)
			return true;
		else
			return false;
	}
	else if (typeSelected == itemType::Shield)
	{
		if (player.e_shield == itemID::None)
			return true;
		else
			return false;
	}
	else if (typeSelected == itemType::Head)
	{
		if (player.e_head == itemID::None)
			return true;
		else
			return false;
	}
	else if (typeSelected == itemType::Shoulders)
	{
		if (player.e_shoulders == itemID::None)
			return true;
		else
			return false;
	}
	else if (typeSelected == itemType::Chest)
	{
		if (player.e_chest == itemID::None)
			return true;
		else
			return false;
	}
	else if (typeSelected == itemType::Arms)
	{
		if (player.e_arms == itemID::None)
			return true;
		else
			return false;
	}
	else if (typeSelected == itemType::Legs)
	{
		if (player.e_legs == itemID::None)
			return true;
		else
			return false;
	}
	else if (typeSelected == itemType::Feet)
	{
		if (player.e_feet == itemID::None)
			return true;
		else
			return false;
	}
	else if (typeSelected == itemType::Cape)
	{
		if (player.e_cape == itemID::None)
			return true;
		else
			return false;
	}
	else if (typeSelected == itemType::Ring && ringSlot == 1)
	{
		if (player.e_ring1 == itemID::None)
			return true;
		else
			return false;
	}
	else if (typeSelected == itemType::Ring && ringSlot == 2)
	{
		if (player.e_ring2 == itemID::None)
			return true;
		else
			return false;
	}
	else if (typeSelected == itemType::Necklace)
	{
		if (player.e_necklace == itemID::None)
			return true;
		else
			return false;
	}
}

void replaceEquip(Player& player, vector<itemID>& equipList, vector<int>& itemPosition, int& equipSelected, int& ringSlot)
{
	itemID IDEquip = equipList[equipSelected]; //item to equip
	int atIndex = itemPosition[equipSelected]; //index of item to equip in player's inventory
	itemID IDUnequip;

	baseItem equip = itemDatabase[IDEquip];

	if (equip.type == itemType::Weapon)
	{
		IDUnequip = player.e_weapon;
		player.e_weapon = IDEquip;

		player.inventory[atIndex] = IDUnequip;
		player.inventory_QTY[atIndex] = 1;

		//Remove stats/enchants from unequipped item
		baseItem unequip = itemDatabase[IDUnequip];
		weapon_stats wpn = get<weapon_stats>(unequip.stats);

		if (wpn.atk > 0)
			player.sub_atk(wpn.atk);
		else if (wpn.atk < 0)
			player.add_atk(wpn.atk * -1);

		if (wpn.spellPower > 0)
			player.sub_spellPower(wpn.spellPower);
		else if (wpn.spellPower < 0)
			player.add_spellPower(wpn.spellPower * -1);

		if (wpn.hitChance > 0)
			player.sub_hitChance(wpn.hitChance);
		else if (wpn.hitChance < 0)
			player.add_hitChance(wpn.hitChance * -1);

		if (wpn.critChance > 0)
			player.sub_critChance(wpn.critChance);
		else if (wpn.critChance < 0)
			player.add_critChance(wpn.critChance * -1);

		if (unequip.enchants)
		{
			enchantments e = *unequip.enchants;
			unequip_ItemEnchants(player, e);
		}

		//Add stats/enchants from equipped item
		wpn = get<weapon_stats>(equip.stats);

		player.add_atk(wpn.atk);
		player.add_spellPower(wpn.spellPower);
		player.add_hitChance(wpn.hitChance);
		player.add_critChance(wpn.critChance);

		if (equip.enchants)
		{
			enchantments e = *equip.enchants;
			equip_ItemEnchants(player, e);
		}

	}
	else if (equip.type == itemType::Shield || equip.type == itemType::Head || equip.type == itemType::Shoulders || equip.type == itemType::Chest || equip.type == itemType::Arms || equip.type == itemType::Legs || equip.type == itemType::Feet)
	{
		if (equip.type == itemType::Shield)
		{
			IDUnequip = player.e_shield;
			player.e_shield = IDEquip;
		}
		else if (equip.type == itemType::Head)
		{
			IDUnequip = player.e_head;
			player.e_head = IDEquip;
		}
		else if (equip.type == itemType::Shoulders)
		{
			IDUnequip = player.e_shoulders;
			player.e_shoulders = IDEquip;
		}
		else if (equip.type == itemType::Chest)
		{
			IDUnequip = player.e_chest;
			player.e_chest = IDEquip;
		}
		else if (equip.type == itemType::Arms)
		{
			IDUnequip = player.e_arms;
			player.e_arms = IDEquip;
		}
		else if (equip.type == itemType::Legs)
		{
			IDUnequip = player.e_legs;
			player.e_legs = IDEquip;
		}
		else if (equip.type == itemType::Feet)
		{
			IDUnequip = player.e_feet;
			player.e_feet = IDEquip;
		}

		//remove stats/encahnts
		baseItem unequip = itemDatabase[IDUnequip];
		armor_stats armor = get<armor_stats>(unequip.stats);

		if (armor.def > 0)
			player.sub_def(armor.def);
		else if (armor.def < 0)
			player.add_def(armor.def * -1);

		if (armor.magicResist > 0)
			player.sub_magicResist(armor.magicResist);
		else if (armor.magicResist < 0)
			player.add_magicResist(armor.magicResist * -1);

		if (armor.dodgeChance > 0)
			player.sub_dodgeChance(armor.dodgeChance);
		else if (armor.dodgeChance < 0)
			player.add_dodgeChance(armor.dodgeChance * -1);

		if (unequip.enchants)
		{
			enchantments e = *unequip.enchants;
			unequip_ItemEnchants(player, e);
		}

		//add stats/enchants
		armor = get<armor_stats>(equip.stats);

		player.add_def(armor.def);
		player.add_magicResist(armor.magicResist);
		player.add_dodgeChance(armor.dodgeChance);

		if (equip.enchants)
		{
			enchantments e = *equip.enchants;
			equip_ItemEnchants(player, e);
		}
	}
	else if (equip.type == itemType::Cape || equip.type == itemType::Necklace)
	{
		if (equip.type == itemType::Cape)
		{
			IDUnequip = player.e_cape;
			player.e_cape = IDEquip;
		}
		else if (equip.type == itemType::Necklace)
		{
			IDUnequip = player.e_necklace;
			player.e_necklace = IDEquip;
		}

		baseItem unequip = itemDatabase[IDUnequip];

		if (unequip.enchants)
		{
			enchantments e = *unequip.enchants;
			unequip_ItemEnchants(player, e);
		}

		if (equip.enchants)
		{
			enchantments e = *equip.enchants;
			equip_ItemEnchants(player, e);
		}
		

	}
	else if (equip.type == itemType::Ring)
	{
		if (ringSlot == 1)
		{
			IDUnequip = player.e_ring1;
			player.e_ring1 = IDEquip;
		}
		else if (ringSlot == 2)
		{
			IDUnequip = player.e_ring2;
			player.e_ring2 = IDEquip;
		}

		baseItem unequip = itemDatabase[IDUnequip];

		if (unequip.enchants)
		{
			enchantments e = *unequip.enchants;
			unequip_ItemEnchants(player, e);
		}

		if (equip.enchants)
		{
			enchantments e = *equip.enchants;
			equip_ItemEnchants(player, e);
		}
	}
}