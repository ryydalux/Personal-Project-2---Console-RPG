#pragma once

#include <optional>
#include <unordered_map>
#include <iostream>

#include "skill_ID.h"
#include "buffs.h"

enum class skillSource
{
	None, //For any skill that doesn't scale with atk or spellPower
	Physical,
	Magical,
};

enum class skillType
{
	None,

	Damage,
	Heal,
	Buff,
};

enum class skillRarity
{
	None,

	Common,
	Uncommon,
	Rare,
	Epic,
	Legendary
};

struct skillCost
{
	int mpCost{ 0 };
	int hpCost{ 0 };
};

struct skillBuff
{
	buff buffs;
	int turns;
};

struct baseSkill
{
	string name;
	string description;
	skillSource source;
	skillType type;
	skillRarity rarity;
	skillCost cost; //Could be Mana or/and HP
	int cd; //skill's cooldown. If 0 = useable every turn.

	optional<skillBuff> b;
};

inline unordered_map<skillID, baseSkill> skillDataBase
{
	{skillID::None, {"", "", skillSource::None, skillType::None, skillRarity::None, skillCost{0,0}, 0}},

	//Physical Skills

	{skillID::Attack, {"Attack", "A standard attack.", skillSource::Physical, skillType::Damage, skillRarity::Common, skillCost{0,0}, 0}},
	{skillID::PreciseStrike, {"Precise Strike", "A concentrated attack with slightly increased\n                                        odds to inflict a critical strike.", skillSource::Physical, skillType::Damage, skillRarity::Uncommon, skillCost{6,0}, 0}},
	{skillID::HeavyStrike, {"Heavy Strike", "Firmly grip your weapon to perform\n                                     a more serious physical attack.", skillSource::Physical, skillType::Damage, skillRarity::Rare, skillCost{8,0}, 0}},
	{skillID::UltimateStrike, {"Ultimate Strike", "Deliver the most powerful physical attack.", skillSource::Physical, skillType::Damage, skillRarity::Legendary, skillCost{14,0}, 0}},
	{skillID::MomentumStrike, {"Momentum Strike", "Critically strike your enemy when their guard is down", skillSource::Physical, skillType::Damage, skillRarity::Epic, skillCost{10,0}, 0}},
	{skillID::DizzyingBlow, {"Dizzying Blow", "Slam your enemy with your weapon's hilt for a chance to stun them.", skillSource::Physical, skillType::Damage, skillRarity::Uncommon, skillCost{6,0}, 5}},
	
	
	//Magical Skills

	{skillID::Heal, {"Heal", "Use your magical powers to heal yourself.", skillSource::Magical, skillType::Heal, skillRarity::Uncommon, skillCost{8,0}, 0}},
	{skillID::Fire, {"Fire", "Cast a small fireball to shoot at your enemy.", skillSource::Magical, skillType::Damage, skillRarity::Uncommon, skillCost{8,0}, 0}},
	{skillID::BlindingLights, {"Blinding Lights", "Blind your opponent with a powerful light.", skillSource::Magical, skillType::Damage, skillRarity::Uncommon, skillCost{6,0}, 5}},
	{skillID::Renew, {"Renew", "Reinvigorate your soul.", skillSource::None, skillType::Heal, skillRarity::Rare, skillCost{14,0}, 10, skillBuff{{.hpRegen = 3}, 6}}},

	{skillID::GreaterHeal, {"Greater Heal", "Use your magical powers to heal yourself even more.", skillSource::Magical, skillType::Heal, skillRarity::Rare, skillCost{16,0}, 0}},
	{skillID::GreaterFire, {"Greater Fire", "Cast a large fireball to shoot at your enemy.", skillSource::Magical, skillType::Damage, skillRarity::Rare, skillCost{16,0}, 0}},

	{skillID::Transfusion, {"Transfusion", "Steal the blood of your enemy.", skillSource::Magical, skillType::Damage, skillRarity::Epic, skillCost{16,0}, 0}},

	//None Physical/Magical skills
	{skillID::FirstAid, {"First Aid", "When in critical danger, attempt to mend minor wounds.", skillSource::None, skillType::Heal, skillRarity::Common, skillCost{4,0}, 99}},
	{skillID::Meditate, {"Meditate", "Calm your mind to restore a small amount of mana.", skillSource::None, skillType::Heal, skillRarity::Common, skillCost{0,0}, 99}},

	//Buffing Skills
	{skillID::Guarded, {"Guarded", "Become more alert to incoming attacks.", skillSource::None, skillType::Buff, skillRarity::Uncommon, skillCost{10,0}, 20, skillBuff{{.defense = 2}, 11}}},
	{skillID::SpiritShield, {"Spirit Shield", "Become more alert to incoming magical attacks.", skillSource::None, skillType::Buff, skillRarity::Uncommon, skillCost{10,0}, 20, skillBuff{{.magicResist = 2}, 11}}},
	{skillID::AmbitiousBlade, {"Ambitious Blade", "Sharpen your blade to increase your odds of inflicting critical blows.", skillSource::None, skillType::Buff, skillRarity::Uncommon, skillCost{8,0}, 20, skillBuff{{.critChance = 10}, 11}}},
	{skillID::Perseverence, {"Perseverence", "Become perseverent, gaining health at the start of your turn.", skillSource::None, skillType::Buff, skillRarity::Uncommon, skillCost{10,0}, 20, skillBuff{{.hpRegen = 2}, 11}}},
	
	{skillID::EmpoweredMind, {"Empowered Mind", "Empower your mind, making your spells even stronger.", skillSource::None, skillType::Buff, skillRarity::Rare, skillCost{16,0}, 20, skillBuff{{.spellPower = 5}, 11}}},
	{skillID::Opportunity, {"Opportunity", "If your enemy is stunned, cleanse yourself of all negative ailments.", skillSource::None, skillType::Buff, skillRarity::Rare, skillCost{22,0}, 99}},
	{skillID::CurePoison, {"Cure Poison", "The antidote to any sickness.", skillSource::None, skillType::Buff, skillRarity::Rare, skillCost{18,0}, 99}},
	{skillID::InnerFocus, {"Inner Focus", "Focus your mind to increase mana regen.", skillSource::None, skillType::Buff, skillRarity::Rare, skillCost{16,0}, 20, skillBuff{{.mpRegen = 6}, 11}}},
	{skillID::Invisibility, {"Invisibility", "Become untouchable.. or almost.", skillSource::None, skillType::Buff, skillRarity::Rare, skillCost{16,0}, 20, skillBuff{{.dodgeChance = 15}, 11}}},
	{skillID::Resilience, {"Resilience", "Become more resilient to both physical and magical attacks.", skillSource::None, skillType::Buff, skillRarity::Rare, skillCost{18,0}, 99, skillBuff{{.defense = 3, .magicResist = 3}, 99}}},
	{skillID::Berserk, {"Berserk", "Blood boils, anger grows, attacks hit harder.", skillSource::None, skillType::Buff, skillRarity::Rare, skillCost{16,0}, 20, skillBuff{{.atk = 5}, 11}}},

	{skillID::Vampirism, {"Vampirism", "Bloodlusty spells.", skillSource::None, skillType::Buff, skillRarity::Epic, skillCost{22,0}, 99, skillBuff{{.spellVamp = 20}, 99}}},

	{skillID::EverlastingWill, {"Everlasting Will", "Unleash your greatest magical powers.", skillSource::None, skillType::Buff, skillRarity::Legendary, skillCost{24,0}, 99, skillBuff{{.mpRegen = 8, .spellPower = 15}, 99}}},


	//Debuffing Skills
};


