#pragma once
#include <iostream>

#include "Player.h"
#include "skill_ID.h"

inline int skillFormula(Player& player, skillID skill) //Returns result of the specified skill's formula depending on current player attributes
{
	switch (skill)
	{
		//Physical Skills

		case skillID::Attack: 
			return player.get_atk();
			
		case skillID::HeavyStrike: 
			return 6 + (player.get_atk() * 1.5);
			
		case skillID::PreciseStrike: 
			return 3 + player.get_atk();

		case skillID::UltimateStrike:
			return 10 + (player.get_atk() * 2);

		case skillID::MomentumStrike:
			return 8 + player.get_atk() * 1.2;

		case skillID::DizzyingBlow:
			return 3 + (player.get_atk() * 1.2);

			


		//Magical Skills

		case skillID::Heal: 
			return 10 + player.get_spellPower(); 
		case skillID::Fire: 
			return 6 + player.get_spellPower();
		case skillID::BlindingLights: 
			return 3 + (player.get_spellPower() * 0.5);
		case skillID::Renew: 
			return 15 + (player.get_spellPower() * 1.2);

		case skillID::GreaterHeal:
			return 25 + player.get_spellPower() * 1.5;
		case skillID::GreaterFire:
			return 16 + player.get_spellPower() * 1.5;

		case skillID::Transfusion:
			return 15 + player.get_spellPower() * 1.2;
	
		//Non Physical || Magical

		case skillID::FirstAid: 
		{
			double temp = player.get_maxHP() * 0.05;

			return 5 + temp;
		}

		case skillID::Meditate: 
		{
			double temp = player.get_maxMP() * 0.1;

			return 1 + temp;
		}

	}
}