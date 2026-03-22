#include <iostream>
#include <iomanip>
#include <conio.h>

#include "Player.h"
#include "menu_Status.h"
#include "Utility.h"
#include "item_ID.h"
#include "item_dataBase.h"
using namespace std;


int manage_statusMenu(Player& player)
{
	int status_option{0};

	while (status_option != -1)
	{ 
		system("cls");
		print_statusMenuHeader();

		status_option = statusScreen(player);

		if (status_option == 1)
			statInfo();
	}

	return status_option;
}

void print_statusMenuHeader()
{
	cout << "========================================================================================================================\n						     Status Screen\n"
		"========================================================================================================================\n\n\n";
}

int statusScreen(Player& player)
{
	baseItem e_weapon, e_shield, e_head, e_shoulders, e_cape, e_chest, e_arms, e_legs, e_feet, e_ring1, e_ring2, e_necklace;
	getEquipItem(player, e_weapon, e_shield, e_head, e_shoulders, e_cape, e_chest, e_arms, e_legs, e_feet, e_ring1, e_ring2, e_necklace);
	string xp = to_string(player.get_currentEXP()) + "/" + to_string(player.get_reqEXP());
	string hp = to_string(player.get_HP()) + "/" + to_string(player.get_maxHP());
	string mp = to_string(player.get_MP()) + "/" + to_string(player.get_maxMP());

	Type(" Name: "); cout << player.get_name(); cout << "                                                                             F = Stat Information";
	Type("\n Status: "); cout << player.get_status();
	Type("\n Coins: "); cout << typeColor("Gold") << player.get_gold() << "g " << typeColor("Silver") << player.get_silver() << "s " << typeColor("Copper") << player.get_copper() << "c " << clearColor();

	Type("\n\n-------------------- Character Stats -------------------- | ----------------------- Equipement -----------------------");
	cout << left;
	cout << "\n                                                          |";

	Type("\nLevel: "); cout << setw(3) << player.get_lvl() << "                                                | ";  Type(" Weapon ------ "); cout << get_itemColor(e_weapon) << e_weapon.name << clearColor();
	cout << "\n                                                          |"; Type("  Shield ------ "); cout << get_itemColor(e_shield) << e_shield.name << clearColor();
	Type("\nXP: "); cout << setw(15) << xp << "                                       |";
	cout << "\n                                                          |"; Type("  Head -------- "); cout << get_itemColor(e_head) << e_head.name << clearColor();
	Type("\nHealth: "); cout << setw(9) << hp; Type("   | Health Regen: "); cout << setw(4) << player.get_hpRegen() << "                  |"; Type("  Shoulders --- "); cout << get_itemColor(e_shoulders) << e_shoulders.name << clearColor();
	Type("\nMana: "); cout << setw(9) << mp; Type("     | Mana Regen: "); cout << setw(4) << player.get_mpRegen() << "                    |"; Type("  Chest ------- "); cout << get_itemColor(e_chest) << e_chest.name << clearColor();

	cout << "\n                                                          |"; Type("  Arms -------- "); cout << get_itemColor(e_arms) << e_arms.name << clearColor();
	Type("\nAttack Damage: "); cout << setw(3) << player.get_atk(); Type("  | Spell Power: "); cout << setw(3) << player.get_spellPower(); cout << "                    |"; Type("  Legs -------- "); cout << get_itemColor(e_legs) << e_legs.name << clearColor();
	Type("\nDefense: "); cout << setw(3) << player.get_defense(); Type("        | Magic Resist: "); cout << setw(3) << player.get_magicResist(); cout << "                   |"; Type("  Feet -------- "); cout << get_itemColor(e_feet) << e_feet.name << clearColor();
	cout << "\n                                                          |";

	Type("\nCrit Chance: "); cout << right << setw(3) << player.get_critChance() << "%" << "    | Crit Damage: x" << showpoint << setprecision(2) << player.get_critMult() << noshowpoint; cout << "                  |"; Type("  Cape -------- "); cout << get_itemColor(e_cape) << e_cape.name << clearColor();
	Type("\nHit Chance: "); cout << right <<  setw(3) << player.get_hitChance() << "%"; Type("     | Dodge Chance: "); cout << right << setw(3) << player.get_dodgeChance() << "%"; cout << "                 |"; Type("  Ring 1 ------ "); cout << get_itemColor(e_ring1) << e_ring1.name << clearColor();
	cout << "\n                                                          |"; Type("  Ring 2 ------ "); cout << get_itemColor(e_ring2) << e_ring2.name << clearColor();
	Type("\nLife Steal: "); cout << right << setw(3) << player.get_lifesteal() << "%"; Type("     | Spell Vamp: "); cout << right <<  setw(3) << player.get_spellVamp() << "%" << "                   |"; Type("  Neck -------- "); cout << get_itemColor(e_necklace) << e_necklace.name << clearColor();
	Type("\nThorns: "); cout << left << setw(4) << player.get_thorns() << "                                              |";


	




	utility_clearGetchBuffer(); //Clear any potential BS while outputting info

	int keyPressed = _getch();

	while (keyPressed != 8 && keyPressed != 70 && keyPressed != 102) //if user does not input BS = Do nothing
	{
		keyPressed = _getch();
	}

	if (keyPressed == 70 || keyPressed == 102) //F
	{
		return 1;
	}

	if (keyPressed == 8) //BS
	{
		return -1;
	}
}

