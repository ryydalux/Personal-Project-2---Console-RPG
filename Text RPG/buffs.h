#pragma once
#include <iostream>

using namespace std;

struct buff //Player buffs can only increase the main stats for X amount of time
{
	int maxHP{ 0 };
	int maxMP{ 0 };

	int hpRegen{ 0 };
	int mpRegen{ 0 };

	int atk{ 0 };
	int spellPower{ 0 };
	int defense{ 0 };
	int magicResist{ 0 };

	int hitChance{ 0 };
	int dodgeChance{ 0 };

	int critChance{ 0 };
	double critMult{ 0 };

	int lifesteal{ 0 };
	int spellVamp{ 0 };

	int thorns{ 0 };
};

struct debuff 
{
	int bleed; //hp--
	int poison; //hp--  (usually lasts longer than bleed)

	int drain; //mp--

	int weak; //atk debuff
	int nullify; //spellPower debuff

	int frail; //def debuff
	int shatter; //magicResist debuff

	int blind; //hitChance debuff
	int immobile; //dodgeChance debuff
};