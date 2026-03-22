#pragma once
#pragma warning (disable:26495)

#include <vector>
#include <string>

#include "item_ID.h"
#include "skill_ID.h"
#include "map_ID.h"
#include "buffs.h"
using namespace std;

class Player {
private:

	int saveSlot{ 0 };

protected:

	//Stats
	string name;
	string status;

	int lvl{ 0 };
	int requiredXP{ 0 };
	int currentXP{ 0 };

	int maxHP{ 0 };
	int currentHP{ 0 };

	int maxMP{ 0 };
	int currentMP{ 0 };

	int hpRegen{ 0 };
	int mpRegen{ 0 };

	int atk{ 0 };
	int spellPower{ 0 };
	int defense{ 0 };
	int magicResist{ 0 };

	int critChance{ 0 };
	double critMult{ 0 }; //Multiplier (atk * critDMG)

	int hitChance{ 0 }; //player.hitChance - enemy.dodgeChance = final hit chance
	int dodgeChance{ 0 };

	int lifesteal{ 0 };//Lifesteal for damaging skills of type Physical
	int spellVamp{ 0 }; //Lifesteal for damaging skills of type Spell

	int thorns{ 0 }; //Inflicts physical damage (reduceable by enemy defense) when struck by physical skills
	
	//Money
	int copper{ 0 };
	int silver{ 0 };
	int gold{ 0 };

public:

	vector<itemID> shop{}; //shop inventory
	vector<int> shopQTY{}; //vector containing quantity of items available in shop inventory
	vector<itemID> shop2{}; //page 2
	vector<int> shopQTY2{};
	vector<itemID> shop3{}; //page 3
	vector<int> shopQTY3{};

	int fightCount{ 0 }; //Tracked/Used for shop reroll
	bool stunned{false}; //is player stunned (used for combat)

	//Player buffs and debuffs, with their corresponding parallel timer vector
	vector<buff> pbuffs{};
	vector<int> pbuffsTimer{};

	vector<debuff> pdebuffs{};
	vector<int> pdebuffsTimer{};

	//Player skill cooldown timer 
	vector<skillID> CDskill{};
	vector<int> CDskillTimer{};

	//Unlocked maps by character
	vector<mapID> mapsUnlocked; //Public for easier access with Load/Save system

	//Get mapsUnlocked size
	int get_mapsUnlockedSize()
	{
		int count{ 0 };

		for (mapID check : mapsUnlocked)
			count++;

		return count;
	}


	//Equipped Gear
	itemID e_weapon{ 0 };
	itemID e_shield{ 0 };

	itemID e_head{ 0 };
	itemID e_shoulders{ 0 };
	itemID e_chest{ 0 };
	itemID e_arms{ 0 };
	itemID e_legs{ 0 };
	itemID e_feet{ 0 };

	itemID e_cape{ 0 };
	itemID e_ring1{ 0 };
	itemID e_ring2{ 0 };
	itemID e_necklace{ 0 };

	//Inventory
	itemID inventory[20];
	int inventorySize{20}; //for modularity in case I want to change inventory size of array later
	int inventory_QTY[20]; //Parallel to inventory array, tracks quantity of items held by player

	//Skills Inventory
	vector<skillID> skills;

	Player() {}; //Dummy Constructor - Called in int main(), allows to pass player object around program

	//New Save constructor
	Player(string n, int s)
	{
		saveSlot = s;

		name = n;
		status = "Alive";

		lvl = 1;

		requiredXP = 15;

		maxHP = 10;
		currentHP = 10;

		maxMP = 5;
		currentMP = 5;

		atk = 1; //Unarmed

		critMult = 2.0; //2x damage by default
		hitChance = 80;
		dodgeChance = 10; // 10% chance by default

		mapsUnlocked.push_back(mapID::Meadows); //Meadows Map unlocked by default


		//Create empty equipement slots

		e_weapon = itemID::None;
		e_shield = itemID::None;

		e_head = itemID::None;
		e_shoulders = itemID::None;
		e_chest = itemID::None;
		e_arms = itemID::None;
		e_legs = itemID::None;
		e_feet = itemID::None;

		e_cape = itemID::None;
		e_ring1 = itemID::None;
		e_ring2 = itemID::None;
		e_necklace = itemID::None;

		//Create empty inventory

		for (int i{ 0 }; i < inventorySize; i++)
		{
			inventory[i] = itemID::None;
			inventory_QTY[i] = 0;
		}

		//Add Attack skill to player
		skills.push_back(skillID::Attack);
	}


	//Existing save constructor - Stats
	Player(int saveSlot, string name, string status, int lvl, int requiredXP, int currentXP, int maxHP, int currentHP, int maxMP, int currentMP, int hpRegen, int mpRegen,
		int atk, int spellPower, int defense, int magicResist, int lifesteal, int spellVamp, int critChance, double critMult, int hitChance, int dodgeChance, int thorns, int copper, int silver, int gold,
	    int fightCount)
	{
		this->saveSlot = saveSlot;
		this->name = name;
		this->status = status;
		this->lvl = lvl;
		this->requiredXP = requiredXP;
		this->currentXP = currentXP;
		this->maxHP = maxHP;
		this->currentHP = currentHP;
		this->maxMP = maxMP;
		this->currentMP = currentMP;
		this->hpRegen = hpRegen; 
		this->mpRegen = mpRegen;
		this->atk = atk;
		this->spellPower = spellPower;
		this->defense = defense;
		this->magicResist = magicResist;
		this->lifesteal = lifesteal;
		this->spellVamp = spellVamp;
		this->critChance = critChance;
		this->critMult = critMult;
		this->hitChance = hitChance;
		this->dodgeChance = dodgeChance;
		this->thorns = thorns;
		this->copper = copper;
		this->silver = silver;
		this->gold = gold;
		this->fightCount = fightCount;
	}


