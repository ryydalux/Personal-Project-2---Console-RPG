#pragma once

#include "buffs.h"

inline void useSkill(Player& player, baseMob*& enemyMob, skillID skill, vector<string>& logs) //Skill logic for combat (used by combat.cpp)
{
	switch (skill)
	{
		//PHYSICAL ------------------------------------------------------------------------

		case skillID::Attack:
		{
			bool hit = checkMiss(player, enemyMob);

			if (hit == true)
			{
				bool crit = rollCrit(player, 0);
				int dmg = (skillFormula(player, skill));

				if (crit == true)
					dmg *= player.get_critMult();

				dmg -= enemyMob->get_defense();

				if (dmg <= 0)
					dmg = 1;

				enemyMob->sub_HP(dmg);

				string log;

				sortLogs(logs);

				if (crit == true)
					log = player.get_name() + " attacks and deals " + typeColor("Critical") + to_string(dmg) + typeColor("Physical") + " physical damage" + clearColor() + "!";
				else
					log = player.get_name() + " attacks and deals " + to_string(dmg) + typeColor("Physical") + " physical damage" + clearColor() + "!";

				logs.push_back(log);

				checkOnHit(player, enemyMob, logs, skill, dmg);
			}
			else
			{
				sortLogs(logs);
				string log = player.get_name() + " misses!";
				logs.push_back(log);
			}
		}
		break;

		case skillID::HeavyStrike:
		{
			baseSkill s = skillDataBase[skill];
			skillCost cost = s.cost;
			bool hit = checkMiss(player, enemyMob);

			if (hit == true)
			{
				bool crit = rollCrit(player, 0);
				int dmg = (skillFormula(player, skill));

				if (crit == true)
					dmg *= player.get_critMult();

				dmg -= enemyMob->get_defense();

				if (dmg <= 0)
					dmg = 1;

				enemyMob->sub_HP(dmg);

				player.sub_MP(cost.mpCost);
				player.sub_HP(cost.hpCost);

				string log;

				sortLogs(logs);

				if (crit == true)
					log = player.get_name() + "uses " + get_skillColor(s) + s.name + clearColor() + " and deals " + typeColor("Critical") + to_string(dmg) + typeColor("Physical") + " physical damage" + clearColor() + "!";
				else
					log = player.get_name() + "uses " + get_skillColor(s) + s.name + clearColor() + " and deals " + to_string(dmg) + typeColor("Physical") + " physical damage" + clearColor() + "!";

				logs.push_back(log);

				checkOnHit(player, enemyMob, logs, skill, dmg);
			}
			else
			{
				sortLogs(logs);
				string log = player.get_name() + "uses " + get_skillColor(s) + s.name + clearColor() + " but misses!";
				logs.push_back(log);
			}
		}
		break;

		case skillID::UltimateStrike:
		{
			baseSkill s = skillDataBase[skill];
			skillCost cost = s.cost;
			bool hit = checkMiss(player, enemyMob);

			if (hit == true)
			{
				bool crit = rollCrit(player, 0);
				int dmg = (skillFormula(player, skill));

				if (crit == true)
					dmg *= player.get_critMult();

				dmg -= enemyMob->get_defense();

				if (dmg <= 0)
					dmg = 1;

				enemyMob->sub_HP(dmg);

				player.sub_MP(cost.mpCost);
				player.sub_HP(cost.hpCost);

				string log;

				sortLogs(logs);

				if (crit == true)
					log = player.get_name() + "uses " + get_skillColor(s) + s.name + clearColor() + " and deals " + typeColor("Critical") + to_string(dmg) + typeColor("Physical") + " physical damage" + clearColor() + "!";
				else
					log = player.get_name() + "uses " + get_skillColor(s) + s.name + clearColor() + " and deals " + to_string(dmg) + typeColor("Physical") + " physical damage" + clearColor() + "!";

				logs.push_back(log);

				checkOnHit(player, enemyMob, logs, skill, dmg);
			}
			else
			{
				sortLogs(logs);
				string log = player.get_name() + "uses " + get_skillColor(s) + s.name + clearColor() + " but misses!";
				logs.push_back(log);
			}
		}
		break;

		case skillID::PreciseStrike:
		{
			baseSkill s = skillDataBase[skill];
			skillCost cost = s.cost;
			bool hit = checkMiss(player, enemyMob);

			if (hit == true)
			{
				bool crit = rollCrit(player, 15);
				int dmg = (skillFormula(player, skill));

				if (crit == true)
					dmg *= player.get_critMult();

				dmg -= enemyMob->get_defense();

				if (dmg <= 0)
					dmg = 1;

				enemyMob->sub_HP(dmg);

				player.sub_MP(cost.mpCost);
				player.sub_HP(cost.hpCost);

				string log;

				sortLogs(logs);

				if (crit == true)
					log = player.get_name() + "uses " + get_skillColor(s) + s.name + clearColor() + " and deals " + typeColor("Critical") + to_string(dmg) + typeColor("Physical") + " physical damage" + clearColor() + "!";
				else
					log = player.get_name() + "uses " + get_skillColor(s) + s.name + clearColor() + " and deals " + to_string(dmg) + typeColor("Physical") + " physical damage" + clearColor() + "!";

				logs.push_back(log);

				checkOnHit(player, enemyMob, logs, skill, dmg);
			}
			else
			{
				sortLogs(logs);
				string log = player.get_name() + "uses " + get_skillColor(s) + s.name + clearColor() + " but misses!";
				logs.push_back(log);
			}
		}
		break;

		case skillID::MomentumStrike:
		{
			baseSkill s = skillDataBase[skill];
			skillCost cost = s.cost;
			bool hit = checkMiss(player, enemyMob);

			if (hit == true)
			{
				bool crit;

				if (enemyMob->stunned == true)
					crit = rollCrit(player, 100);
				else
					crit = rollCrit(player, 0);

				int dmg = (skillFormula(player, skill));

				if (crit == true)
					dmg *= player.get_critMult();

				dmg -= enemyMob->get_defense();

				if (dmg <= 0)
					dmg = 1;

				enemyMob->sub_HP(dmg);

				player.sub_MP(cost.mpCost);
				player.sub_HP(cost.hpCost);

				string log;

				sortLogs(logs);

				if (crit == true)
					log = player.get_name() + "uses " + get_skillColor(s) + s.name + clearColor() + " and deals " + typeColor("Critical") + to_string(dmg) + typeColor("Physical") + " physical damage" + clearColor() + "!";
				else
					log = player.get_name() + "uses " + get_skillColor(s) + s.name + clearColor() + " and deals " + to_string(dmg) + typeColor("Physical") + " physical damage" + clearColor() + "!";

				logs.push_back(log);

				checkOnHit(player, enemyMob, logs, skill, dmg);
			}
			else
			{
				sortLogs(logs);
				string log = player.get_name() + "uses " + get_skillColor(s) + s.name + clearColor() + " but misses!";
				logs.push_back(log);
			}
		}
		break;

		case skillID::DizzyingBlow:
		{
			baseSkill s = skillDataBase[skill];
			skillCost cost = s.cost;
			bool hit = checkMiss(player, enemyMob);

			if (hit == true)
			{
				bool crit = rollCrit(player, 0);

				int dmg = (skillFormula(player, skill));

				if (crit == true)
					dmg *= player.get_critMult();

				dmg -= enemyMob->get_defense();

				if (dmg <= 0)
					dmg = 1;

				enemyMob->sub_HP(dmg);

				player.sub_MP(cost.mpCost);
				player.sub_HP(cost.hpCost);

				string log;

				sortLogs(logs);

				if (crit == true)
					log = player.get_name() + "uses " + get_skillColor(s) + s.name + clearColor() + " and deals " + typeColor("Critical") + to_string(dmg) + typeColor("Physical") + " physical damage" + clearColor() + "!";
				else
					log = player.get_name() + "uses " + get_skillColor(s) + s.name + clearColor() + " and deals " + to_string(dmg) + typeColor("Physical") + " physical damage" + clearColor() + "!";

				logs.push_back(log);

				int roll = rand() % 100 + 1;

				if (roll <= 40)
					enemyMob->stunned = true;

				checkOnHit(player, enemyMob, logs, skill, dmg);
			}
			else
			{
				sortLogs(logs);
				string log = player.get_name() + "uses " + get_skillColor(s) + s.name + clearColor() + " but misses!";
				logs.push_back(log);
			}
		}
		break;


		//MAGICAL ------------------------------------------------------------------------

		case skillID::Heal:
		{
			baseSkill skill = skillDataBase[skillID::Heal];
			skillCost cost = skill.cost;

			player.sub_MP(cost.mpCost);
			player.sub_HP(cost.hpCost);

			player.add_HP(skillFormula(player, skillID::Heal));

			if (player.get_HP() > player.get_maxHP())
				player.set_HP(player.get_maxHP());

			string log = player.get_name() + " uses " + get_skillColor(skill) + skill.name + clearColor() + " and restores " + to_string(skillFormula(player, skillID::Heal)) + typeColor("Health") + " health" + clearColor() + "!";
			logs.push_back(log);
		}
		break;

		case skillID::Fire:
		{
			baseSkill skill = skillDataBase[skillID::Fire];
			skillCost cost = skill.cost;

			player.sub_MP(cost.mpCost);
			player.sub_HP(cost.hpCost);

			int dmg = skillFormula(player, skillID::Fire) - enemyMob->get_magicResist();

			if (dmg <= 0)
				dmg = 1;

			enemyMob->sub_HP(dmg);

			string log = player.get_name() + " uses " + get_skillColor(skill) + skill.name + clearColor() + " and deals " + to_string(dmg) + typeColor("Magical") + " magical damage" + clearColor() + "!";
			logs.push_back(log);

			checkOnHit(player, enemyMob, logs, skillID::Fire, dmg);
		}
		break;

		case skillID::BlindingLights:
		{
			baseSkill skill = skillDataBase[skillID::BlindingLights];
			skillCost cost = skill.cost;

			player.sub_MP(cost.mpCost);
			player.sub_HP(cost.hpCost);

			int dmg = skillFormula(player, skillID::BlindingLights) - enemyMob->get_magicResist();

			if (dmg <= 0)
				dmg = 1;

			enemyMob->sub_HP(dmg);

			string log = player.get_name() + " uses " + get_skillColor(skill) + skill.name + clearColor() + " and deals " + to_string(dmg) + typeColor("Magical") + " magical damage" + clearColor() + "!";
			logs.push_back(log);

			int roll = rand() % 100 + 1;

			if (roll <= 40)
				enemyMob->stunned = true;

			checkOnHit(player, enemyMob, logs, skillID::BlindingLights, dmg);
		}
		break;

		case skillID::Renew:
		{
			baseSkill skill = skillDataBase[skillID::Renew];
			skillCost cost = skill.cost;
			skillBuff buffs = *skill.b;

			buff b = buffs.buffs;
			int turns = buffs.turns;

			player.sub_MP(cost.mpCost);
			player.sub_HP(cost.hpCost);

			int heal = skillFormula(player, skillID::Renew);

			player.add_HP(heal);
			applyBuffSkill(player, buffs);

			if (player.get_HP() > player.get_maxHP())
				player.set_HP(player.get_maxHP());

			player.pbuffs.push_back(b);
			player.pbuffsTimer.push_back(turns);

			string log = player.get_name() + " uses " + get_skillColor(skill) + skill.name + clearColor() + " and heals for " + to_string(heal) + typeColor("Health") + " health" + clearColor() + "!";
			logs.push_back(log);

			updateLogs(player, enemyMob, logs);

			log = player.get_name() + " also applies " + to_string(b.hpRegen) + typeColor("Health") + " health regen" + clearColor() + "!";
			logs.push_back(log);
		}
		break;

		case skillID::GreaterHeal:
		{
			baseSkill skill = skillDataBase[skillID::GreaterHeal];
			skillCost cost = skill.cost;

			player.sub_MP(cost.mpCost);
			player.sub_HP(cost.hpCost);

			player.add_HP(skillFormula(player, skillID::GreaterHeal));

			if (player.get_HP() > player.get_maxHP())
				player.set_HP(player.get_maxHP());

			string log = player.get_name() + " uses " + get_skillColor(skill) + skill.name + clearColor() + " and restores " + to_string(skillFormula(player, skillID::Heal)) + typeColor("Health") + " health" + clearColor() + "!";
			logs.push_back(log);
		}
		break;

		case skillID::GreaterFire:
		{
			baseSkill skill = skillDataBase[skillID::GreaterFire];
			skillCost cost = skill.cost;

			player.sub_MP(cost.mpCost);
			player.sub_HP(cost.hpCost);

			int dmg = skillFormula(player, skillID::GreaterFire) - enemyMob->get_magicResist();

			if (dmg <= 0)
				dmg = 1;

			enemyMob->sub_HP(dmg);

			string log = player.get_name() + " uses " + get_skillColor(skill) + skill.name + clearColor() + " and deals " + to_string(dmg) + typeColor("Magical") + " magical damage" + clearColor() + "!";
			logs.push_back(log);

			checkOnHit(player, enemyMob, logs, skillID::GreaterFire, dmg);
		}
		break;

		case skillID::Transfusion:
		{
			baseSkill skill = skillDataBase[skillID::Transfusion];
			skillCost cost = skill.cost;

			player.sub_MP(cost.mpCost);
			player.sub_HP(cost.hpCost);

			int dmg = skillFormula(player, skillID::Transfusion) - enemyMob->get_magicResist();

			if (dmg <= 0)
				dmg = 1;

			enemyMob->sub_HP(dmg);
			player.add_HP(dmg);

			if (player.get_HP() > player.get_maxHP())
				player.set_HP(player.get_maxHP());

			string log = player.get_name() + " uses " + get_skillColor(skill) + skill.name + clearColor() + " and steals " + to_string(dmg) + typeColor("Health") + " health" + clearColor() + " from " + enemyMob->get_name() + "!";
			logs.push_back(log);
		}
		break;


		//NON PHYSICAL/MAGICAL ------------------------------------------------------------------------

		case skillID::Meditate:
		{
			baseSkill skill = skillDataBase[skillID::Meditate];

			player.add_MP(skillFormula(player, skillID::Meditate));

			if (player.get_MP() > player.get_maxMP())
				player.set_MP(player.get_maxMP());

			string log = player.get_name() + " uses " + get_skillColor(skill) + skill.name + clearColor() + " and restores " + typeColor("Mana") + to_string(skillFormula(player, skillID::Meditate))  + " mana" + clearColor() + "!";
			logs.push_back(log);
		}
		break;

		case skillID::FirstAid:
		{
			baseSkill skill = skillDataBase[skillID::FirstAid];
			skillCost cost = skill.cost;

			player.sub_MP(cost.mpCost);
			player.sub_HP(cost.hpCost);

			if (player.get_HP() <= player.get_maxHP() * 0.3)
			{
				player.add_HP(skillFormula(player, skillID::FirstAid));

				if (player.get_HP() > player.get_maxHP())
					player.set_HP(player.get_maxHP());

				string log = player.get_name() + " uses " + get_skillColor(skill) + skill.name + clearColor() + " and restores " + typeColor("Health") + to_string(skillFormula(player, skillID::FirstAid)) + " health" + clearColor() + "!";
				logs.push_back(log);
			}
			else
			{
				string log = player.get_name() + " uses " + get_skillColor(skill) + skill.name + clearColor() + " but fails to heal their wounds!";
				logs.push_back(log);
			}
		}
		break;









		// BUFFING SKILLS ------------------------------------------------------------------------

		case skillID::Guarded:
		{
			baseSkill skill = skillDataBase[skillID::Guarded];
			skillCost cost = skill.cost;
			skillBuff buffs = *skill.b;

			applyBuffSkill(player, buffs);

			player.sub_MP(cost.mpCost);
			player.sub_HP(cost.hpCost);

			string log = player.get_name() + " uses " + get_skillColor(skill) + skill.name + clearColor() + " and increases defense by 2!";
			logs.push_back(log);
		}
		break;

		case skillID::SpiritShield:
		{
			baseSkill skill = skillDataBase[skillID::SpiritShield];
			skillCost cost = skill.cost;
			skillBuff buffs = *skill.b;

			applyBuffSkill(player, buffs);

			player.sub_MP(cost.mpCost);
			player.sub_HP(cost.hpCost);

			string log = player.get_name() + " uses " + get_skillColor(skill) + skill.name + clearColor() + " and increases magic resist by 2!";
			logs.push_back(log);
		}
		break;

		case skillID::AmbitiousBlade:
		{
			baseSkill skill = skillDataBase[skillID::AmbitiousBlade];
			skillCost cost = skill.cost;
			skillBuff buffs = *skill.b;

			applyBuffSkill(player, buffs);

			player.sub_MP(cost.mpCost);
			player.sub_HP(cost.hpCost);

			string log = player.get_name() + " uses " + get_skillColor(skill) + skill.name + clearColor() + " and increases " + typeColor("Critical") + "critical chance " + clearColor() + "!";
			logs.push_back(log);
		}
		break;

		case skillID::Opportunity:
		{
			baseSkill skill = skillDataBase[skillID::Opportunity];
			skillCost cost = skill.cost;

			player.sub_MP(cost.mpCost);
			player.sub_HP(cost.hpCost);

			string log;

			if (enemyMob->stunned == true)
			{
				clearPlayerDebuffs(player);
				log = player.get_name() + " uses " + get_skillColor(skill) + skill.name + clearColor() + " and cleanses themselves of all negative ailements!";
			}
			else
				log = player.get_name() + " uses " + get_skillColor(skill) + skill.name + clearColor() + " but it fails!";
				
			logs.push_back(log);
		}
		break;

		case skillID::CurePoison:
		{
			baseSkill skill = skillDataBase[skillID::CurePoison];
			skillCost cost = skill.cost;

			player.sub_MP(cost.mpCost);
			player.sub_HP(cost.hpCost);

			bool foundPoison{ false };

			for (int i{ 0 }; i < player.pdebuffs.size(); i++)
			{
				debuff d = player.pdebuffs[i];

				if (d.poison > 0)
				{
					foundPoison = true;
					player.pdebuffsTimer[i] = 0;
				}
			}

			string log;

			if (foundPoison == true)
			{
				log = player.get_name() + " uses " + get_skillColor(skill) + skill.name + clearColor() + " and is no longer " + typeColor("Poison") + "poisoned" + clearColor() + "!";
			}
			else
				log = player.get_name() + " uses " + get_skillColor(skill) + skill.name + clearColor() + " but it fails!";

			logs.push_back(log);
		}
		break;

		case skillID::InnerFocus:
		{
			baseSkill skill = skillDataBase[skillID::InnerFocus];
			skillCost cost = skill.cost;
			skillBuff buffs = *skill.b;

			buff b = buffs.buffs;
			int turns = buffs.turns;

			applyBuffSkill(player, buffs);

			player.sub_MP(cost.mpCost);
			player.sub_HP(cost.hpCost);
			
			string log = player.get_name() + " uses " + get_skillColor(skill) + skill.name + clearColor() + " and applies " + typeColor("Health") + "health regen" + clearColor() + "!";
			logs.push_back(log);
		}
		break;

		case skillID::EverlastingWill:
		{
			baseSkill skill = skillDataBase[skillID::EverlastingWill];
			skillCost cost = skill.cost;
			skillBuff buffs = *skill.b;

			buff b = buffs.buffs;
			int turns = buffs.turns;

			applyBuffSkill(player, buffs);

			player.sub_MP(cost.mpCost);
			player.sub_HP(cost.hpCost);

			string log = player.get_name() + " uses " + get_skillColor(skill) + skill.name + clearColor() + " and gains " + to_string(b.mpRegen) + typeColor("Mana") + " mana regen " + clearColor()
				       + "and " + to_string(b.spellPower) + typeColor("Magical") + " magical power" + clearColor() + "!";
			logs.push_back(log);
		}
		break;

		case skillID::Vampirism:
		{
			baseSkill skill = skillDataBase[skillID::Vampirism];
			skillCost cost = skill.cost;
			skillBuff buffs = *skill.b;

			applyBuffSkill(player, buffs);

			player.sub_MP(cost.mpCost);
			player.sub_HP(cost.hpCost);

			string log = player.get_name() + " uses " + get_skillColor(skill) + skill.name + clearColor() + " and gains 20% spell vamp!";
			logs.push_back(log);
		}
		break;

		case skillID::Invisibility:
		{
			baseSkill skill = skillDataBase[skillID::Invisibility];
			skillCost cost = skill.cost;
			skillBuff buffs = *skill.b;

			buff b = buffs.buffs;
			int turns = buffs.turns;

			applyBuffSkill(player, buffs);

			player.sub_MP(cost.mpCost);
			player.sub_HP(cost.hpCost);

			string log = player.get_name() + " uses " + get_skillColor(skill) + skill.name + clearColor() + " and gains 15% dodge chance!";
			logs.push_back(log);
		}
		break;

		case skillID::Resilience:
		{
			baseSkill skill = skillDataBase[skillID::Resilience];
			skillCost cost = skill.cost;
			skillBuff buffs = *skill.b;

			buff b = buffs.buffs;
			int turns = buffs.turns;

			applyBuffSkill(player, buffs);

			player.sub_MP(cost.mpCost);
			player.sub_HP(cost.hpCost);

			string log = player.get_name() + " uses " + get_skillColor(skill) + skill.name + clearColor() + " and gains 3 defense and magic resist!";
			logs.push_back(log);
		}
		break;

		case skillID::EmpoweredMind:
		{
			baseSkill skill = skillDataBase[skillID::EmpoweredMind];
			skillCost cost = skill.cost;
			skillBuff buffs = *skill.b;

			buff b = buffs.buffs;
			int turns = buffs.turns;

			applyBuffSkill(player, buffs);

			player.sub_MP(cost.mpCost);
			player.sub_HP(cost.hpCost);

			string log = player.get_name() + " uses " + get_skillColor(skill) + skill.name + clearColor() + " and gains 5 " + typeColor("Magical") + "magical power" + clearColor() + "!";
			logs.push_back(log);
		}
		break;

		case skillID::Berserk:
		{
			baseSkill skill = skillDataBase[skillID::Berserk];
			skillCost cost = skill.cost;
			skillBuff buffs = *skill.b;

			buff b = buffs.buffs;
			int turns = buffs.turns;

			applyBuffSkill(player, buffs);

			player.sub_MP(cost.mpCost);
			player.sub_HP(cost.hpCost);

			string log = player.get_name() + " uses " + get_skillColor(skill) + skill.name + clearColor() + " and gains 5 " + typeColor("Physical") + "physical power" + clearColor() + "!";
			logs.push_back(log);
		}
		break;

	}
}