void getEquipItem(Player& player, baseItem& e_weapon, baseItem& e_shield, baseItem& e_head, baseItem& e_shoulders, baseItem& e_cape, baseItem& e_chest, baseItem& e_arms, baseItem& e_legs, baseItem& e_feet, baseItem& e_ring1, baseItem& e_ring2, baseItem& e_necklace)
{
	itemID id;

	id = player.get_eWeapon();
	e_weapon = itemDatabase[id];

	id = player.get_eShield();
	e_shield = itemDatabase[id];

	id = player.get_eHead();
	e_head = itemDatabase[id];

	id = player.get_eShoulders();
	e_shoulders = itemDatabase[id];

	id = player.get_eCape();
	e_cape = itemDatabase[id];

	id = player.get_eChest();
	e_chest = itemDatabase[id];

	id = player.get_eArms();
	e_arms = itemDatabase[id];

	id = player.get_eLegs();
	e_legs = itemDatabase[id];

	id = player.get_eFeet();
	e_feet = itemDatabase[id];

	id = player.get_eRing1();
	e_ring1 = itemDatabase[id];

	id = player.get_eRing2();
	e_ring2 = itemDatabase[id];

	id = player.get_eNecklace();
	e_necklace = itemDatabase[id];
}

void statInfo()
{
	system("cls");

	Type("========================================================================================================================\n						     Stat Info\n"
		"========================================================================================================================\n\n\n");

	Type("[Level] Your current character level. Every level up you are rewarded with a stat increase.\n\n");
	Type("[XP] The amount of experience points your character has. Gather enough XP to gain a level up!\n\n");
	cout << typeColor("Health") << "[Health] " << clearColor();
	Type("The amount of"); cout << typeColor("Health") << " health points " << clearColor(); Type("your character currently has. If your "); cout << typeColor("Health") << "health" << clearColor(); Type(" reaches 0, it's game over.\n\n");
	cout << typeColor("Health") << "[Health Regen] " << clearColor();
	Type("The amount of"); cout << typeColor("Health") << " health points" << clearColor(); Type(" your character regenerates every turn during combat.\n\n");
	cout << typeColor("Mana") << "[Mana] " << clearColor();
	Type("The amount of"); cout << typeColor("Mana") << " mana points " << clearColor(); Type("your character currently has. "); cout << typeColor("Mana") << "Mana " << clearColor(); Type("is used to cast skills.\n\n");
	cout << typeColor("Mana") << "[Mana Regen] " << clearColor();
	Type("The amount of "); cout << typeColor("Mana") << "mana points" << clearColor(); Type(" your character regenerates every turn during combat.\n\n");
	cout << typeColor("Physical") << "[Attack Damage] " << clearColor();
	Type("The amount of "); cout << typeColor("Physical") << "physical power" << clearColor(); Type(" your character has. Relevant for"); cout << typeColor("Physical") << " physical skills" << clearColor() << ".\n\n";
	cout << typeColor("Magical") << "[Spell Power] " << clearColor();
	Type("The amount of"); cout << typeColor("Magical") << " magical power" << clearColor(); Type(" your character has. Relevant for"); cout << typeColor("Magical") << " magical skills" << clearColor() << ".\n\n";
	Type("[Defense] The amount of"); cout << typeColor("Physical") << " physical damage" << clearColor(); Type(" your character blocks.\n\n");
	Type("[Magic Resist] The amount of"); cout << typeColor("Magical") << " magical damage" << clearColor(); Type(" your character blocks.\n\n");
	cout << typeColor("Critical") << "[Critical Chance] " << clearColor();
	Type("Represents how likely your character will land a "); cout << typeColor("Critical") << "critical hit" << clearColor() << ".\n                  " << typeColor("Critical") << "Critical hits" << clearColor(); Type(" only apply to physical attacks.\n\n");
	cout << typeColor("Critical") << "[Critical Damage] " << clearColor();
	Type("Multiplier that represents how much damage your "); cout << typeColor("Critical") << "critical hits" << clearColor(); Type(" will inflict.\n\n");
	Type("[Hit Chance] How likely you are to perform a succesful"); cout << typeColor("Physical") << " physical attack" << clearColor(); Type(" to an enemy.\n\n");
	Type("[Dodge Chance] How likely you are to dodge an enemy's"); cout << typeColor("Physical") << " physical attack" << clearColor() << ".\n\n";
	Type("[Life Steal] A percentage that represents how much"); cout << typeColor("Health") << " health" << clearColor(); Type(" you get back when dealing"); cout << typeColor("Physical") << " physical damage" << clearColor() << ".\n\n";
	Type("[Spell Vamp] A percentage that represents how much"); cout << typeColor("Health") << " health" << clearColor(); Type(" you get back when dealing"); cout << typeColor("Magical") << " magical damage" << clearColor() << ".\n\n";
	cout << typeColor("Thorns") << "[Thorns] " << clearColor();
	Type("The amount of damage you deal when an enemy strikes you."); cout << typeColor("Thorns") << " Thorns " << clearColor(); Type("only work against"); cout << typeColor("Physical") << " physical attacks" << clearColor() << ".\n\n";


	int keyPressed = _getch();

	while (keyPressed != 8)
		keyPressed = _getch();
}