#pragma once

#include <iostream>
#include <vector>

#include "item_ID.h"
#include "Player.h"
#include "Utility.h"
#include "buffs.h"
using namespace std;

class baseMob
{
protected:

	string name{ "" };
	string mobType{ "" }; // Normal || Elite || Boss

	int lvl{ 0 };
	int xp{ 0 }; //experience the player will receive upon beating the mob

	int maxHP{ 0 };
	int hp{ 0 };

	int hpRegen{ 0 };

	int atk{ 0 };
	int spellPower{ 0 };

	int def{ 0 };
	int magicResist{ 0 };

	int critChance{ 0 };
	double critMult{ 2 };

	int hitChance{ 80 }; //Default
	int dodgeChance{ 10 }; //Default

	int lifesteal{ 0 };
	int spellVamp{ 0 };

	int thorns{ 0 };

	//Money Drop
	int copper{ 0 };
	int silver{ 0 };
	int gold{ 0 };

	//Item Loot
	itemID loot;
	int lootChance{ 0 };

public:

	//Combat
	virtual void takeTurn(Player& player, vector<string>& logs, int& turnCount) {}; //To access inherited objects logic
	virtual ~baseMob() {}; //Deconstructor

	bool stunned{ false }; //is mob currently stunned

	bool checkMiss(Player& player, vector<string>& logs)
	{
		int threshold = get_hitChance() - player.get_dodgeChance();

		int roll = rand() % 100 + 1;

		if (roll <= threshold)
			return true;
		else
			return false;
	}

	bool rollCrit(int n) //n being if an enemy's skill has extra crit chance i.e Deep Cuts
	{
		int roll = rand() % 100 + 1;

		if (roll <= critChance + n)
			return true;
		else
			return false;
	}

	void basicAttack(Player& player, vector<string>& logs) //default physical attack
	{
		bool hit = checkMiss(player, logs);

		if (hit == true)
		{
			int dmg = get_atk();
			bool crit = rollCrit(0);

			if (crit == true)
				dmg *= critMult;

			dmg -= player.get_defense();

			if (dmg < 1)
				dmg = 1;

			player.sub_HP(dmg);

			string log;

			if (crit == true)
				log = get_name() + " attacks and deals " + typeColor("Critical") + to_string(dmg) + typeColor("Physical") + " physical damage" + clearColor() + "!";
			else
				log = get_name() + " attacks and deals " + to_string(dmg) + typeColor("Physical") + " physical damage" + clearColor() + "!";

			logs.push_back(log);

			checkLifesteal(dmg, player, logs);
			checkThorns(player, logs);
		}
		else
		{
			string log = get_name() + " misses!";
			logs.push_back(log);
		}
	}

	void checkLifesteal(int& dmg, Player& player, vector<string>& logs) 
	{
		if (lifesteal > 0)
		{
			double mult = lifesteal / 100.0;
			int lifesteal = dmg * mult;

			if (lifesteal == 0)
				lifesteal = 1;

			updateLogs(player, logs);

			add_HP(lifesteal);
			string log = get_name() + " restores " + typeColor("Health") + to_string(lifesteal) + " health" + clearColor() + "!";
			logs.push_back(log);
			
		}
	}

	void checkSpellvamp(int& dmg, Player& player, vector<string>& logs) 
	{
		if (spellVamp > 0)
		{
			double mult = spellVamp / 100.0;
			int spellVamp = dmg * mult;

			if (spellVamp == 0)
				spellVamp = 1;

			updateLogs(player, logs);

			add_HP(spellVamp);
			string log = get_name() + " restores " + typeColor("Health") + to_string(spellVamp) + " health" + clearColor() + "!";
			logs.push_back(log);
		}
	}

	void checkThorns(Player& player, vector<string>& logs) //check after physical attack lands (dont check if attack misses)
	{
		if (player.get_thorns() > 0)
		{
			sub_HP(player.get_thorns());

			updateLogs(player, logs);

			string log = player.get_name() + " reflects " + to_string(player.get_thorns()) + typeColor("Thorns") + " thorns damage" + clearColor() + "!";
			logs.push_back(log);
		}
	}

	string mobSkillColor(string s) //Used for logs when mob uses a skill
	{
		if (s == "Uncommon")
			return "\x1b[32m";
		else if (s == "Rare")
			return "\x1b[34m";
		else if (s == "Epic")
			return "\x1b[35m";
		else if (s == "Legendary")
			return "\x1b[33m";
	}

	void updateLogs(Player& player, vector<string>& logs) //update logs needs to be available for mobs aswell for certain contexts
	{
		if (logs.size() >= 15)
		{
			for (int i{ 0 }; i < 14; i++)
			{
				logs[i] = logs[i + 1];
			}

			logs.resize(14); //resize to allow new log to be pushback at index 15
		}

		system("cls");

		cout << "########################################################################################################################\n\n"
			"--- BATTLE LOGS ---\n\n";

		for (string check : logs)
		{
			cout << check << endl;
		}

		int size = logs.size();

		for (int i{ size }; i < 15; i++)
			cout << endl;

		cout << "\n\n--- BATTLE MENU ---\n\n";

		for (int i{ 0 }; i < 4; i++)
		{
			cout << "  ";

			if (i == 0)
				cout << "Attack";
			if (i == 1)
				cout << "Skills";
			if (i == 2)
				cout << "Potions";
			if (i == 3)
				cout << "Status";

			cout << endl;
		}

		waitS(1);
	}

	void erasePlayerDebuffs(Player& player) //used by enemy
	{
		for (int i{ 0 }; i < player.pdebuffs.size(); i++)
		{
			debuff d = player.pdebuffs[i];

			player.add_atk(d.weak);
			player.add_spellPower(d.nullify);
			player.add_def(d.frail);
			player.add_magicResist(d.shatter);
			player.add_hitChance(d.blind);
			player.add_dodgeChance(d.immobile);
		}

		player.pdebuffs.clear();
		player.pdebuffsTimer.clear();
	}