	//Getters - Stats
	int get_saveSlot() { return saveSlot; }

	string get_status() { return status; }
	string get_name() { return name; }

	int get_lvl() { return lvl; }
	int get_reqEXP() { return requiredXP; }
	int get_currentEXP() { return currentXP; }

	int get_maxHP() { return maxHP; }
	int get_HP() { return currentHP; }

	int get_maxMP() { return maxMP; }
	int get_MP() { return currentMP; }

	int get_hpRegen() { return hpRegen; }
	int get_mpRegen() { return mpRegen; }

	int get_atk() { return atk; }
	int get_spellPower() { return spellPower; }
	int get_defense() { return defense; }
	int get_magicResist() { return magicResist; }

	int get_critChance() { return critChance; }
	double get_critMult() { return critMult; }
	int get_hitChance() { return hitChance; }
	int get_dodgeChance() { return dodgeChance; }

	int get_lifesteal() { return lifesteal; }
	int get_spellVamp() { return spellVamp; }

	int get_thorns() { return thorns; }

	int get_copper() { return copper; }
	int get_silver() { return silver; }
	int get_gold() { return gold; }


	//Getters - Equipement
	itemID get_eWeapon() { return e_weapon; }

	itemID get_eShield() { return e_shield; }

	itemID get_eHead() { return e_head; }

	itemID get_eShoulders() { return e_shoulders; }

	itemID get_eChest() { return e_chest; }

	itemID get_eArms() { return e_arms; }

	itemID get_eLegs() { return e_legs; }

	itemID get_eFeet() { return e_feet; }

	itemID get_eCape() { return e_cape; }

	itemID get_eRing1() { return e_ring1; }

	itemID get_eRing2() { return e_ring2; }

	itemID get_eNecklace() { return e_necklace; }


	//Setters - Stats
	void add_lvl(int n) { lvl += n; }

	void set_requiredXP(int n) { requiredXP = n; }
	void set_currentXP(int n) { currentXP = n; }
	void add_currentXP(int n) { currentXP += n; }

	void set_maxHP(int n) { maxHP = n; }
	void add_maxHP(int n) { maxHP += n; }
	void sub_maxHP(int n) { maxHP -= n; }

	void set_HP(int n) { currentHP = n; }
	void add_HP(int n) { currentHP += n; }
	void sub_HP(int n) { currentHP -= n; }

	void set_maxMP(int n) { maxMP = n; }
	void add_maxMP(int n) { maxMP += n; }
	void sub_maxMP(int n) { maxMP -= n; }

	void set_MP(int n) { currentMP = n; }
	void add_MP(int n) { currentMP += n; }
	void sub_MP(int n) { currentMP -= n; }

	void add_hpRegen(int n) { hpRegen += n; }
	void sub_hpRegen(int n) { hpRegen -= n; }

	void add_mpRegen(int n) { mpRegen += n; }
	void sub_mpRegen(int n) { mpRegen -= n; }

	void add_atk(int n) { atk += n; }
	void sub_atk(int n) { atk -= n; }

	void add_spellPower(int n) { spellPower += n; }
	void sub_spellPower(int n) { spellPower -= n; }

	void add_def(int n) { defense += n; }
	void sub_def(int n) { defense -= n; }

	void add_magicResist(int n) { magicResist += n; }
	void sub_magicResist(int n) { magicResist -= n; }

	void add_critChance(int n) { critChance += n; }
	void sub_critChance(int n) { critChance -= n; }

	void add_critMult(double n) { critMult += n; }
	void sub_critMult(double n) { critMult -= n; }

	void add_hitChance(int n) { hitChance += n; }
	void sub_hitChance(int n) { hitChance -= n; }

	void add_dodgeChance(int n) { dodgeChance += n; }
	void sub_dodgeChance(int n) { dodgeChance -= n; }

	void add_lifesteal(int n) { lifesteal += n; }
	void sub_lifesteal(int n) { lifesteal -= n; }

	void add_spellVamp(int n) { spellVamp += n; }
	void sub_spellVamp(int n) { spellVamp -= n; }
	
	void add_thorns(int n) { thorns += n; }
	void sub_thorns(int n) { thorns -= n; }

	//Setters - Money
	void add_copper(int n) { copper += n; }
	void sub_copper(int n) { copper -= n; }
	void set_copper(int n) { copper = n; }

	void add_silver(int n) { silver += n; }
	void sub_silver(int n) { silver -= n; }
	void set_silver(int n) { silver = n; }

	void add_gold(int n) { gold += n; }
	void sub_gold(int n) { gold -= n; }
	void set_gold(int n) { gold = n; }
};