#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <variant>
#include <optional>

#include "item_ID.h"
#include "buffs.h"
using namespace std;

struct enchantments //Meant for armor/weapons/accessories
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

enum class itemType //Allows program to know what is the item exactly (Useful for inventory system).
{
	None,

	Weapon,
	Shield,

	Head,
	Shoulders,
	Chest,
	Arms,
	Legs,
	Feet,

	Cape,
	Ring,
	Necklace,

	Food, //Food have their own itemType because they cannot be used in combat. Therefore they need to be separated from potions.
	Potion, //Can be used in inventory (meant for regular restorative potions)
	BuffPotion, 

	SkillBook,
	Loot,
};

enum class itemRarity
{
	None,

	Common,
	Uncommon,
	Rare,
	Epic,
	Legendary
};

struct weapon_stats
{
	int atk; 
	int spellPower; 

	int hitChance; // Daggers +5 / Swords +0 / GreatSwords -5
	int critChance; // Daggers +10
	bool twoHanded; //Is weapon two handed? (prevents use of shields)

};

struct armor_stats
{
	string weight; //Light armor provides dodgeChance and magicResist // Medium provides defense // Heavy provides the most defense but at the cost of losing dodgeChance
	int def;
	int magicResist;

	int dodgeChance; // Light Armor +1 / Medium Armor +0 / Heavy Armor -1   ||   Medium Shield +0 / Heavy Shield -1
};

struct consumable_stats //Meant for foods and potions
{
	string type; //Health / Mana
	int powerValue; //Effect's power value (heal, poison, or whatever) // Skillbooks powervalue represent lvl required 
};

struct consumable_buffs //for buff potions
{
	buff buffs;
	int turns; 
};


struct baseItem 
{
	string name; //Name of Item
	string description; //Basic description of item
	itemRarity rarity; //None, Common (White), Uncommon (Green), Rare (Blue), Epic (Purple), Legendary (Yellow)
	itemType type; //what is the item exactly?
	int priceValue; //Value of item in shop
	bool stackable; //Is item stackable?

	variant<monostate, weapon_stats, armor_stats, consumable_stats, consumable_buffs> stats;
	optional<enchantments> enchants; //If equipement has enchant buffs
	optional<skillID> skill; //for skillbooks, contains the skill it unlocks upon use
};



//Item Database ------------------------------------------------------------------------------------------------------------------------------------ //Copper : 1$ //Silver 100$ //Gold 10 000$