	//Getters -------------------

	string get_name() { return name; }
	string get_mobType() { return mobType; }

	int get_LVL() { return lvl; }
	int get_XP() { return xp; }

	int get_maxHP() { return maxHP; }
	int get_HP() { return hp; }
	int get_hpRegen() { return hpRegen; }

	int get_atk() { return atk; }
	int get_spellPower() { return spellPower; }
	int get_defense() { return def; }
	int get_magicResist() { return magicResist; }

	int get_critChance() { return critChance; }
	int get_critMult() { return critMult; }

	int get_hitChance() { return hitChance; }
	int get_dodgeChance() { return dodgeChance; }

	int get_lifesteal() { return lifesteal; }
	int get_spellVamp() { return spellVamp; }

	int get_thorns() { return thorns; }

	int get_copper() { return copper; }
	int get_silver() { return silver; }
	int get_gold() { return gold; }

	itemID get_loot() { return loot; }
	int get_lootChance() { return lootChance; }

	//Setters -------------------
	
	void add_maxHP(int n) { maxHP += n; }
	void sub_maxHP(int n) { maxHP -= n; }

	
	void add_HP(int n) { hp += n; }
	void sub_HP(int n) { hp -= n; }
	void set_HP(int n) { hp = n; }

	void add_hpRegen(int n) { hpRegen += n; }
	void sub_hpRegen(int n) { hpRegen -= n; }

	void add_atk(int n) { atk += n; }
	void sub_atk(int n) { atk -= n; }

	void add_spellPower(int n) { spellPower += n; }
	void sub_spellPower(int n) { spellPower -= n; }

	void add_def(int n) { def += n; }
	void sub_def(int n) { def -= n; }

	void add_magicResist(int n) { magicResist += n; }
	void sub_magicResist(int n) { magicResist -= n; }

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

};

class Rat:public baseMob
{
public:
	Rat()
	{
		name = "Rat"; 
		mobType = "Normal";

		lvl = rand() % 2 + 1; //Can only be between levels 1 to 2

		xp = lvl;

		maxHP = 1 + (lvl * 2); // 3hp / 5hp
		hp = maxHP;

		atk = 1;

		hitChance = 60;

		copper = lvl;

		//Item Loot
		loot = itemID::RatTail;

		if (lvl == 1)
			lootChance = 33;
		else if (lvl == 2)
			lootChance = 66;
	}

	void takeTurn(Player& player, vector<string>& logs, int& turnCount) override
	{
		basicAttack(player, logs);
	}
};

class Boar:public baseMob
{
public:
	Boar()
	{
		name = "Boar";
		mobType = "Normal";

		lvl = rand() % 2 + 3; //Can only be between levels 3 to 4

		xp = lvl;

		maxHP = 2 + (lvl * 2); // 8hp / 10hp
		hp = maxHP;

		atk = lvl;
		def = 1;

		dodgeChance = 5;

		copper = lvl;

		//Item Loot
		loot = itemID::BoarSnout;
		
		if (lvl == 3)
			lootChance = 33;
		else if (lvl == 4)
			lootChance = 66;
	}

	void takeTurn(Player& player, vector<string>& logs, int& turnCount) override
	{
		basicAttack(player, logs);
	
	}
};

class ElderTusk :public baseMob
{
public:
	bool hasRam{ true }; //one time use ability. deals more damage and stuns

	ElderTusk()
	{
		name = "Elder Tusk";
		mobType = "Boss";

		lvl = 5;

		xp = 30;

		maxHP = 35;
		hp = maxHP;

		atk = 9;
		def = 2;

		dodgeChance = 5;

		copper = 25;

		//Item Loot
		loot = itemID::BoarSnout;
		lootChance = 100;
	}

	void takeTurn(Player& player, vector<string>& logs, int& turnCount) override
	{
		if (hasRam == true)
		{
			int roll = rand() % 100 + 1;

			if (roll <= 30)
			{
				Ram(player, logs);
				hasRam = false;
				return;
			}
		}
			
		basicAttack(player, logs);
	}

	void Ram(Player& player, vector<string>& logs) //Deals more damage and stuns player. (One time use)
	{
		int dmg = (get_atk() + 3) - player.get_defense();

		if (dmg < 1)
			dmg = 1;

		
		player.sub_HP(dmg);
		string log = get_name() + " uses " + mobSkillColor("Uncommon") + "Ram" + clearColor() + " and deals " + to_string(dmg) + typeColor("Physical") + " physical damage" + clearColor() + "!";
		logs.push_back(log);
		updateLogs(player, logs);

		player.stunned = true;

		checkThorns(player, logs);
	}
};

// ----- DARK FOREST -------------------------------------------------------------------------------------------------------------------

class Wolf :public baseMob
{
public:
	bool hasHowl{ true };

	Wolf()
	{
		name = "Wolf";
		mobType = "Normal";

		lvl = rand() % 2 + 6; //levels 6 and 7

		xp = lvl;

		if (lvl == 6)
			maxHP = 16;
		else if (lvl == 7)
			maxHP = 20;
			
		hp = maxHP;

		if (lvl == 6)
			atk = 9;
		else if (lvl == 7)
			atk = 10;


		if (lvl == 6)
			def = 2;
		else if (lvl == 7)
			def = 3;

		copper = lvl;

		//Item Loot
		loot = itemID::WolfPelt;

		if (lvl == 6)
			lootChance = 33;
		else if (lvl == 7)
			lootChance = 66;
	}

	void takeTurn(Player& player, vector<string>& logs, int& turnCount) override
	{
		if (hasHowl == true) //the longer the fight lasts, the more likely wolf is gonna cast Howl
		{
			int roll = rand() % 100 + 1;

			if (turnCount == 3 && roll <= 25)
			{
				Howl(player, logs);
				hasHowl = false;
				return;
			}
			else if (turnCount == 4 && roll <= 50)
			{
				Howl(player, logs);
				hasHowl = false;
				return;
			}
			else if (turnCount >= 5)
			{
				Howl(player, logs);
				hasHowl = false;
				return;
			}
		}

		basicAttack(player, logs);
	}

