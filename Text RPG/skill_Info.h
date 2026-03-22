#pragma once
#include <iostream>

#include "skill_formulas.h"
#include "skill_ID.h"
#include "skill_dataBase.h"
#include "Utility.h"
#include "Player.h"
using namespace std;

//"\033[38;5;209m" = Salmon -> Physical Damage
//"\033[38;5;141m" = Purple Magenta -> Magical Damage
//"\033[38;5;196m" = Red -> Critical Chance / Critical Damage
//"\x1b[32m" = Green -> Health
//"\033[38;5;117m" = Blue Sky -> Mana

//"\x1b[37m" = Clear Code

inline void displaySkillCooldown(skillID ID)
{
	baseSkill skill = skillDataBase[ID];

	cout << "[Cooldown] ";

	if (skill.cd != 99 && skill.cd != 0)
		cout << skill.cd << " turns";
	else if (skill.cd == 99)
		cout << "One-time use";
	else if (skill.cd == 0)
		cout << "Spammable";


	cout << endl << endl << "                                     ";
}

inline void skillInfo_Desc(Player& player, skillID skill) //Prints the description for every skill. 
{
	switch (skill)
	{
		//Physical

		case skillID::Attack:
			cout << "Deals " << typeColor("Physical") << skillFormula(player, skill) << " physical damage" << clearColor() << ".";
			return;
		case skillID::HeavyStrike:
			cout << "Deals " << typeColor("Physical") << skillFormula(player, skill) << " physical damage" << clearColor() << ".";
			return;
		case skillID::PreciseStrike:
			cout << "Deals " << typeColor("Physical") << (int)skillFormula(player, skill) << " physical damage" << clearColor() << ".";
			cout << "\n                                     Has a " << "\033[38;5;196m" << player.get_critChance() + 15 << "% chance " << clearColor() << "to inflict a" << "\033[38;5;196m" << " critical strike" << clearColor() << ".";
			return;
		case skillID::UltimateStrike:
			cout << "Deals " << typeColor("Physical") << skillFormula(player, skill) << " physical damage" << clearColor() << ".";
			return;
		case skillID::MomentumStrike:
			cout << "Deals " << typeColor("Physical") << skillFormula(player, skill) << " physical damage" << clearColor() << ".";
			cout << "\n                                     Guaranteed " << typeColor("Critical") << "critical strike " << clearColor() << "if the enemy is " << typeColor("Stun") << "stunned" << clearColor() << ".";
			return;
		case skillID::DizzyingBlow:
			cout << "Deals " << typeColor("Physical") << skillFormula(player, skill) << " physical damage" << clearColor() << ".";
			cout << "\n                                     Has a 40% chance to " << typeColor("Stun") << "stun " << clearColor() << "the enemy.";
			return;



		//Magical

		case skillID::Heal:
			cout << "Restores " << typeColor("Health") << skillFormula(player, skill) << " health" << clearColor() << ".";
			return;
		case skillID::Fire:
			cout << "Deals " << typeColor("Magical") << skillFormula(player, skill) << " magical damage" << clearColor() << ".";
			return;
		case skillID::BlindingLights:
			cout << "Deals " << typeColor("Magical") << skillFormula(player, skill) << " magical damage" << clearColor() << ".";
			cout << "\n                                     Has a 40% chance to " << typeColor("Stun") << "stun " << clearColor() << "the enemy.";
			return;
		case skillID::Renew:
		{
			baseSkill skill = skillDataBase[skillID::Renew];
			skillBuff b = *skill.b;
			buff buffs = b.buffs;

			cout << "Restores " << typeColor("Health") << skillFormula(player, skillID::Renew) << " health" << clearColor() << " and applies " << typeColor("Health") << buffs.hpRegen << " health regen " << clearColor() << "for " << b.turns-1 << " turns.";
			return;
		}
		case skillID::GreaterHeal:
			cout << "Restores " << typeColor("Health") << skillFormula(player, skill) << " health" << clearColor() << ".";
			return;
		case skillID::GreaterFire:
			cout << "Deals " << typeColor("Magical") << skillFormula(player, skill) << " magical damage" << clearColor() << ".";
			return;
		case skillID::Transfusion:
			cout << "Deals " << typeColor("Magical") << skillFormula(player, skill) << " magical damage " << clearColor() << "and " << typeColor("Health") << "heals " << clearColor() << "for the same amount.";
			return;





		//Non Physical || Magical

		case skillID::FirstAid:
			cout << "Restores " << "\x1b[32m" << skillFormula(player, skill) << " health" << clearColor() << ".";
			cout << "\n                                     This skill only works when below " << typeColor("Health") << "30% health" << clearColor() << ".";
			return;

		case skillID::Meditate:
			cout << "Restores " << typeColor("Mana") << skillFormula(player, skill) << " mana" << clearColor() << ".";
			return;


		//Buff

		case skillID::Guarded:
		{
			baseSkill skill = skillDataBase[skillID::Guarded];
			skillBuff b = *skill.b;

			cout << "Raises defense by 2 for " << b.turns-1 << " turns.";
			return;
		}

		case skillID::SpiritShield:
		{
			baseSkill skill = skillDataBase[skillID::SpiritShield];
			skillBuff b = *skill.b;

			cout << "Raises magic resist by 2 for " << b.turns-1 << " turns.";
			return;
		}

		case skillID::AmbitiousBlade:
		{
			baseSkill skill = skillDataBase[skillID::AmbitiousBlade];
			skillBuff b = *skill.b;

			cout << "Raises " << typeColor("Critical") << "critical chance " << clearColor() << "by 10% for " << b.turns-1 << " turns.";
			return;
		}

		case skillID::Perseverence:
		{
			baseSkill skill = skillDataBase[skillID::Perseverence];
			skillBuff b = *skill.b;
			buff buffs = b.buffs;

			cout << "Restore " << typeColor("Health") << buffs.hpRegen << " health " << clearColor() << "at the start of your turn for " << b.turns-1 << " turns.";
			return;
		}

		case skillID::Opportunity:
		{
			baseSkill skill = skillDataBase[skillID::Opportunity];
			skillBuff b = *skill.b;
			buff buffs = b.buffs;

			cout << "Cleanse yourself of any negative ailments. This skill only works if the enemy is currently " << typeColor("Stun") << "stunned" << clearColor() << ".";
			return;
		}

		case skillID::CurePoison:
		{
			baseSkill skill = skillDataBase[skillID::CurePoison];

			cout << "This skill cures the " << typeColor("Poison") << "poison " << clearColor() << "status effect";
			return;
		}

		case skillID::InnerFocus:
		{
			baseSkill skill = skillDataBase[skillID::InnerFocus];
			skillBuff b = *skill.b;
			buff buffs = b.buffs;

			cout << "Restore " << typeColor("Mana") << buffs.mpRegen << " mana " << clearColor() << "at the start of your turn for " << b.turns - 1 << " turns.";
			return;
		}

		case skillID::EverlastingWill:
		{
			baseSkill skill = skillDataBase[skillID::EverlastingWill];
			skillBuff b = *skill.b;
			buff buffs = b.buffs;

			cout << "Gain " << typeColor("Mana") << buffs.mpRegen << " mana regen" << clearColor() << " and " << typeColor("Magical") << buffs.spellPower << " magical power" << clearColor() << " for the rest of combat.";
			return;
		}

		case skillID::Vampirism:
		{
			baseSkill skill = skillDataBase[skillID::Vampirism];
			skillBuff b = *skill.b;

			cout << "Raises spell vamp by 20% for the rest of combat.";
			return;
		}

		case skillID::Invisibility:
		{
			baseSkill skill = skillDataBase[skillID::Invisibility];
			skillBuff b = *skill.b;

			cout << "Raises dodge chance by 15% for " << b.turns - 1 << " turns.";
			return;
		}

		case skillID::Resilience:
		{
			baseSkill skill = skillDataBase[skillID::Resilience];
			skillBuff b = *skill.b;

			cout << "Raises defense and magic resist by 3 for the rest of combat.";
			return;
		}

		case skillID::EmpoweredMind:
		{
			baseSkill skill = skillDataBase[skillID::EmpoweredMind];
			skillBuff b = *skill.b;

			cout << "Raises " << typeColor("Magical") << "magical power " << clearColor() << "by 5 for " << b.turns - 1 << " turns.";
			return;
		}

		case skillID::Berserk:
		{
			baseSkill skill = skillDataBase[skillID::Berserk];
			skillBuff b = *skill.b;

			cout << "Raises " << typeColor("Physical") << "physical power " << clearColor() << "by 5 for " << b.turns - 1 << " turns.";
			return;
		}

	}
}