inline unordered_map<itemID, baseItem> itemDatabase
{
	{itemID::None, {"", "", itemRarity::None, itemType::None, 0, false, {}}},

	// Physical Weapons (weapon_stats : atk, spellPower, hitChance, critChance, twohanded)

	{itemID::RustyDagger, {"Rusty Dagger", "A rusty iron dagger, trashed away by their previous owner.", itemRarity::Common, itemType::Weapon, 5, false, weapon_stats{1,0,5,10, false}, {}} },
	{itemID::RustySword, {"Rusty Sword", "A rusty iron blade, it appears to be heavily corroded.", itemRarity::Common, itemType::Weapon, 15, false, weapon_stats{2,0,0,0, false}, {}}},

	{itemID::IronDagger, {"Iron Dagger", "A tiny but well-crafted iron dagger.", itemRarity::Common, itemType::Weapon, 45, false, weapon_stats{2,0,5,10, false}, {}}},
	{itemID::IronSword, {"Iron Sword", "A balanced iron blade.", itemRarity::Common, itemType::Weapon, 65, false, weapon_stats{3,0,0,0, false}, {}}},
	{itemID::IronGreatSword, {"Iron Greatsword", "A long and imposing iron blade.", itemRarity::Common, itemType::Weapon, 100, false, weapon_stats{5,0,-5,0, true}, {}}},

	{itemID::SteelDagger, {"Steel Dagger", "A tiny but polished steel dagger.", itemRarity::Uncommon, itemType::Weapon, 150, false, weapon_stats{4,0,5,10, false}, {}}},
	{itemID::SteelSword, {"Steel Sword", "A well-polished steel blade.", itemRarity::Uncommon, itemType::Weapon, 190, false, weapon_stats{5,0,0,0, false}, {}}},
	{itemID::SteelGreatSword, {"Steel Greatsword", "A long and imposing steel blade.", itemRarity::Uncommon, itemType::Weapon, 250, false, weapon_stats{7,0,-5,0, true}, {}}},

	{itemID::RogueDagger, {"Rogue's Dagger", " tiny but well-crafted iron dagger,\n                                        imbued with magical essence.", itemRarity::Uncommon, itemType::Weapon, 350, false, weapon_stats{2,0,5,10, true}, enchantments{.atk = 3, .critChance = 5}}},
	{itemID::JuggernautGreatsword, {"Juggernaut's Greatsword", "A long and imposing iron blade,\n                                        imbued with magical essence.", itemRarity::Uncommon, itemType::Weapon, 350, false, weapon_stats{5,0,-5,0, true}, enchantments{.maxHP = 5, .atk = 2}}},

	{itemID::UnbrokenSword, {"Sword of the Unbroken", "A well-polished steel blade,\n                                         imbued with powerful magical essence.", itemRarity::Epic, itemType::Weapon, 650, false, weapon_stats{5,0,0,0, false}, enchantments{.maxHP = 10, .hpRegen = 2, .atk = 2}}},
	{itemID::UnbrokenGreatSword, {"Greatsword of the Unbroken", "A long and imposing steel blade,\n                                         imbued with powerful magical essence.", itemRarity::Epic, itemType::Weapon, 750, false, weapon_stats{7,0,-5,0, true}, enchantments{.maxHP = 15, .hpRegen = 3, .atk = 4}}},

	{itemID::WandererGreatSword, {"The Wanderer's Greatsword", "A mystical blade that once belonged to a fallen prince.", itemRarity::Legendary, itemType::Weapon, 3000, false, weapon_stats{10,0,-5,0, true}, enchantments{.maxHP = 40, .maxMP = 15, .hpRegen = 5, .mpRegen = 3, .atk = 8, .spellPower = 8}}},
	
	//Shields (armor_stats : weight, def, magicResist, dodgeChance)

	{itemID::LeatherShield, {"Leather Shield", "A shield barely held together by a few leather straps.", itemRarity::Common, itemType::Shield, 30, false, armor_stats{"Medium",1,0,0}, {}}},
	{itemID::IronShield, {"Iron Shield", "A sturdy and reliable iron shield.", itemRarity::Common, itemType::Shield, 80, false, armor_stats{"Heavy",2,0,-1}, {}}},
	{itemID::SteelShield, {"Steel Shield", "A sturdy and reliable steel shield.", itemRarity::Uncommon, itemType::Shield, 215, false, armor_stats{"Heavy",3,0,-1}, {}}},

	{itemID::ThornrootShield, {"Thornroot Shield", "A sturdy and reliable shield made out of steel and thornroot.", itemRarity::Rare, itemType::Shield, 425, false, armor_stats{"Heavy",3,0,-1}, enchantments{.maxHP = 15, .defense = 2, .thorns = 5}}},

	{itemID::UnbrokenShield, {"Shield of the Unbroken", "A sturdy and reliable steel shield,\n                                        imbued with powerful magical essence.", itemRarity::Epic, itemType::Shield, 700, false, armor_stats{"Heavy",3,0,-1}, enchantments{.maxHP = 20, .hpRegen = 5, .defense = 3, .magicResist = 2}}},

	//Unique Shields

	//Magical Weapons (-5 hitChance because two handed)

	{itemID::WoodenStaff, {"Wooden Staff", "An old piece of wood that is somehow capable of casting magic.", itemRarity::Common, itemType::Weapon, 20, false, weapon_stats{1,2,-5,0, true}, {}} },
	{itemID::IronRod, {"Iron Rod", "A long iron rod capable of channeling magic.", itemRarity::Common, itemType::Weapon, 80, false, weapon_stats{2,5,-5,0, true}, {}} },
	{itemID::SteelRod, {"Steel Rod", "A long steel rod capable of channeling magic.", itemRarity::Uncommon, itemType::Weapon, 215, false, weapon_stats{3,8,-5,0, true}, {}} },

	{itemID::ScholarRod, {"Scholar's Rod", "A long iron rod capable of channeling magic,\n                                        imbued with magical essence.", itemRarity::Uncommon, itemType::Weapon, 350, false, weapon_stats{2,5,-5,0, true}, enchantments{.mpRegen = 1, .spellPower = 4}} },

	{itemID::VladimirScepter, {"Vladimir's Scepter", "A mysterious scepter that once belonged to a powerful dark wizard.", itemRarity::Epic, itemType::Weapon, 575, false, weapon_stats{3,8,-5,0, true}, enchantments{.spellPower = 5, .spellVamp = 20}} },
	
	{itemID::ElderRod, {"Rod of the Elders", "A long steel rod capable of channeling magic,\n                                        imbued with powerful magical essence.", itemRarity::Epic, itemType::Weapon, 700, false, weapon_stats{3,8,-5,0, true}, enchantments{.mpRegen = 3, .spellPower = 8}} },

	// Medium/Heavy Armor (armor_stats : weight, def, magicResist, dodgeChance) //3X cost increase per tier / Chestplate = 50% more expensive

	{itemID::LeatherHelmet, {"Leather Coif", "A worn hat made out of leather.", itemRarity::Common, itemType::Head, 20, false, armor_stats{"Medium",1,0,0}, {}}},
	{itemID::LeatherShoulder, {"Leather Spaulders", "A pair of worn leather spaulders.", itemRarity::Common, itemType::Shoulders, 20, false, armor_stats{"Medium",1,0,0}, {}}},
	{itemID::LeatherChest, {"Leather Jerkin", "A worn leather jerkin.", itemRarity::Common, itemType::Chest, 20, false, armor_stats{"Medium",1,0,0}, {}}},
	{itemID::LeatherArms, {"Leather Bracers", "A worn pair of leather bracers.", itemRarity::Common, itemType::Arms, 20, false, armor_stats{"Medium",1,0,0}, {}}},
	{itemID::LeatherLegs, {"Leather Breeches", "A worn pair of leather breeches.", itemRarity::Common, itemType::Legs, 20, false, armor_stats{"Medium",1,0,0}, {}}},
	{itemID::LeatherFeet, {"Leather Boots", "A worn pair of leather boots.", itemRarity::Common, itemType::Feet, 20, false, armor_stats{"Medium",1,0,0}, {}}},

	{itemID::IronHelmet, {"Iron Helmet", "A sturdy helmet made of iron.", itemRarity::Common, itemType::Head, 60, false, armor_stats{"Heavy",2,0,-1}, {}}},
	{itemID::IronShoulder, {"Iron Pauldrons", "A sturdy pair of iron pauldrons.", itemRarity::Common, itemType::Shoulders, 60, false, armor_stats{"Heavy",2,0,-1}, {}}},
	{itemID::IronChest, {"Iron Chestplate", "A sturdy iron chestplate.", itemRarity::Common, itemType::Chest, 60, false, armor_stats{"Heavy",2,0,-1}, {}}},
	{itemID::IronArms, {"Iron Gauntlets", "A sturdy pair of iron gauntlets.", itemRarity::Common, itemType::Arms, 60, false, armor_stats{"Heavy",2,0,-1}, {}}},
	{itemID::IronLegs, {"Iron Greaves", "A sturdy pair of iron greaves.", itemRarity::Common, itemType::Legs, 60, false, armor_stats{"Heavy",2,0,-1}, {}}},
	{itemID::IronFeet, {"Iron Sabatons", "A sturdy pair of iron sabatons.", itemRarity::Common, itemType::Feet, 60, false, armor_stats{"Heavy",2,0,-1}, {}}},

	{itemID::ScaleHelmet, {"Scale Coif", "A hat made out of reinforced leather.", itemRarity::Uncommon, itemType::Head, 225, false, armor_stats{"Medium",2,0,0}, {}}},
	{itemID::ScaleShoulder, {"Scale Spaulders", "A pair of reinforced leather spaulders.", itemRarity::Uncommon, itemType::Shoulders, 225, false, armor_stats{"Medium",2,0,0}, {}}},
	{itemID::ScaleChest, {"Scale Jerkin", "A reinforced leather jerkin.", itemRarity::Uncommon, itemType::Chest, 225, false, armor_stats{"Medium",2,0,0}, {}}},
	{itemID::ScaleArms, {"Scale Bracers", "A pair of reinforced leather bracers.", itemRarity::Uncommon, itemType::Arms, 225, false, armor_stats{"Medium",2,0,0}, {}}},
	{itemID::ScaleLegs, {"Scale Breeches", "A pair of reinforced leather breeches.", itemRarity::Uncommon, itemType::Legs, 225, false, armor_stats{"Medium",2,0,0}, {}}},
	{itemID::ScaleFeet, {"Scale Boots", "A pair of reinforced leather boots.", itemRarity::Uncommon, itemType::Feet, 225, false, armor_stats{"Medium",2,0,0}, {}}},

	{itemID::SteelHelmet, {"Steel Helmet", "A sturdy helmet made out of steel.", itemRarity::Uncommon, itemType::Head, 225, false, armor_stats{"Heavy",3,1,-1}, {}}},
	{itemID::SteelShoulder, {"Steel Pauldrons", "A sturdy pair of pauldrons made out of steel.", itemRarity::Uncommon, itemType::Shoulders, 225, false, armor_stats{"Heavy",3,1,-1}, {}}},
	{itemID::SteelChest, {"Steel Chestplate", "A sturdy chestplate made out of steel.", itemRarity::Uncommon, itemType::Chest, 225, false, armor_stats{"Heavy",3,1,-1}, {}}},
	{itemID::SteelArms, {"Steel Gauntlets", "A sturdy pair of steel gauntlets.", itemRarity::Uncommon, itemType::Arms, 225, false, armor_stats{"Heavy",3,1,-1}, {}}},
	{itemID::SteelLegs, {"Steel Greaves", "A sturdy pair of steel greaves.", itemRarity::Uncommon, itemType::Legs, 225, false, armor_stats{"Heavy",3,1,-1}, {}}},
	{itemID::SteelFeet, {"Steel Sabatons", "A sturdy pair of steel sabatons.", itemRarity::Uncommon, itemType::Feet, 225, false, armor_stats{"Heavy",3,1,-1}, {}}},


	{itemID::RogueHelmet, {"Rogue's Coif", "A hat made out of leather,\n                                        imbued with magical essence.", itemRarity::Uncommon, itemType::Head, 250, false, armor_stats{"Medium",1,0,0}, enchantments{.atk = 2, .dodgeChance = 2, .critChance = 3}}},
	{itemID::RogueShoulder, {"Rogue's Spaulders", "A pair of leather spaulders,\n                                        imbued with magical essence.", itemRarity::Uncommon, itemType::Shoulders, 250, false, armor_stats{"Medium",1,0,0}, enchantments{.atk = 2, .dodgeChance = 2, .critChance = 3}}},
	{itemID::RogueChest, {"Rogue's Jerkin", "A leather jerkin,\n                                        imbued with magical essence.", itemRarity::Uncommon, itemType::Chest, 250, false, armor_stats{"Medium",1,0,0}, enchantments{.atk = 2, .dodgeChance = 2, .critChance = 3}}},
	{itemID::RogueArms, {"Rogue's Bracers", "A pair of leather bracers,\n                                        imbued with magical essence.", itemRarity::Uncommon, itemType::Arms, 250, false, armor_stats{"Medium",1,0,0}, enchantments{.atk = 2, .dodgeChance = 2, .critChance = 3}}},
	{itemID::RogueLegs, {"Rogue's Breeches", "A pair of leather breeches,\n                                        imbued with magical essence.", itemRarity::Uncommon, itemType::Legs, 250, false, armor_stats{"Medium",1,0,0}, enchantments{.atk = 2, .dodgeChance = 2, .critChance = 3}}},
	{itemID::RogueFeet, {"Rogue's Boots", "A pair of leather boots,\n                                        imbued with magical essence.", itemRarity::Uncommon, itemType::Feet, 250, false, armor_stats{"Medium",1,0,0}, enchantments{.atk = 2, .dodgeChance = 2, .critChance = 3}}},

	{itemID::JuggernautHelmet, {"Juggernaut's Helmet", "A sturdy helmet made of iron,\n                                        imbued with magical essence.", itemRarity::Uncommon, itemType::Head, 250, false, armor_stats{"Heavy",2,0,-1}, enchantments{.maxHP = 5, .defense = 1, .magicResist = 1}}},
	{itemID::JuggernautShoulder, {"Juggernaut's Pauldrons", "A sturdy pair of iron pauldrons,\n                                        imbued with magical essence.", itemRarity::Uncommon, itemType::Shoulders, 250, false, armor_stats{"Heavy",2,0,-1}, enchantments{.maxHP = 5, .defense = 1, .magicResist = 1}}},
	{itemID::JuggernautChest, {"Juggernaut's Chestplate", "A sturdy iron chestplate,\n                                        imbued with magical essence.", itemRarity::Uncommon, itemType::Chest, 250, false, armor_stats{"Heavy",2,0,-1}, enchantments{.maxHP = 5, .defense = 1, .magicResist = 1}}},
	{itemID::JuggernautArms, {"Juggernaut's Gauntlets", "A sturdy pair of iron gauntlets,\n                                        imbued with magical essence.", itemRarity::Uncommon, itemType::Arms, 250, false, armor_stats{"Heavy",2,0,-1}, enchantments{.maxHP = 5, .defense = 1, .magicResist = 1}}},
	{itemID::JuggernautLegs, {"Juggernaut's Greaves", "A sturdy pair of iron greaves,\n                                        imbued with magical essence.", itemRarity::Uncommon, itemType::Legs, 250, false, armor_stats{"Heavy",2,0,-1}, enchantments{.maxHP = 5, .defense = 1, .magicResist = 1}}},
	{itemID::JuggernautFeet, {"Juggernaut's Sabatons", "A sturdy pair of iron sabatons,\n                                        imbued with magical essence.", itemRarity::Uncommon, itemType::Feet, 250, false, armor_stats{"Heavy",2,0,-1}, enchantments{.maxHP = 5, .defense = 1, .magicResist = 1}}},

	{ itemID::ThornrootHelmet, {"Thornroot Helmet", "A sturdy helmet made out of steel and thornroot.", itemRarity::Rare, itemType::Head, 450, false, armor_stats{"Heavy",3,1,-1}, enchantments{.maxHP = 10, .defense = 2, .thorns = 3}} },
	{ itemID::ThornrootShoulder, {"Thornroot Pauldrons", "A sturdy pair of pauldrons made out of steel and thornroot.", itemRarity::Rare, itemType::Shoulders, 450, false, armor_stats{"Heavy",3,1,-1}, enchantments{.maxHP = 10, .defense = 2, .thorns = 3}} },
	{ itemID::ThornrootChest, {"Thornroot Chestplate", "A sturdy chestplate made out of steel and thornroot.", itemRarity::Rare, itemType::Chest, 450, false, armor_stats{"Heavy",3,1,-1}, enchantments{.maxHP = 10, .defense = 2, .thorns = 3}} },
	{ itemID::ThornrootArms, {"Thornroot Gauntlets", "A sturdy pair of gauntlets made out of steel and thornroot.", itemRarity::Rare, itemType::Arms, 450, false, armor_stats{"Heavy",3,1,-1}, enchantments{.maxHP = 10, .defense = 2, .thorns = 3}} },
	{ itemID::ThornrootLegs, {"Thornroot Greaves", "A sturdy pair of greaves made out of steel and thornroot.", itemRarity::Rare, itemType::Legs, 450, false, armor_stats{"Heavy",3,1,-1}, enchantments{.maxHP = 10, .defense = 2, .thorns = 3}} },
	{ itemID::ThornrootFeet, {"Thornroot Sabatons", "A sturdy pair of sabatons made out of steel and thornroot.", itemRarity::Rare, itemType::Feet, 450, false, armor_stats{"Heavy",3,1,-1}, enchantments{.maxHP = 10, .defense = 2, .thorns = 3}} },

	{itemID::UnseenHelmet, {"Coif of the Unseen Blade", "A hat made out of reinforced leather,\n                                        imbued with powerful magical essence.", itemRarity::Epic, itemType::Head, 625, false, armor_stats{"Medium",2,0,0}, enchantments{.atk = 4, .hitChance = 3, .dodgeChance = 4, .critChance = 5}}},
	{itemID::UnseenShoulder, {"Spaulders of the Unseen Blade", "A pair of reinforced leather spaulders,\n                                        imbued with powerful magical essence.", itemRarity::Epic, itemType::Shoulders, 625, false, armor_stats{"Medium",2,0,0}, enchantments{.atk = 4, .hitChance = 3, .dodgeChance = 4, .critChance = 5}}},
	{itemID::UnseenChest, {"Jerkin of the Unseen Blade", "A reinforced leather jerkin,\n                                        imbued with powerful magical essence.", itemRarity::Epic, itemType::Chest, 625, false, armor_stats{"Medium",2,0,0}, enchantments{.atk = 4, .hitChance = 3, .dodgeChance = 4, .critChance = 5}}},
	{itemID::UnseenArms, {"Bracers of the Unseen Blade", "A pair of reinforced leather bracers,\n                                        imbued with powerful magical essence.", itemRarity::Epic, itemType::Arms, 625, false, armor_stats{"Medium",2,0,0}, enchantments{.atk = 4, .hitChance = 3, .dodgeChance = 4, .critChance = 5}}},
	{itemID::UnseenLegs, {"Breeches of the Unseen Blade", "A pair of reinforced leather breeches,\n                                        imbued with powerful magical essence.", itemRarity::Epic, itemType::Legs, 625, false, armor_stats{"Medium",2,0,0}, enchantments{.atk = 4, .hitChance = 3, .dodgeChance = 4, .critChance = 5}}},
	{itemID::UnseenFeet, {"Boots of the Unseen Blade", "A pair of reinforced leather boots,\n                                        imbued with powerful magical essence.", itemRarity::Epic, itemType::Feet, 625, false, armor_stats{"Medium",2,0,0}, enchantments{.atk = 4, .hitChance = 3, .dodgeChance = 4, .critChance = 5}}},

	{itemID::UnbrokenHelmet, {"Helm of the Unbroken", "A sturdy helmet made out of steel,\n                                        imbued with powerful magical essence.", itemRarity::Epic, itemType::Head, 625, false, armor_stats{"Heavy",3,1,-1}, enchantments{.maxHP = 15, .hpRegen = 1, .defense = 2, .magicResist = 2}}},
	{itemID::UnbrokenShoulder, {"Pauldrons of the Unbroken", "A sturdy pair of pauldrons made out of steel, imbued with powerful magical essence.", itemRarity::Epic, itemType::Shoulders, 625, false, armor_stats{"Heavy",3,1,-1}, enchantments{.maxHP = 10, .hpRegen = 1, .defense = 2, .magicResist = 2}}},
	{itemID::UnbrokenChest, {"Chestplate of the Unbroken", "A sturdy chestplate made out of steel,\n                                        imbued with powerful magical essenceimbued with powerful magical essence.", itemRarity::Epic, itemType::Chest, 625, false, armor_stats{"Heavy",3,1,-1}, enchantments{.maxHP = 15, .hpRegen = 1, .defense = 2, .magicResist = 2}}},
	{itemID::UnbrokenArms, {"Gauntlets of the Unbroken", "A sturdy pair of gauntlets made out of steel,\n                                        imbued with powerful magical essence.", itemRarity::Epic, itemType::Arms, 625, false, armor_stats{"Heavy",3,1,-1}, enchantments{.maxHP = 15, .hpRegen = 1, .defense = 2, .magicResist = 2}}},
	{itemID::UnbrokenLegs, {"Greaves of the Unbroken", "A sturdy pair of greaves made out of steel,\n                                        imbued with powerful magical essence.", itemRarity::Epic, itemType::Legs, 625, false, armor_stats{"Heavy",3,1,-1}, enchantments{.maxHP = 15, .hpRegen = 1, .defense = 2, .magicResist = 2}}},
	{itemID::UnbrokenFeet, {"Sabatons of the Unbroken", "A sturdy pair of sabatons made out of steel,\n                                        imbued with powerful magical essence.", itemRarity::Epic, itemType::Feet, 625, false, armor_stats{"Heavy",3,1,-1}, enchantments{.maxHP = 15, .hpRegen = 1, .defense = 2, .magicResist = 2}}},

	//Light Armor

	{itemID::ClothHelmet, {"Cloth Hood", "A slightly dirty hood made out of cloth.", itemRarity::Common, itemType::Head, 20, false, armor_stats{"Light",0,0,1}, {}}},
	{itemID::ClothShoulder, {"Cloth Epaulets", "A pair of epaulets made out of cloth.", itemRarity::Common, itemType::Shoulders, 20, false, armor_stats{"Light",0,0,1}, {}}},
	{itemID::ClothChest, {"Cloth Robe", "A light robe made out of cloth.", itemRarity::Common, itemType::Chest, 20, false, armor_stats{"Light",0,0,1}, {}}},
	{itemID::ClothArms, {"Cloth Cuffs", "A pair of small cuffs made out of cloth.", itemRarity::Common, itemType::Arms, 20, false, armor_stats{"Light",0,0,1}, {}}},
	{itemID::ClothLegs, {"Cloth Trousers", "Slightly dirty trousers made out of cloth.", itemRarity::Common, itemType::Legs, 20, false, armor_stats{"Light",0,0,1}, {}}},
	{itemID::ClothFeet, {"Cloth Shoes", "A pair of shoes made out of cloth.", itemRarity::Common, itemType::Feet, 20, false, armor_stats{"Light",0,0,1}, {}}},

	{itemID::SilkHelmet, {"Silk Hood", "A hood made out of silk.", itemRarity::Common, itemType::Head, 60, false, armor_stats{"Light",0,1,1}, {}}},
	{itemID::SilkShoulder, {"Silk Epaulets", "A pair of silk epaulets.", itemRarity::Common, itemType::Shoulders, 60, false, armor_stats{"Light",0,1,1}, {}}},
	{itemID::SilkChest, { "Silk Robe", "A light robe made out of silk.", itemRarity::Common, itemType::Chest, 60, false, armor_stats{"Light",0,1,1}, {} }},
	{ itemID::SilkArms, {"Silk Cuffs", "A pair of small cuffs made out of silk.", itemRarity::Common, itemType::Arms, 60, false, armor_stats{"Light",0,1,1}, {}} },
	{ itemID::SilkLegs, {"Silk Trousers", "Trousers made out of silk.", itemRarity::Common, itemType::Legs, 60, false, armor_stats{"Light",0,1,1}, {}} },
	{ itemID::SilkFeet, {"Silk Shoes", "A pair of shoes made out of silk.", itemRarity::Common, itemType::Feet, 60, false, armor_stats{"Light",0,1,1}, {}} },

	{ itemID::LinenHelmet, {"Linen Hood", "A hood made out of Linen.", itemRarity::Uncommon, itemType::Head, 225, false, armor_stats{"Light",1,2,1}, {}} },
	{ itemID::LinenShoulder, {"Linen Epaulets", "A pair of Linen epaulets.", itemRarity::Uncommon, itemType::Shoulders, 225, false, armor_stats{"Light",1,2,1}, {}} },
	{ itemID::LinenChest, { "Linen Robe", "A light robe made out of Linen.", itemRarity::Uncommon, itemType::Chest, 225, false, armor_stats{"Light",1,2,1}, {} } },
	{ itemID::LinenArms, {"Linen Cuffs", "A pair of small cuffs made out of Linen.", itemRarity::Uncommon, itemType::Arms, 225, false, armor_stats{"Light",1,2,1}, {}} },
	{ itemID::LinenLegs, {"Linen Trousers", "Trousers made out of Linen.", itemRarity::Uncommon, itemType::Legs, 225, false, armor_stats{"Light",1,2,1}, {}} },
	{ itemID::LinenFeet, {"Linen Shoes", "A pair of shoes made out of Linen.", itemRarity::Uncommon, itemType::Feet, 225, false, armor_stats{"Light",1,2,1}, {}} },


	{itemID::ScholarHelmet, {"Scholar's Hood", "A hood made out of silk,\n                                        imbued with magical essence.", itemRarity::Uncommon, itemType::Head, 250, false, armor_stats{"Light",0,1,1}, enchantments{.maxMP = 5, .spellPower = 2}}},
	{itemID::ScholarShoulder, {"Scholar's Epaulets", "A pair of silk epaulets,\n                                        imbued with magical essence.", itemRarity::Uncommon, itemType::Shoulders, 250, false, armor_stats{"Light",0,1,1}, enchantments{.maxMP = 5, .spellPower = 2}}},
	{itemID::ScholarChest, {"Scholar's Robe", "A light robe made out of silk,\n                                        imbued with magical essence.", itemRarity::Uncommon, itemType::Chest, 250, false, armor_stats{"Light",0,1,1}, enchantments{.maxMP = 5, .spellPower = 2}}},
	{itemID::ScholarArms, {"Scholar's Cuffs", "Trousers made out of silk,\n                                        imbued with magical essence.", itemRarity::Uncommon, itemType::Arms, 250, false, armor_stats{"Light",0,1,1}, enchantments{.maxMP = 5, .spellPower = 2}}},
	{itemID::ScholarLegs, {"Scholar's Trousers", "A pair of small cuffs made out of silk,\n                                        imbued with magical essence.", itemRarity::Uncommon, itemType::Legs, 250, false, armor_stats{"Light",0,1,1}, enchantments{.maxMP = 5, .spellPower = 2}}},
	{itemID::ScholarFeet, {"Scholar's Shoes", "A pair of shoes made out of silk,\n                                        imbued with magical essence.", itemRarity::Uncommon, itemType::Feet, 250, false, armor_stats{"Light",0,1,1}, enchantments{.maxMP = 5, .spellPower = 2}}},
	
	{ itemID::ElderHelmet, {"Hood of the Elders", "A hood made out of Linen,\n                                        imbued with powerful magical essence.", itemRarity::Epic, itemType::Head, 625, false, armor_stats{"Light",1,2,1}, enchantments{.maxMP = 10, .mpRegen = 2, .spellPower = 4, .magicResist = 1}} },
	{ itemID::ElderShoulder, {"Epaulets of the Elders", "A pair of Linen epaulets,\n                                        imbued with powerful magical essence.", itemRarity::Epic, itemType::Shoulders, 625, false, armor_stats{"Light",1,2,1}, enchantments{.maxMP = 10, .mpRegen = 2, .spellPower = 4, .magicResist = 1}} },
	{ itemID::ElderChest, { "Robe of the Elders", "A light robe made out of Linen,\n                                        imbued with powerful magical essence.", itemRarity::Epic, itemType::Chest, 625, false, armor_stats{"Light",1,2,1}, enchantments{.maxMP = 10, .mpRegen = 2, .spellPower = 4, .magicResist = 1} } },
	{ itemID::ElderArms, {"Cuffs of the Elders", "A pair of small cuffs made out of Linen,\n                                        imbued with powerful magical essence.", itemRarity::Epic, itemType::Arms, 625, false, armor_stats{"Light",1,2,1}, enchantments{.maxMP = 10, .mpRegen = 2, .spellPower = 4, .magicResist = 1}} },
	{ itemID::ElderLegs, {"Trousers of the Elders", "Trousers made out of Linen,\n                                        imbued with powerful magical essence.", itemRarity::Epic, itemType::Legs, 625, false, armor_stats{"Light",1,2,1}, enchantments{.maxMP = 10, .mpRegen = 2, .spellPower = 4, .magicResist = 1}} },
	{ itemID::ElderFeet, {"Shoes of the Elders", "A pair of shoes made out of Linen,\n                                        imbued with powerful magical essence.", itemRarity::Epic, itemType::Feet, 625, false, armor_stats{"Light",1,2,1}, enchantments{.maxMP = 10, .mpRegen = 2, .spellPower = 4, .magicResist = 1}} },

	//Rings

	{ itemID::RingHealth, {"Ring of Health", "A simple ring to keep it's wearer healthy.", itemRarity::Uncommon, itemType::Ring, 120, false, {}, enchantments{.maxHP = 5}} },
	{ itemID::RingStrength, {"Ring of Strength", "Offense is the greatest defense.", itemRarity::Uncommon, itemType::Ring, 120, false, {}, enchantments{.atk = 1}  } },
	{ itemID::RingMana, {"Ring of Mana", "A simple ring that provides a small amount of energy.", itemRarity::Uncommon, itemType::Ring, 120, false, {}, enchantments{.maxMP = 3}  } },
	{ itemID::RingWillpower, {"Ring of Willpower", "For amateurs of the arcane arts.", itemRarity::Uncommon, itemType::Ring, 120, false, {}, enchantments{.spellPower = 1}  } },
	
	{ itemID::RingAccuracy, {"Ring of Accuracy", "First rule of attacking is to hit something.", itemRarity::Uncommon, itemType::Ring, 200, false, {}, enchantments{.hitChance = 10}  } },
	{ itemID::RingMagicResistance, {"Ring of Magic Resistance", "This simple ring offers small protection against the arcanes.", itemRarity::Uncommon, itemType::Ring, 200, false, {}, enchantments{.magicResist = 1}  } },
	{ itemID::RingDefense, {"Ring of Defense", "Defense is the greatest offense.", itemRarity::Uncommon, itemType::Ring, 200, false, {}, enchantments{.defense = 1}  } },
	{ itemID::RingWarden, {"Warden's Ring", "Authority through resilience.", itemRarity::Rare, itemType::Ring, 380, false, {}, enchantments{.defense = 2, .magicResist = 2}  } },
	{ itemID::RingRessource, {"Ring of the Ressourceful", "Just the essentials.", itemRarity::Rare, itemType::Ring, 380, false, {}, enchantments{.maxHP = 10, .maxMP = 10}  } },

	{ itemID::RingEnergy, {"Ring of Energy", "To help you in your daily spellcasting.", itemRarity::Uncommon, itemType::Ring, 330, false, {}, enchantments{.maxMP = 15}  } },
	{ itemID::RingFortitude, {"Ring of Fortitude", "A good health means a long life.", itemRarity::Uncommon, itemType::Ring, 330, false, {}, enchantments{.maxHP = 15}  } },
	{ itemID::RingMight, {"Ring of Might", "Strength above all.", itemRarity::Uncommon, itemType::Ring, 330, false, {}, enchantments{.atk = 3}  } },
	{ itemID::RingProtection, {"Ring of Protection", "You'll never feel pain again... maybe.", itemRarity::Uncommon, itemType::Ring, 330, false, {}, enchantments{.defense = 3}  } },
	{ itemID::RingSorcery, {"Ring of Sorcery", "Cast even more powerful spells.", itemRarity::Uncommon, itemType::Ring, 330, false, {}, enchantments{.spellPower = 3}  } },
	{ itemID::RingImmunity, {"Ring of Immunity", "Offers even more protection against the arcanes.", itemRarity::Uncommon, itemType::Ring, 330, false, {}, enchantments{.magicResist = 3}  } },

	{ itemID::ScholarRing, {"Scholar's Ring", "For those who seek knowledge.", itemRarity::Rare, itemType::Ring, 720, false, {}, enchantments{.maxMP = 15, .spellPower = 5}  } },
	{ itemID::JuggernautRing, {"Juggernaut's Ring", "Your presence will not go unnoticed.", itemRarity::Rare, itemType::Ring, 720, false, {}, enchantments{.maxHP = 15, .atk = 2}  } },
	{ itemID::RogueRing, {"Rogue's Ring", "They'll never know what hit them.", itemRarity::Rare, itemType::Ring, 720, false, {}, enchantments{.atk = 3, .dodgeChance = 5, .critChance = 5}  } },
	{ itemID::ThornsRing, {"Ring of Thorns", "An eye for an eye.", itemRarity::Rare, itemType::Ring, 720, false, {}, enchantments{.thorns = 4}  } },

	{ itemID::UnbrokenRing, {"Covenant of the Unbroken", "Only few have lived long enough to see it through their own eyes..", itemRarity::Legendary, itemType::Ring, 2500, false, {}, enchantments{.maxHP = 25, .hpRegen = 8, .atk = 5, .defense = 3, .magicResist = 3}  } },
	{ itemID::ElderRing, {"Sigil of the Elders", "Once belonged to one of the most powerful wizard to ever live.", itemRarity::Legendary, itemType::Ring, 2500, false, {}, enchantments{.maxMP = 20, .mpRegen = 5, .spellPower = 8, .magicResist = 1}  } },
	{ itemID::UnseenRing, {"Shadow Master's Ring", "For the most dangerous tasks.", itemRarity::Legendary, itemType::Ring, 2500, false, {}, enchantments{.atk = 5, .hitChance = 5, .dodgeChance = 7, .critChance = 10, .critMult = 0.5} } },

	//Necklaces

	{ itemID::NecklaceVersatility, {"Necklace of Versatility", "Jack of all trades, master of none.", itemRarity::Uncommon, itemType::Necklace, 420, false, {}, enchantments{.maxHP = 5, .maxMP = 5, .atk = 1, .spellPower = 1, .defense = 1, .magicResist = 1}  } },
	{ itemID::NecklaceRuby, {"Ruby Necklace", "A bright red gem.", itemRarity::Rare, itemType::Necklace, 750, false, {}, enchantments{.maxHP = 20, .hpRegen = 2}  } },
	{ itemID::NecklaceSapphire, {"Sapphire Necklace", "A bright blue gem.", itemRarity::Rare, itemType::Necklace, 750, false, {}, enchantments{.maxMP = 20, .mpRegen = 2}  } },
	{ itemID::NecklaceThorns, {"Thornroot Necklace", "An uncomfortable necklace to wear, made of thornroot.", itemRarity::Rare, itemType::Necklace, 750, false, {}, enchantments{.thorns = 8}  } },

	{ itemID::NecklaceUnbroken, {"Chain of the Unbroken", "An uncomfortable necklace to wear, made of thornroot.", itemRarity::Legendary, itemType::Necklace, 3500, false, {}, enchantments{.maxHP = 50, .hpRegen = 12, .atk = 5, .defense = 3, .magicResist = 3}  } },
	{ itemID::NecklaceElder, {"Talisman of the Elders", "An uncomfortable necklace to wear, made of thornroot.", itemRarity::Legendary, itemType::Necklace, 3500, false, {}, enchantments{.maxMP = 45, .mpRegen = 8, .spellPower = 12, .magicResist = 2}  } },

	//Capes
	{ itemID::LeatherCape, {"Leather Cape", "A cape made out of leather, it is slightly worn.", itemRarity::Common, itemType::Cape, 70, false, {}, enchantments{.maxHP = 4, .maxMP = 2}  } },

	{ itemID::ReinforcedLeatherCape, {"Reinforced Leather Cape", "A cape made out of reinforced leather.", itemRarity::Uncommon, itemType::Cape, 200, false, {}, enchantments{.maxHP = 10, .maxMP = 5}  } },
	{ itemID::AdventurerCape, {"Adventurer's Cape", "This cape once belonged to an unknown adventurer.", itemRarity::Rare, itemType::Cape, 400, false, {}, enchantments{.maxHP = 12, .maxMP = 7, .atk = 1, .spellPower = 1}  } },

	{ itemID::ShadowCloak, {"Shadow Cloak", "The assassin's favorite.", itemRarity::Rare, itemType::Cape, 600, false, {}, enchantments{.dodgeChance = 10, .critChance = 5}  } },

	{ itemID::SpellWeaverCape, {"Spell Weaver's Cape", "A mythical cape infused with powerful magical energy.", itemRarity::Legendary, itemType::Cape, 2500, false, {}, enchantments{.maxMP = 35, .mpRegen = 5, .spellPower = 10}  } },
	{ itemID::OverlordCape, {"Overlord's Cape", "The embodiment of true power.", itemRarity::Legendary, itemType::Cape, 2500, false, {}, enchantments{.maxHP = 50, .hpRegen = 7, .atk = 8}  } },
	{ itemID::UnseenCape, {"Master Assassin's Cape", "This cape has been smuggled from a very distant land.", itemRarity::Legendary, itemType::Cape, 2500, false, {}, enchantments{.atk = 12, .hitChance = 10, .dodgeChance = 10, .critChance = 10}  } },

	//Foods ---------------------------------------------------------------------------------------------------------------------------------------

	//hp only (Tier 1: 3$ for +1 / Tier 2: 8$ for +3 / Tier 3: 13$ for +5)
	{ itemID::Bread, {"Bread", "A simple slice of bread.", itemRarity::Common, itemType::Food, 1, true, consumable_stats{"Health", 1}, {} } },
	{ itemID::Cheese, {"Cheese", "A bland piece of cheese.", itemRarity::Common, itemType::Food, 1, true, consumable_stats{"Health", 1}, {} } },
	{ itemID::BoiledEgg, {"Boiled Egg", "A small egg, boiled to perfection.", itemRarity::Common, itemType::Food, 1, true, consumable_stats{"Health", 1}, {} } },
	{ itemID::CookedMeat, {"Cooked Meat", "A nice and warm slice of steak.", itemRarity::Common, itemType::Food, 3, true, consumable_stats{"Health", 4}, {} } },
	{ itemID::HotBroth, {"Hot Broth", "A warm chicken broth.", itemRarity::Common, itemType::Food, 3, true, consumable_stats{"Health", 4}, {} } },
	{ itemID::HealthyStew, {"Healthy Stew", "A well-cooked and healthy vegetable stew.", itemRarity::Uncommon, itemType::Food, 8, true, consumable_stats{"Health", 10}, {} } },
	{ itemID::BakedPotato, {"Baked Potato", "A stuffed potato full of vegetables and meat.", itemRarity::Uncommon, itemType::Food, 8, true, consumable_stats{"Health", 10}, {} } },

	//hp and mana (Tier 1: 7$ for +1 / Tier 2: 18$ for +3 / Tier 3: 28$ for +5)

	{ itemID::Apple, {"Apple", "A simple red apple.", itemRarity::Common, itemType::Food, 2, true, consumable_stats{"HealthMana", 1}, {} } },
	{ itemID::Pear, {"Pear", "A simple Pear.", itemRarity::Common, itemType::Food, 2, true, consumable_stats{"HealthMana", 1}, {} } },
	{ itemID::Mango, {"Mango", "A delicious mango.", itemRarity::Uncommon, itemType::Food, 7, true, consumable_stats{"HealthMana", 4}, {} } },
	{ itemID::Kiwi, {"Kiwi", "A delicious kiwi.", itemRarity::Uncommon, itemType::Food, 7, true, consumable_stats{"HealthMana", 4}, {} } },
	{ itemID::DragonFruit, {"Dragon Fruit", "An exotic and sought-after dragonfruit.", itemRarity::Uncommon, itemType::Food, 12, true, consumable_stats{"HealthMana", 7}, {} } },
	{ itemID::Pomegranate, {"Pomegranate", "An exotic and sought-after pomegranate.", itemRarity::Uncommon, itemType::Food, 12, true, consumable_stats{"HealthMana", 7}, {} } },

	//mana only (Tier 1: 3$ for +1 / Tier 2: 8$ for +3 / Tier 3: 13$ for +5)

	{ itemID::Water, {"Water", "A jug of water.", itemRarity::Common, itemType::Food, 1, true, consumable_stats{"Mana", 1}, {} } },
	{ itemID::Milk, {"Milk", "A refreshing jug of milk.", itemRarity::Common, itemType::Food, 3, true, consumable_stats{"Mana", 4}, {} } },
	{ itemID::Juice, {"Juice", "An energizing fruit juice.", itemRarity::Uncommon, itemType::Food, 8, true, consumable_stats{"Mana", 10}, {} } },


	//SkillBooks --------------------------------------------------------------------------------------------------------------------------------------- Common 50 / Uncommon 300 / Rare 600 / Epic 1200 / Legendary 3000

	//Physical
	{ itemID::SB_PreciseStrike, {"SkillBook - Precise Strike", "Unlocks the following skill: Precise Strike\n\n                                        \"A concentrated attack with slightly increased\n                                        odds to inflict a critical strike.\"", itemRarity::Uncommon, itemType::SkillBook, 300, false, {}, {}, skillID::PreciseStrike} },
	{ itemID::SB_HeavyStrike, {"SkillBook - Heavy Strike", "Unlocks the following skill: Heavy Strike\n\n                                        \"Firmly grip your weapon to perform a more serious physical attack.\"", itemRarity::Rare, itemType::SkillBook, 600, false, {}, {}, skillID::HeavyStrike} },
	{ itemID::SB_UltimateStrike, {"SkillBook - Ultimate Strike", "Unlocks the following skill: Ultimate Strike\n\n                                        \"Deliver the most powerful physical attack.\"", itemRarity::Legendary, itemType::SkillBook, 3000, false, {}, {}, skillID::UltimateStrike} },
	{ itemID::SB_MomentumStrike, {"SkillBook - Momentum Strike", "Unlocks the following skill: Momentum Strike\n\n                                        \"Critically strike your enemy when their guard is down.\"", itemRarity::Epic, itemType::SkillBook, 1400, false, {}, {}, skillID::MomentumStrike} },
	{ itemID::SB_DizzyingBlow, {"SkillBook - Dizzying Blow", "Unlocks the following skill: Dizzying Blow\n\n                                        \"Slam your enemy with your weapon's hilt for a chance to stun them.\"", itemRarity::Uncommon, itemType::SkillBook, 300, false, {}, {}, skillID::DizzyingBlow} },


	//Magical
	{ itemID::SB_BlindingLights, {"SkillBook - Blinding Lights", "Unlocks the following skill: Blinding Lights\n\n                                        \"Stun your opponent with a powerful light that\n                                         also deals a small amount of magical damage.\"", itemRarity::Uncommon, itemType::SkillBook, 300, false, {}, {}, skillID::BlindingLights} },
	{ itemID::SB_Fire, {"SkillBook - Fire", "Unlocks the following skill: Fire\n\n                                        \"Cast a small fireball to shoot at your enemy.\"", itemRarity::Uncommon, itemType::SkillBook, 300, false, {}, {}, skillID::Fire} },
	{ itemID::SB_Heal, {"SkillBook - Heal", "Unlocks the following skill: Heal\n\n                                        \"Use your magical powers to heal yourself.\"", itemRarity::Uncommon, itemType::SkillBook, 300, false, {}, {}, skillID::Heal} },
	
	{ itemID::SB_Renew, {"SkillBook - Renew", "Unlocks the following skill: Renew\n\n                                        \"Reinvigorate your soul.\"", itemRarity::Rare, itemType::SkillBook, 600, false, {}, {}, skillID::Renew} },
	{ itemID::SB_GreaterFire, {"SkillBook - Greater Fire", "Unlocks the following skill: Greater Fire\n\n                                        \"Cast a large fireball to shoot at your enemy.\"", itemRarity::Rare, itemType::SkillBook, 600, false, {}, {}, skillID::GreaterFire} },
	{ itemID::SB_GreaterHeal, {"SkillBook - Greater Heal", "Unlocks the following skill: Greater Heal\n\n                                        \"Use your magical powers to heal yourself even more.\"", itemRarity::Rare, itemType::SkillBook, 600, false, {}, {}, skillID::GreaterHeal} },

	{ itemID::SB_Transfusion, {"SkillBook - Transfusion", "Unlocks the following skill: Transfusion\n\n                                        \"Steal the blood of your enemy.\"", itemRarity::Epic, itemType::SkillBook, 1400, false, {}, {}, skillID::Transfusion} },
	
	//Non Physical or magical
	{ itemID::SB_FirstAid, {"SkillBook - First Aid", "Unlocks the following skill: First Aid\n\n                                        \"When in critical danger, attempt to mend minor wounds.\"", itemRarity::Common, itemType::SkillBook, 50, false, {}, {}, skillID::FirstAid} },
	{ itemID::SB_Meditate, {"SkillBook - Meditate", "Unlocks the following skill: Meditate\n\n                                        \"Calm your mind to restore a small amount of mana.\"", itemRarity::Common, itemType::SkillBook, 50, false, {}, {}, skillID::Meditate} },

	//Buffs / Debuffs
		
	{ itemID::SB_Guarded, {"SkillBook - Guarded", "Unlocks the following skill: Guarded\n\n                                        \"Become more alert to incoming physical attacks.\"", itemRarity::Uncommon, itemType::SkillBook, 300, false, {}, {}, skillID::Guarded} },
	{ itemID::SB_SpiritShield, {"SkillBook - Spirit Shield", "Unlocks the following skill: Spirit Shield\n\n                                        \"Become more alert to incoming magical attacks.\"", itemRarity::Uncommon, itemType::SkillBook, 300, false, {}, {}, skillID::SpiritShield} },
	{ itemID::SB_AmbitiousBlade, {"SkillBook - Ambitious Blade", "Unlocks the following skill: Ambitious Blade\n\n                                        \"Sharpen your blade to increase your odds of inflicting critical blows.\"", itemRarity::Uncommon, itemType::SkillBook, 300, false, {}, {}, skillID::AmbitiousBlade} },
	{ itemID::SB_Perseverence, {"SkillBook - Perseverence", "Unlocks the following skill: Perserverence\n\n                                        \"Sharpen your blade to increase your odds of inflicting critical blows.\"", itemRarity::Uncommon, itemType::SkillBook, 300, false, {}, {}, skillID::Perseverence} },
	{ itemID::SB_Opportunity, {"SkillBook - Opportunity", "Unlocks the following skill: Opportunity\n\n                                        \"If your enemy is stunned, cleanse yourself of all negative ailments.\"", itemRarity::Rare, itemType::SkillBook, 600, false, {}, {}, skillID::Opportunity} },
	{ itemID::SB_CurePoison, {"SkillBook - Cure Poison", "Unlocks the following skill: Cure Poison\n\n                                        \"The antidote to any sickness.\"", itemRarity::Rare, itemType::SkillBook, 600, false, {}, {}, skillID::CurePoison} },
	{ itemID::SB_InnerFocus, {"SkillBook - Inner Focus", "Unlocks the following skill: Inner Focus\n\n                                        \"Focus your mind to increase mana regen.\"", itemRarity::Rare, itemType::SkillBook, 600, false, {}, {}, skillID::InnerFocus} },
	{ itemID::SB_Invisibility, {"SkillBook - Invisibility", "Unlocks the following skill: Invisibility\n\n                                        \"Become untouchable.. or almost.\"", itemRarity::Rare, itemType::SkillBook, 600, false, {}, {}, skillID::Invisibility} },
	{ itemID::SB_Resilience, {"SkillBook - Resilience", "Unlocks the following skill: Resilience\n\n                                        \"Become more resilient to both physical and magical attacks.\"", itemRarity::Rare, itemType::SkillBook, 600, false, {}, {}, skillID::Resilience} },
	{ itemID::SB_EmpoweredMind, {"SkillBook - Empowered Mind", "Unlocks the following skill: Empowered Mind\n\n                                        \"Empower your mind, making your spells even stronger.\"", itemRarity::Rare, itemType::SkillBook, 600, false, {}, {}, skillID::EmpoweredMind} },
	{ itemID::SB_Berserk, {"SkillBook - Berserk", "Unlocks the following skill: Berserk\n\n                                        \"Blood boils, anger grows, attacks hit harder.\"", itemRarity::Rare, itemType::SkillBook, 600, false, {}, {}, skillID::Berserk} },

	{ itemID::SB_Vampirism, {"SkillBook - Vampirism", "Unlocks the following skill: Vampirism\n\n                                        \"Bloodlusty spells.\"", itemRarity::Epic, itemType::SkillBook, 1400, false, {}, {}, skillID::Vampirism} },

	{ itemID::SB_EverlastingWill, {"SkillBook - Everlasting Will", "Unlocks the following skill: Everlasting Will\n\n                                        \"Unleash your greatest magical powers.\"", itemRarity::Legendary, itemType::SkillBook, 2500, false, {}, {}, skillID::EverlastingWill} },


	//Mob Loot ---------------------------------------------------------------------------------------------------------------------------------------

	{ itemID::RatTail, {"Rat Tail", "The cut tail of a dirty rat.", itemRarity::Common, itemType::Loot, 2, true, {}, {}} },
	{ itemID::BoarSnout, {"Boar Snout", "The slimey snout of a dead boar.", itemRarity::Common, itemType::Loot, 4, true, {}, {}} },
	{ itemID::WolfPelt, {"Wolf Pelt", "The clean pelt of a hungry wolf.", itemRarity::Common, itemType::Loot, 8, true, {}, {}} },
	{ itemID::BrokenDagger, {"Broken Dagger", "A broken dagger that once belonged to a bad person.\n                                         It's engravings makes it somewhat valuable.", itemRarity::Common, itemType::Loot, 12, true, {}, {}} },
	{ itemID::BoneAshes, {"Bone Ashes", "Ashes that once belonged to something.. well not alive.", itemRarity::Common, itemType::Loot, 16, true, {}, {}} },
	{ itemID::RottenFlesh, {"Rotten Flesh", "A piece of flesh that carries a putrid smell.\n                                         It once belonged to a ghoul, this might make it somewhat valuable.", itemRarity::Common, itemType::Loot, 16, true, {}, {}} },
	{ itemID::GhostEssence, {"Ghostly Essence", "The purified essence of a lost soul.", itemRarity::Uncommon, itemType::Loot, 30, true, {}, {}} },
	{ itemID::BundleSilk, {"Bundle of Silk", "Produced by the largest of spiders.", itemRarity::Common, itemType::Loot, 24, true, {}, {}} },
	{ itemID::MossyGoo, {"Mossy Goo", "Although disgusting, merchants would gladly pay for it.", itemRarity::Common, itemType::Loot, 30, true, {}, {}} },
	{ itemID::Thornroot, {"Thornroot", "A poisonous thornroot, very rare and hard to find.", itemRarity::Uncommon, itemType::Loot, 50, true, {}, {}} },
	{ itemID::GiantFeather, {"Giant Feather", "A large black feather.", itemRarity::Common, itemType::Loot, 38, true, {}, {}} },


	//Potions ----------------------------------------------------------------------------------------------------------------------------------------

	//Health Pots
	{ itemID::HPPhial, {"Health Phial", "A small phial containing a red liquid.", itemRarity::Uncommon, itemType::Potion, 15, true, consumable_stats{"Health", 10}, {}} },
	{ itemID::HPPot, {"Health Potion", "A potion containing a red liquid.", itemRarity::Uncommon, itemType::Potion, 125, true, consumable_stats{"Health", 25}, {}}},
	{ itemID::HPTonic, {"Health Tonic", "A flask containing a highly concentrated red liquid.", itemRarity::Rare, itemType::Potion, 300, true, consumable_stats{"Health", 50}, {}} },

	//Mana Pots
	{ itemID::MPPhial, {"Mana Phial", "A small phial containing a blue liquid.", itemRarity::Uncommon, itemType::Potion, 15, true, consumable_stats{"Mana", 10}, {}} },
	{ itemID::MPPot, {"Mana Potion", "A potion containing a blue liquid.", itemRarity::Uncommon, itemType::Potion, 125, true, consumable_stats{"Mana", 25}, {}} },
	{ itemID::MPTonic, {"Mana Tonic", "A flask containing a highly concentrated blue liquid.", itemRarity::Rare, itemType::Potion, 300, true, consumable_stats{"Mana", 50}, {}} },

	//Buff Pots TurnCount is always +1, because of how tickdown works in combat.cpp

	{ itemID::StrPhial, {"Strength Phial", "A small phial containing a orange liquid.", itemRarity::Uncommon, itemType::BuffPotion, 75, true, consumable_buffs{.buffs{.atk = 2}, .turns = 4}} },
	{ itemID::StrPot, {"Strength Potion", "A potion containing a orange liquid.", itemRarity::Rare, itemType::BuffPotion, 250, true, consumable_buffs{.buffs{.atk = 5}, .turns = 4}} },
	{ itemID::StrTonic, {"Strength Tonic", "A flask containing a highly concentrated orange liquid.", itemRarity::Epic, itemType::BuffPotion, 520, true, consumable_buffs{.buffs{.atk = 10}, .turns = 4}} },

	{ itemID::SpellPowerPhial, {"Magical Phial", "A small phial containing a purple liquid.", itemRarity::Uncommon, itemType::BuffPotion, 75, true, consumable_buffs{.buffs{.spellPower = 3}, .turns = 4}} },
	{ itemID::SpellPowerPot, {"Magical Power Potion", "A potion containing a purple liquid.", itemRarity::Rare, itemType::BuffPotion, 250, true, consumable_buffs{.buffs{.spellPower = 7}, .turns = 4}} },
	{ itemID::SpellPowerTonic, {"Magical Power Tonic", "A flask containing a highly concentrated purple liquid.", itemRarity::Epic, itemType::BuffPotion, 520, true, consumable_buffs{.buffs{.spellPower = 13}, .turns = 4}} },

	{ itemID::ThornsPhial, {"Thorns Phial", "A small phial containing a brown liquid.", itemRarity::Uncommon, itemType::BuffPotion, 75, true, consumable_buffs{.buffs{.thorns = 3}, .turns = 6}} },
	{ itemID::ThornsPot, {"Thorns Power Potion", "A potion containing a brown liquid.", itemRarity::Rare, itemType::BuffPotion, 250, true, consumable_buffs{.buffs{.thorns = 5}, .turns = 6}} },
	{ itemID::ThornsTonic, {"Thorns Power Tonic", "A flask containing a highly concentrated brown liquid.", itemRarity::Epic, itemType::BuffPotion, 520, true, consumable_buffs{.buffs{.thorns = 10}, .turns = 6}} },

	{ itemID::HPRegenPhial, {"Rejuvinating Phial", "A small phial containing a red liquid.", itemRarity::Uncommon, itemType::BuffPotion, 75, true, consumable_buffs{.buffs{.hpRegen = 4}, .turns = 6}} },
	{ itemID::HPRegenPot, {"Rejuvinating Potion", "A potion containing a red liquid.", itemRarity::Rare, itemType::BuffPotion, 250, true, consumable_buffs{.buffs{.hpRegen = 8}, .turns = 6}} },
	{ itemID::HPRegenTonic, {"Rejuvinating Tonic", "A flask containing a highly concentrated red liquid.", itemRarity::Epic, itemType::BuffPotion, 520, true, consumable_buffs{.buffs{.hpRegen = 15}, .turns = 6}} },

	{ itemID::MPRegenPhial, {"Energy Phial", "A small phial containing a blue liquid.", itemRarity::Uncommon, itemType::BuffPotion, 75, true, consumable_buffs{.buffs{.mpRegen = 4}, .turns = 6}} },
	{ itemID::MPRegenPot, {"Energy Potion", "A potion containing a blue liquid.", itemRarity::Rare, itemType::BuffPotion, 250, true, consumable_buffs{.buffs{.mpRegen = 8}, .turns = 6}} },
	{ itemID::MPRegenTonic, {"Energy Tonic", "A flask containing a highly concentrated blue liquid.", itemRarity::Epic, itemType::BuffPotion, 520, true, consumable_buffs{.buffs{.mpRegen = 15}, .turns = 6}} },
		
};