	void Howl(Player& player, vector<string>& logs) //Buffs attack by 2 for remainder of combat
	{
		int increase{ 2 };
		atk += increase;

		string log = get_name() + " uses" + mobSkillColor("Uncommon") + " Howl" + clearColor() + " and gains " + to_string(increase) + typeColor("Physical") + " attack power" + clearColor() + "!";
		logs.push_back(log);
		updateLogs(player, logs);
	}

};

class Highwayman :public baseMob 
{
public:
	Highwayman()
	{
		name = "Highwayman";
		mobType = "Normal";

		lvl = rand() % 2 + 8; //levels 8 and 9

		xp = lvl;

		if (lvl == 8)
			maxHP = 26;
		else if (lvl == 9)
			maxHP = 32;

		hp = maxHP;

		if (lvl == 8)
			atk = 12;
		else if (lvl == 9)
			atk = 13;

		if (lvl == 8)
			def = 2;
		else if (lvl == 9)
			def = 3;

		dodgeChance = 20;
		hitChance = 85;

		critChance = 15;

		copper = lvl+3; //they drop a bit more money cuz they r thieves

		//Item Loot
		loot = itemID::BrokenDagger;

		if (lvl == 8)
			lootChance = 33;
		else if (lvl == 9)
			lootChance = 66;
	}

	void takeTurn(Player& player, vector<string>& logs, int& turnCount) override
	{
		int roll = rand() % 100 + 1;

		if (roll <= 15)
		{
			TwinStrikes(player, logs);
			return;
		}
		
		basicAttack(player, logs);
	}

	void TwinStrikes(Player& player, vector<string>& logs) //attacks twice, each strike has increased crit chance.
	{

		string log = get_name() + " uses" + mobSkillColor("Uncommon") + " Twin Strikes " + clearColor() + "and attacks twice!";
		logs.push_back(log);

		updateLogs(player, logs);

		bool hit = checkMiss(player, logs);

		if (hit == true)
		{
			int dmg = atk;
			bool crit = rollCrit(35);

			if (crit == true)
				dmg *= critMult;

			dmg -= player.get_defense();

			if (dmg < 1)
				dmg = 1;

			player.sub_HP(dmg);

			if (crit == true)
				log = name + "'s first attack deals " + typeColor("Critical") + to_string(dmg) + typeColor("Physical") + " physical damage" + clearColor() + "!";
			else
				log = name + "'s first attack deals " + to_string(dmg) + typeColor("Physical") + " physical damage" + clearColor() + "!";

			logs.push_back(log);
			updateLogs(player, logs);

			checkThorns(player, logs);
		}
		else
		{
			log = get_name() + " misses the first attack!";
			logs.push_back(log);

			updateLogs(player, logs);
		}

		hit = checkMiss(player, logs); //Second hit
		
		if (hit == true)
		{
			int dmg = atk;
			bool crit = rollCrit(35);

			if (crit == true)
				dmg *= critMult;

			dmg -= player.get_defense();

			if (dmg <= 1)
				dmg = 1;

			player.sub_HP(dmg);

			if (crit == true)
				log = name + "'s second attack deals " + typeColor("Critical") + to_string(dmg) + typeColor("Physical") + " physical damage" + clearColor() + "!";
			else
				log = name + "'s second attack deals " + to_string(dmg) + typeColor("Physical") + " physical damage" + clearColor() + "!";

			logs.push_back(log);

			updateLogs(player, logs);

			checkThorns(player, logs);
		}
		else
		{
			log = get_name() + " misses the second attack!";
			logs.push_back(log);

			updateLogs(player, logs);
		}
	}
};

class HighwaymanCaptain :public baseMob
{
public:
	bool hasEnchant{ false }; //Boss will enchant his weapons after x turn, hasEnchant will become true, and all subsequent attacks will be enchanted (dealing extra magic damage)
	bool hasCunning{ true }; //Improves critical chance for rest of combat
	bool hasDeepCut{ true }; //applies bleed to player

	HighwaymanCaptain()
	{
		name = "Highwayman Captain";
		mobType = "Boss";

		lvl = 10;

		xp = 75;

		maxHP = 80;
		hp = maxHP;

		atk = 15;

		def = 4;
		magicResist = 2;

		dodgeChance = 20;
		hitChance = 85;

		critChance = 15;

		silver = 1;

		//Item Loot
		loot = itemID::BrokenDagger;

		lootChance = 100;
	}

	void takeTurn(Player& player, vector<string>& logs, int& turnCount) override
	{
		int roll = rand() % 100 + 1;

		if (roll <= 20 && hasDeepCut == true)
		{
			DeepCut(player, logs);
			return;
		}
		else if (hasDeepCut == true && turnCount >= 5)
		{
			DeepCut(player, logs);
			return;
		}
			
		if (roll <= 60 && hasDeepCut == false && turnCount >= 7 && hasCunning == true)
		{
			Cunning(player, logs);
			hasCunning = false;
			return;
		}

		if (hasCunning == false && hasDeepCut == false && hasEnchant == false && hp <= maxHP/2) //final stage at half hp, when all other skills casted already
		{
			EnchantWeapon(player, logs);
			hasEnchant = true;
			return;
		}

		if (hasEnchant == true)
		{
			EnchantedStrike(player, logs);
			return;
		}

		basicAttack(player, logs);
	}

	void DeepCut(Player& player, vector<string>& logs) //Attack guarantees crit + applies bleed debuff onto player
	{
		bool hit = checkMiss(player, logs);
		string log;

		if (hit == true)
		{
			debuff d{ .bleed = 2 };
			player.pdebuffs.push_back(d);
			player.pdebuffsTimer.push_back(5);

			int dmg = atk * critMult;

			dmg -= player.get_defense();

			if (dmg < 1)
				dmg = 1;

			player.sub_HP(dmg);

			log = name + " uses " + mobSkillColor("Uncommon") + "Deep Cut" + clearColor() + " and deals " + typeColor("Critical") + to_string(dmg) + typeColor("Physical") + " physical damage" + clearColor() + "!";
			logs.push_back(log);

			updateLogs(player, logs);


			log = player.get_name() + " is " + typeColor("Bleed") + "bleeding" + clearColor() + "!";
			logs.push_back(log);

			updateLogs(player, logs);

			checkThorns(player, logs);

			hasDeepCut = false;
		}
		else
		{

			log = name + " uses " + mobSkillColor("Uncommon") + "Deep Cut" + clearColor() + " but misses!";
			logs.push_back(log);

			updateLogs(player, logs);

			hasDeepCut = true;
		}
	}

	void Cunning(Player& player, vector<string>& logs)
	{
		critChance += 15;

		updateLogs(player, logs);

		string log = name + " uses " + mobSkillColor("Uncommon") + "Cunning " + clearColor() + "and gains " + typeColor("Critical") + "15% Critical Chance" + clearColor() + "!";
		logs.push_back(log);

		updateLogs(player, logs);
	}

	void EnchantWeapon(Player& player, vector<string>& logs)
	{
		updateLogs(player, logs);

		string log = name + " uses " + mobSkillColor("Uncommon") + "Enchant Weapon " + clearColor() + "and imbues his daggers with" + typeColor("Magical") + " magical powers" + clearColor() + "!";
		logs.push_back(log); 

		updateLogs(player, logs);
	}

	void EnchantedStrike(Player& player, vector<string>& logs)
	{
		bool hit = checkMiss(player, logs);

		if (hit == true)
		{
			int Pdmg = atk;
			int Mdmg = 3;
			bool crit = rollCrit(0);

			if (crit == true)
				Pdmg *= critMult;

			Pdmg -= player.get_defense();
			Mdmg -= player.get_magicResist();

			if (Pdmg < 1)
				Pdmg = 1;

			if (Mdmg < 1)
				Mdmg = 1;

			player.sub_HP(Pdmg + Mdmg);

			string log;

			if (crit == true)
			{
				log = name + " uses " + mobSkillColor("Uncommon") + "Enchanted Strike " + clearColor() + "and deals " + typeColor("Critical") + to_string(Pdmg) + typeColor("Physical") +
					" physical damage" + clearColor() + " and " + to_string(Mdmg) + typeColor("Magical") + " magical damage" + clearColor() + "!";
			}
			else
			{
				log = log = name + " uses " + mobSkillColor("Uncommon") + " Enchanted Strike " + clearColor() + "and deals " + to_string(Pdmg) + typeColor("Physical") +
					" physical damage" + clearColor() + " and " + to_string(Mdmg) + typeColor("Magical") + " magical damage" + clearColor() + "!";
			}

			logs.push_back(log);

			updateLogs(player, logs);
		}
		else
		{
			updateLogs(player, logs);

			string log = name + " uses " + mobSkillColor("Uncommon") + "Enchanted Strike " + clearColor() + "but misses!";
			logs.push_back(log);

			updateLogs(player, logs);
		}
	}
};

	// ----- UNDEAD CATACOMBS -------------------------------------------------------------------------------------------------------------------


class SkeletonWarrior :public baseMob
{
public:
	bool hasTerrify{ true }; //reduce player atk and spellPower

	SkeletonWarrior()
	{
		name = "Skeleton Warrior";
		mobType = "Normal";

		lvl = rand() % 2 + 11;

		xp = lvl;

		if (lvl == 11)
			maxHP = 50;
		else if (lvl == 12)
			maxHP = 60;

		hp = maxHP;

		if (lvl == 11)
			atk = 18;
		else if (lvl == 12)
			atk = 20;

		if (lvl == 11)
			def = 3;
		else if (lvl == 12)
			def = 4;

		if (lvl == 11)
			magicResist = 2;
		else if (lvl == 12)
			magicResist = 3;

		copper = lvl; 

		//Item Loot
		loot = itemID::BoneAshes;

		if (lvl == 11)
			lootChance = 33;
		else if (lvl == 12)
			lootChance = 66;
	}

	void takeTurn(Player& player, vector<string>& logs, int& turnCount) override
	{
		int roll = rand() % 100 + 1;

		if (turnCount < 4 && roll <= 20 && hasTerrify == true)
		{
			Terrify(player, logs);
			return;
		}
		else if (turnCount >= 4 && hasTerrify == true)
		{
			Terrify(player, logs);
			return;
		}

		basicAttack(player, logs);
	
	}

	void Terrify(Player& player, vector<string>& logs)
	{
		hasTerrify = false;

		debuff d{ .weak = 3, .shatter = 3 };
		int turns{ 10 };
		
		player.pdebuffs.push_back(d);
		player.pdebuffsTimer.push_back(turns);

		updateLogs(player, logs);

		string log = name + " uses " + mobSkillColor("Uncommon") + "Terrify" + clearColor() + "!";
		logs.push_back(log);

		updateLogs(player, logs);

		log = player.get_name() + " loses " + to_string(d.weak) + typeColor("Physical") + " attack power " + clearColor() + "and " + to_string(d.shatter) + typeColor("Magical") + " magical power " + clearColor() + "for "
			+ to_string(turns) + " turns!";
		logs.push_back(log);
	
		updateLogs(player, logs);
	}
};

class SkeletonArcher :public baseMob
{
public:
	SkeletonArcher()
	{
		name = "Skeleton Archer";
		mobType = "Normal";

		lvl = rand() % 2 + 11;

		xp = lvl;

		if (lvl == 11)
			maxHP = 40;
		else if (lvl == 12)
			maxHP = 45;

		hp = maxHP;

		if (lvl == 11)
			atk = 22;
		else if (lvl == 12)
			atk = 24;

		if (lvl == 11)
			def = 2;
		else if (lvl == 12)
			def = 3;

		if (lvl == 11)
			magicResist = 1;
		else if (lvl == 12)
			magicResist = 2;

		critChance = 15;

		dodgeChance = 15;
		hitChance = 95; //oh they're gonna hit alright

		copper = lvl;

		//Item Loot
		loot = itemID::BoneAshes;

		if (lvl == 11)
			lootChance = 33;
		else if (lvl == 12)
			lootChance = 66;
	}

	void takeTurn(Player& player, vector<string>& logs, int& turnCount) override
	{
		int roll = rand() % 100 + 1;

		if (roll <= 15)
		{
			PiercingArrow(player, logs);
			return;
		}

		bool hit = checkMiss(player, logs);

		
		basicAttack(player, logs);
	}

	void PiercingArrow(Player& player, vector<string>& logs) //Attack that ignores half of player's defense
	{
		bool hit = checkMiss(player, logs);

		if (hit == true)
		{
			int dmg{ atk };
			bool crit = rollCrit(0);

			if (crit == true)
				dmg *= critMult;

			dmg -= player.get_defense() / 2;

			if (dmg < 1)
				dmg = 1;

			player.sub_HP(dmg);

			string log;

			if (crit == true)
				log = name + " uses " + mobSkillColor("Uncommon") + "Piercing Arrow " + clearColor() + "and deals " + typeColor("Critical") + to_string(dmg) + typeColor("Physical") + " physical damage" + clearColor() + "!";
			else
				log = name + " uses " + mobSkillColor("Uncommon") + "Piercing Arrow " + clearColor() + "and deals " + to_string(dmg) + typeColor("Physical") + " physical damage" + clearColor() + "!";

			logs.push_back(log);
			updateLogs(player, logs);
		}
		else
		{
			string log = name + " uses " + mobSkillColor("Uncommon") + "Piercing Arrow " + clearColor() + "but misses!";
			logs.push_back(log);
			updateLogs(player, logs);
		}
	}
};

class Ghoul :public baseMob
{
public:
	bool hasPutridBreath{ true }; //apply poison

	Ghoul()
	{
		name = "Ghoul";
		mobType = "Normal";

		lvl = rand() % 2 + 13;

		xp = lvl;

		if (lvl == 13)
			maxHP = 80;
		else if (lvl == 14)
			maxHP = 90;

		hp = maxHP;

		if (lvl == 13)
			atk = 25;
		else if (lvl == 14)
			atk = 27;

		if (lvl == 13)
			def = 4;
		else if (lvl == 14)
			def = 5;

		if (lvl == 13)
			magicResist = 3;
		else if (lvl == 14)
			magicResist = 4;

		copper = lvl;

		//Item Loot
		loot = itemID::RottenFlesh;

		if (lvl == 13)
			lootChance = 33;
		else if (lvl == 14)
			lootChance = 66;
	}

	void takeTurn(Player& player, vector<string>& logs, int& turnCount) override
	{
		int roll = rand() % 100 + 1;

		if (roll <= 15 && hasPutridBreath == true)
		{
			PutridBreath(player, logs);
			return;
		}

		basicAttack(player, logs);
	}

	void PutridBreath(Player& player, vector<string>& logs)
	{
		hasPutridBreath = false;

		debuff d{ .poison = 3 };
		int turns{ 10 };

		player.pdebuffs.push_back(d);
		player.pdebuffsTimer.push_back(turns);

		string log = get_name() + " uses " + mobSkillColor("Uncommon") + "Putrid Breath " + clearColor() + "and " + typeColor("Poison") + "poisons " + clearColor() + player.get_name() + "!";
		logs.push_back(log);
		updateLogs(player, logs);
	}
};


class TormentedSoul :public baseMob
{
public:
	bool hasAgonizingScream{ true }; //reduces player atk + spellpower + buffs its own atk (cast when half health)
	bool hasSpectralVeil { true }; //buffs dodgeChance

	TormentedSoul()
	{
		name = "Tormented Soul";
		mobType = "Boss";

		lvl = 15;

		xp = 160;

		maxHP = 200;
		hp = maxHP;

		atk = 30;
		spellPower = 8;

		def = 5;
		magicResist = 5;

		silver = 3;

		//Item Loot
		loot = itemID::GhostEssence;
		
		lootChance = 100;
	}

	void takeTurn(Player& player, vector<string>& logs, int& turnCount) override
	{
		int roll = rand() % 100 + 1;

		if (hp <= maxHP / 2 && hasAgonizingScream == true)
		{
			AgonizingScream(player, logs);
			return;
		}

		if (turnCount < 5 && roll <= 20 && hasSpectralVeil == true)
		{
			SpectralVeil(player, logs);
			return;
		}
		else if (turnCount >= 5 && hasSpectralVeil == true)
		{
			SpectralVeil(player, logs);
			return;
		}

		if (roll <= 30)
		{
			ChillingGrasp(player, logs);
			return;
		}

		basicAttack(player, logs);
	}

	void ChillingGrasp(Player& player, vector<string>& logs) //magic attack
	{
		bool hit = checkMiss(player, logs);

		if (hit == true)
		{
			int dmg = spellPower;
			dmg -= player.get_magicResist();
			
			if (dmg < 1)
				dmg = 1;

			player.sub_HP(dmg);

			string log = get_name() + " uses " + mobSkillColor("Uncommon") + "Chilling Grasp " + clearColor() + "and deals " + to_string(dmg) + typeColor("Magical") + " magical damage" + clearColor() + "!";
			logs.push_back(log);
			updateLogs(player, logs);
		}
		else
		{
			string log = get_name() + " uses " + mobSkillColor("Uncommon") + "Chilling Grasp " + clearColor() + " but misses!";
			logs.push_back(log);
			updateLogs(player, logs);
		}
	}

	void SpectralVeil(Player& player, vector<string>& logs)
	{
		hasSpectralVeil = false;

		int amount{ 20 };
		dodgeChance += amount;

		string log = get_name() + " uses " + mobSkillColor("Uncommon") + "Spectral Veil " + clearColor() + "and gains " + to_string(amount) + "% dodge chance!";
		logs.push_back(log);
		updateLogs(player, logs);
	}

	void AgonizingScream(Player& player, vector<string>& logs)
	{
		hasAgonizingScream = false;

		debuff d = { .weak = 3, .nullify = 3 };
		int turns{ 99 };
		player.pdebuffs.push_back(d);
		player.pdebuffsTimer.push_back(turns);

		atk += 3;
		spellPower += 3;

		string log = get_name() + " uses " + mobSkillColor("Rare") + "Agonizing Scream " + clearColor() + "and steals " + to_string(d.weak) + typeColor("Physical") + " attack power "
			                    + clearColor() + "and " + to_string(d.nullify) + typeColor("Magical") + " magical power " + clearColor() + "from " + player.get_name() + "!";
		logs.push_back(log);
		updateLogs(player, logs);
	}
};

// SUNKEN PASSAGE -----------------------------------------------

class GiantTarantula :public baseMob
{
public:
	int VenomCooldown{ 0 };

	GiantTarantula()
	{
		name = "Giant Tarantula";
		mobType = "Normal";

		lvl = rand() % 2 + 16;

		xp = lvl;

		if (lvl == 16)
			maxHP = 120;
		else if (lvl == 17)
			maxHP = 130;

		hp = maxHP;

		if (lvl == 16)
			atk = 30;
		else if (lvl == 17)
			atk = 32;

		if (lvl == 16)
			def = 5;
		else if (lvl == 17)
			def = 6;

		if (lvl == 16)
			magicResist = 4;
		else if (lvl == 17)
			magicResist = 5;

		copper = lvl;

		//Item Loot
		loot = itemID::BundleSilk;

		if (lvl == 16)
			lootChance = 33;
		else if (lvl == 17)
			lootChance = 66;
	}

	void takeTurn(Player& player, vector<string>& logs, int& turnCount) override
	{
		int roll = rand() % 100 + 1;

		if (roll <= 10)
		{
			EntanglePrey(player, logs);
			return;
		}

		if (roll <= 15 && VenomCooldown == 0)
		{
			Venom(player, logs);
			return;
		}

		basicAttack(player, logs);
	}

	void EntanglePrey(Player& player, vector<string>& logs)
	{
		player.stunned = true;

		bool hit = checkMiss(player, logs);

		if (hit == true)
		{
			int dmg = atk + 4;
			dmg -= player.get_defense();

			if (dmg < 1)
				dmg = 1;

			player.sub_HP(dmg);

			string log = get_name() + " uses " + mobSkillColor("Uncommon") + "Entangle Prey " + clearColor() + "and deals " + to_string(dmg) + typeColor("Physical") + " physical damage" + clearColor() + "!";
			logs.push_back(log);
			updateLogs(player, logs);

			log = player.get_name() + " is stunned!";
			logs.push_back(log);
			updateLogs(player, logs);
		}
		else
		{
			string log = get_name() + " uses " + mobSkillColor("Uncommon") + "Entangle Prey " + clearColor() + "but misses!";
			logs.push_back(log);
			updateLogs(player, logs);
		}
	}

	void Venom(Player& player, vector<string>& logs)
	{
		debuff d { .poison = 3 };
		int turns{ 5 };

		VenomCooldown = 20;

		player.pdebuffs.push_back(d);
		player.pdebuffsTimer.push_back(turns);

		string log = get_name() + " uses " + mobSkillColor("Uncommon") + "Venom " + clearColor() + "and " + typeColor("Poison") + "poisons " + clearColor() + player.get_name() + "!";
		logs.push_back(log);
		updateLogs(player, logs);
	}
};

class MossySlime :public baseMob
{
public:
	bool hasGrow{ true };

	MossySlime()
	{
		name = "Mossy Slime";
		mobType = "Normal";

		lvl = rand() % 2 + 18;

		xp = lvl;

		if (lvl == 18)
			maxHP = 140;
		else if (lvl == 19)
			maxHP = 150;

		hp = maxHP;
		hpRegen = 1;

		if (lvl == 18)
			atk = 34;
		else if (lvl == 19)
			atk = 36;

		if (lvl == 18)
			def = 8;
		else if (lvl == 19)
			def = 9;

		magicResist = 4;

		copper = lvl;

		//Item Loot
		loot = itemID::MossyGoo;

		if (lvl == 18)
			lootChance = 33;
		else if (lvl == 19)
			lootChance = 66;
	}

	void takeTurn(Player& player, vector<string>& logs, int& turnCount) override
	{
		int roll = rand() % 100 + 1;

		if (roll <= 15 && hasGrow == true)
		{
			Grow(player, logs);
			return;
		}
		else if (hp <= maxHP / 2 && hasGrow == true)
		{
			Grow(player, logs);
			return;
		}

		basicAttack(player, logs);
	}

	void Grow(Player& player, vector<string>& logs)
	{
		hasGrow = false;

		int amount{ 5 };
		hpRegen += amount;

		string log = name + " uses " + mobSkillColor("Uncommon") + "Grow " + clearColor() + "and gains " + to_string(amount) + typeColor("Health") + " health regen" + clearColor() + "!";
		logs.push_back(log);
		updateLogs(player, logs);
	}
};

class OvergrownThornroot :public baseMob
{
public:
	bool hasSharpThorns{ true }; //increases thorns (cast at 50% HP)
	bool hasPhotosynthesis{ true };

	OvergrownThornroot()
	{
		name = "Overgrown Thornroot";
		mobType = "Boss";

		lvl = 20;

		xp = 300;

		maxHP = 300;
		hp = maxHP;

		hpRegen = 2;

		atk = 40;
		def = 10;
		magicResist = 6;

		silver = 7;
		copper = 50;

		thorns = 3;

		//Item Loot
		loot = itemID::Thornroot;
		lootChance = 100;
	}

	void takeTurn(Player& player, vector<string>& logs, int& turnCount) override
	{
		int roll = rand() % 100 + 1;

		if (roll <= 15)
		{
			PoisonousScratch(player, logs);
			return;
		}

		if (hp <= maxHP / 2 && hasSharpThorns == true)
		{
			SharpThorns(player, logs);
			return;
		}

		if (hp <= maxHP / 2 && hasPhotosynthesis == true && roll <= 40)
		{
			Photosynthesis(player, logs);
			return;
		}

		basicAttack(player, logs);
	}

	void SharpThorns(Player& player, vector<string>& logs)
	{
		hasSharpThorns = false;

		int increase{ 7 };
		thorns += increase;

		string log = name + " uses " + mobSkillColor("Rare") + "Sharp Thorns " + clearColor() + "and gains " + to_string(increase) + typeColor("Thorns") + " thorns" + clearColor() + "!";
		logs.push_back(log);
		updateLogs(player, logs);
	}

	void PoisonousScratch(Player& player, vector<string>& logs)
	{
		bool hit = checkMiss(player, logs);

		if (hit == true)
		{
			debuff d{ .poison = 3 };
			int turns{ 15 };

			player.pdebuffs.push_back(d);
			player.pdebuffsTimer.push_back(turns);

			int dmg = atk + 5;
			dmg -= player.get_defense();

			if (dmg < 1)
				dmg = 1;

			player.sub_HP(dmg);

			string log = name + " uses " + mobSkillColor("Uncommon") + "Poisonous Scratch " + clearColor() + "and deals " + to_string(dmg) + typeColor("Physical") + " physical damage" + clearColor() + "!";
			logs.push_back(log);
			updateLogs(player, logs);

			log = player.get_name() + " is " + typeColor("Poison") + " poisoned" + clearColor() + "!";
			logs.push_back(log);
			updateLogs(player, logs);
		}
		else
		{
			string log = name + " uses " + mobSkillColor("Uncommon") + "Poisonous Scratch " + clearColor() + "but misses!";
			logs.push_back(log);
			updateLogs(player, logs);
		}
	}

	void Photosynthesis(Player& player, vector<string>& logs)
	{
		hasPhotosynthesis = false;

		int hpIncrease{ 75 };

		hpRegen += 3;
		hp += 75;

		string log = name + " uses " + mobSkillColor("Rare") + "Photosynthesis " + clearColor() + "and heals for " + to_string(hpIncrease) + typeColor("Health") + " health" + clearColor() + "!";
		logs.push_back(log);
		updateLogs(player, logs);

		log = name + " applies" + typeColor("Health") + " health regen" + clearColor() + "!";
		logs.push_back(log);
		updateLogs(player, logs);
	}
};

// DEAD SHORE ----------------------------------------------------------------------------------------------------------------------------------

class DecayingVulture :public baseMob
{
public:
	bool hasFlight{ true }; //goes into flight, increasing dodgeChance

	DecayingVulture()
	{
		name = "Decaying Vulture";
		mobType = "Normal";

		lvl = rand() % 4 + 21; //can be from 21 to 24

		xp = lvl;

		if (lvl == 21)
			maxHP = 200;
		else if (lvl == 22)
			maxHP = 220;
		else if (lvl == 23)
			maxHP = 240;
		else if (lvl == 24)
			maxHP = 260;

		hp = maxHP;
		hpRegen = 1;

		if (lvl == 21)
			atk = 40;
		else if (lvl == 22)
			atk = 42;
		else if (lvl == 23)
			atk = 44;
		else if (lvl == 24)
			atk = 45;

		if (lvl == 21)
			def = 10;
		else if (lvl == 22)
			def = 11;
		else if (lvl == 23 || lvl == 24)
			def = 12;

		magicResist = 6;

		copper = lvl;

		//Item Loot
		loot = itemID::GiantFeather;

		if (lvl == 21 || lvl == 22)
			lootChance = 33;
		else if (lvl == 23 || lvl == 24)
			lootChance = 66;
	}

	void takeTurn(Player& player, vector<string>& logs, int& turnCount) override
	{
		int roll = rand() % 100 + 1;

		if (roll <= 30 && hasFlight == true)
		{
			Flight(player, logs);
			return;
		}

		if (roll <= 10)
		{
			Claws(player, logs);
			return;
		}

		basicAttack(player, logs);
	}

	void Flight(Player& player, vector<string>& logs)
	{
		hasFlight = false;

		int increase{ 20 };
		dodgeChance += increase;

		string log = name + " uses " + mobSkillColor("Uncommon") + "Flight " + clearColor() + "and gains" + to_string(increase) + "% dodge chance!";
		logs.push_back(log);
		updateLogs(player, logs);
	}

	void Claws(Player& player, vector<string>& logs)
	{
		bool hit = checkMiss(player, logs);

		if (hit == true)
		{
			debuff d{ .bleed = 3 };
			int turns{ 10 };

			player.pdebuffs.push_back(d);
			player.pdebuffsTimer.push_back(turns);

			int dmg = atk;
			dmg -= player.get_defense();

			if (dmg < 1)
				dmg = 1;

			player.sub_HP(dmg);
				
			string log = name + " uses " + mobSkillColor("Uncommon") + "Claws " + clearColor() + "and deals" + to_string(dmg) + "!";
			logs.push_back(log);
			updateLogs(player, logs);

			log = player.get_name() + " is " + typeColor("Bleed") + "bleeding" + clearColor() + "!";
			logs.push_back(log);
			updateLogs(player, logs);
		}
		else
		{
			string log = name + " uses " + mobSkillColor("Uncommon") + "Claws " + clearColor() + "but misses!";
			logs.push_back(log);
			updateLogs(player, logs);
		}
	}
};

class Wanderer :public baseMob
{
public:
	bool hasDeny{ true }; //clears all player buffs
	bool hasEmpower{ true }; //increases attack and spellpower (cast at 50% hp)
	bool hasImpatience{ true }; //applies critChance and hitChance + lowers player atk and spellPower
	bool hasFinalStand{ true }; //applies hp regen, def, magicRes, thorns (cast at 20% hp)
	bool hasMoltenBlade{ false }; //enchants weapon to apply magic damage on-hit

	Wanderer()
	{
		name = "Wanderer";
		mobType = "Boss";

		lvl = 25;

		xp = 1000;

		maxHP = 500;
		hp = maxHP;

		hpRegen = 3;

		atk = 50;
		spellPower = 8;
		def = 12;
		magicResist = 7;

		silver = 15;

		//Item Loot
		loot = itemID::WandererGreatSword;
		lootChance = 100;
	}

	void takeTurn(Player& player, vector<string>& logs, int& turnCount) override
	{
		int roll = rand() % 100 + 1;

		if (roll <= 15 && hasMoltenBlade == false)
		{
			MoltenBlade(player, logs);
			return;
		}
		else if (hp < maxHP / 2 && hasMoltenBlade == false)
		{
			MoltenBlade(player, logs);
			return;
		}

		if (roll <= 30 && player.pbuffs.size() > 0 && hasDeny == true)
		{
			Deny(player, logs);
			return;
		}

		if (roll <= 40 && hp < maxHP * 0.75 && hasImpatience == true)
		{
			Impatience(player, logs);
			return;
		}

		if (hp < maxHP / 2 && hasEmpower == true)
		{
			Empower(player, logs);
			return;
		}

		if (hp < maxHP * 0.2 && hasFinalStand == true)
		{
			FinalStand(player, logs);
			return;
		}

		if (hasMoltenBlade == true)
		{
			MoltenStrike(player, logs);
			return;
		}
		else
			basicAttack(player, logs);
	}

	void Deny(Player& player, vector<string>& logs)
	{
		hasDeny = false;

		erasePlayerDebuffs(player);

		string log = name + " uses " + mobSkillColor("Epic") + "Deny " + clearColor() + "and removes all of " + player.get_name() + "'s buffs!";
		logs.push_back(log);
		updateLogs(player, logs);
	}

	void Empower(Player& player, vector<string>& logs)
	{
		hasEmpower = false;

		atk += 10;
		spellPower += 10;

		string log = name + " uses " + mobSkillColor("Epic") + "Empower " + clearColor() + "and gains 10 " + typeColor("Physical") + "physical power "
			       + clearColor() + " and 10 " + typeColor("Magical") + "magical power" + clearColor() + "!";
		logs.push_back(log);
		updateLogs(player, logs);
	}

	void Impatience(Player& player, vector<string>& logs)
	{
		hasImpatience = false;

		critChance += 15;
		hitChance += 15;

		debuff d{ .weak = 3, .nullify = 3 };
		int turns{ 99 };

		player.pdebuffs.push_back(d);
		player.pdebuffsTimer.push_back(turns);

		string log = name + " uses " + mobSkillColor("Rare") + "Impatience " + clearColor() + "and gains 15% " + typeColor("Critical") + "critical chance " + clearColor() + " and 15% hit chance!";
		logs.push_back(log);
		updateLogs(player, logs);

		log = player.get_name() + " loses 3" + typeColor("Physical") + " physical power " + clearColor() + " and 3" + typeColor("Magical") + " magical power" + clearColor() + "!";
		logs.push_back(log);
		updateLogs(player, logs);
	}

	void MoltenBlade(Player& player, vector<string>& logs)
	{
		hasMoltenBlade = true;

		string log = name + " uses " + mobSkillColor("Epic") + "Molten Blade " + clearColor() + "and imbues their weapon in " + typeColor("Magical") + "magical power" + clearColor() + "!";
		logs.push_back(log);
		updateLogs(player, logs);
	}

	void MoltenStrike(Player& player, vector<string>& logs)
	{
		bool hit = checkMiss(player, logs);

		if (hit == true)
		{
			bool crit = rollCrit(0);

			int Pdmg = atk;
			int Mdmg = spellPower;

			if (crit == true)
				Pdmg *= critMult;

			Pdmg -= player.get_defense();
			Mdmg -= player.get_magicResist();

			if (Pdmg < 1)
				Pdmg = 1;
			if (Mdmg < 1)
				Mdmg = 1;

			player.sub_HP(Pdmg);
			player.sub_HP(Mdmg);

			string log = name + " uses " + mobSkillColor("Epic") + "Molten Strike " + clearColor() + "and deals " + to_string(Pdmg) + typeColor("Physical") + "physical damage " + clearColor() + "and "
				       + to_string(Mdmg) + typeColor("Magical") + " magical damage" + clearColor() + "!";
			logs.push_back(log);
			updateLogs(player, logs);
		}
		else
		{
			string log = name + " uses " + mobSkillColor("Epic") + "Molten Strike " + clearColor() + "but misses!";
			logs.push_back(log);
			updateLogs(player, logs);
		}
	}

	void FinalStand(Player& player, vector<string>& logs)
	{
		hasFinalStand = false;

		def += 3;
		magicResist += 3;

		hpRegen += 7;
		thorns += 5;

		string log = name + " uses " + mobSkillColor("Legendary") + "Final Stand " + clearColor() + "and gains 3 defense and 3 magic resist!";
		logs.push_back(log);
		updateLogs(player, logs);

		log = name + " applies 7 " + typeColor("Health") + "health regen" + clearColor() + " and 5 " + typeColor("Thorns") + "thorns" + clearColor() + "!";
		logs.push_back(log);
		updateLogs(player, logs);
	}

};




//The wanderer (boss deadshore) has greatsword that he will drop. "Wanderer's Greatsword [Rare]"

