#include <conio.h>
#include <iostream>
#include <vector>

#include "Utility.h"
#include "menu_shop.h"
#include "menu_Inventory.h"
using namespace std;


int manage_shop(Player& player)
{
	int rememberOption{ 0 };

	while(true)
	{
		int option = promptOptions(rememberOption);

		if (option == -1)
			return -1;
		else if (option == 0)
			buyItem(player);
		else if (option == 1)
			sellItem(player);
	}
}

int promptOptions(int& remember)
{
	system("cls");
	cout << "========================================================================================================================\n						          Shop\n"
		"========================================================================================================================\n\n\n\n\n\n\n\n\n\n\n";

	int cursorIndex{ remember };

	while (true)
	{
		for (int i{ 0 }; i < 2; i++)
		{
			cout << "\n                                                     ";

			if (cursorIndex == i)
				cout << "> ";
			else
				cout << "  ";

			if (i == 0)
				cout << "Buy Items";
			else if (i == 1)
				cout << "Sell Items";
		}

		int keyPressed = _getch();

		while (keyPressed != 13 && keyPressed != 119 && keyPressed != 87 && keyPressed != 115 && keyPressed != 83 && keyPressed != 8)
			keyPressed = _getch();

		if (keyPressed == 119 || keyPressed == 87) //W
		{
			cursorIndex--;

			if (cursorIndex < 0)
				cursorIndex = 0;

			utility_eraseLines(2);
			continue;
		}

		if (keyPressed == 115 || keyPressed == 83) //S
		{
			cursorIndex++;

			if (cursorIndex > 1)
				cursorIndex = 1;

			utility_eraseLines(2);
			continue;
		}

		if (keyPressed == 8) //S
		{
			return -1;
		}

		if (keyPressed == 13) //S
		{
			remember = cursorIndex;
			return cursorIndex;
		}
	}
}

void rerollShop(Player& player) //Note: Chance for item to appear in shop = Common 50%/ Uncommon 20%/ Rare 10% / Epic 4% / Legendary 1%
{
	mapID highest = player.mapsUnlocked[player.get_mapsUnlockedSize() - 1]; //get highest unlocked map from player
	player.fightCount = 0; //Reset player's fightCount to 0

	//Empty shop inventory
	player.shop.clear();
	player.shopQTY.clear();
	player.shop2.clear();
	player.shopQTY2.clear();
	player.shop3.clear();
	player.shopQTY3.clear();

	vector<itemID> meadows
	{
		//Uncommons
		itemID::HPPhial, itemID::MPPhial, itemID::RingWillpower, itemID::RingStrength,  itemID::RingHealth, itemID::RingMana,
		

		//Commons
		itemID::RustyDagger, itemID::RustySword, itemID::LeatherArms, itemID::LeatherChest, itemID::LeatherFeet, itemID::LeatherHelmet, itemID::LeatherLegs, itemID::LeatherShield,
		itemID::LeatherShoulder, itemID::SB_FirstAid, itemID::SB_Meditate, itemID::Bread, itemID::Cheese, itemID::Water, itemID::LeatherCape, itemID::BoiledEgg,
		itemID::SB_PreciseStrike, itemID::WoodenStaff, itemID::ClothHelmet, itemID::ClothArms, itemID::ClothChest, itemID::ClothFeet, itemID::ClothLegs, itemID::ClothShoulder,
	};

	vector<itemID> darkForest
	{ 
		//Rare
		itemID::AdventurerCape, itemID::RingWarden, itemID::RingRessource, itemID::SB_Renew,

		//Uncommons
		itemID::StrPhial, itemID::SB_Guarded, itemID::SB_SpiritShield, itemID::SB_Fire, itemID::SB_Heal, itemID::NecklaceVersatility, itemID::ReinforcedLeatherCape, 
		itemID::RingDefense, itemID::RingMagicResistance, itemID::RingAccuracy, itemID::SpellPowerPhial, itemID::ThornsPhial, itemID::HPRegenPhial, itemID::MPRegenPhial,
		itemID::SB_DizzyingBlow, itemID::SB_BlindingLights,


		//Commons
		itemID::IronHelmet, itemID::IronChest, itemID::IronShoulder, itemID::IronArms, itemID::IronLegs, itemID::IronFeet, itemID::IronDagger, itemID::IronSword, itemID::IronGreatSword,
		itemID::SilkArms, itemID::SilkChest, itemID::SilkFeet, itemID::SilkHelmet, itemID::SilkLegs, itemID::SilkShoulder, itemID::IronRod, itemID::IronShield,
	};

	vector<itemID> undeadCatacombs
	{
		//Epic
		itemID::VladimirScepter, itemID::SB_Vampirism,

		//Rare
		itemID::ShadowCloak, itemID::ScholarRing, itemID::JuggernautRing, itemID::RogueRing, itemID::SB_Opportunity, itemID::StrPot, itemID::SB_HeavyStrike, itemID::SpellPowerPot,
		itemID::ThornsPot, itemID::HPRegenPot, itemID::MPRegenPot, itemID::ThornsRing, itemID::NecklaceRuby, itemID::NecklaceSapphire, itemID::SB_InnerFocus, itemID::SB_Invisibility,
		itemID::SB_EmpoweredMind, itemID::SB_Berserk,

		//Uncommons
		itemID::HPPot, itemID::MPPot, itemID::JuggernautHelmet,itemID::JuggernautShoulder, itemID::JuggernautChest, itemID::JuggernautArms, itemID::JuggernautLegs, 
		itemID::JuggernautFeet, itemID::JuggernautGreatsword, itemID::RogueHelmet, itemID::RogueShoulder, itemID::RogueChest, itemID::RogueArms, itemID::RogueLegs, 
		itemID::RogueFeet, itemID::ScholarHelmet, itemID::ScholarShoulder, itemID::ScholarChest, itemID::ScholarArms, itemID::ScholarLegs, itemID::ScholarFeet, 
		itemID::SB_AmbitiousBlade, itemID::SB_Perseverence, itemID::RogueDagger, itemID::ScholarRod, itemID::RingMight, itemID::RingProtection, itemID::RingFortitude, itemID::RingEnergy,
		itemID::RingSorcery, itemID::RingImmunity,

		//Commons
		itemID::CookedMeat, itemID::HotBroth, itemID::Apple, itemID::Milk, itemID::Pear,
	};

	vector<itemID> sunkenPassage
	{
		//Epic
		itemID::SB_Transfusion, itemID::SB_MomentumStrike,

		//Rare
		itemID::SB_CurePoison, itemID::ThornrootHelmet, itemID::ThornrootShoulder, itemID::ThornrootChest, itemID::ThornrootArms, itemID::ThornrootLegs, itemID::ThornrootFeet,
		itemID::ThornrootShield, itemID::NecklaceThorns, itemID::SB_Resilience,

		//Uncommon
		itemID::SteelHelmet, itemID::SteelShoulder, itemID::SteelChest, itemID::SteelArms, itemID::SteelLegs, itemID::SteelFeet, itemID::SteelDagger, itemID::SteelSword,
		itemID::SteelGreatSword, itemID::LinenHelmet, itemID::LinenShoulder, itemID::LinenChest, itemID::LinenArms, itemID::LinenLegs, itemID::LinenFeet, itemID::SteelRod,
		itemID::Mango, itemID::Kiwi, itemID::ScaleHelmet, itemID::ScaleShoulder, itemID::ScaleChest, itemID::ScaleArms, itemID::ScaleLegs, itemID::ScaleFeet,

	};

	vector<itemID> deadShore
	{
		//Legendary
		itemID::SB_UltimateStrike, itemID::UnbrokenRing, itemID::ElderRing, itemID::UnseenRing, itemID::SpellWeaverCape, itemID::OverlordCape, itemID::UnseenCape,
		itemID::NecklaceUnbroken, itemID::NecklaceElder, itemID::SB_EverlastingWill,

		//Epic
		itemID::StrTonic, itemID::SpellPowerTonic, itemID::ThornsTonic, itemID::HPRegenTonic, itemID::MPRegenTonic, itemID::UnbrokenHelmet, itemID::UnbrokenShoulder,
		itemID::UnbrokenChest, itemID::UnbrokenArms, itemID::UnbrokenLegs, itemID::UnbrokenFeet, itemID::UnbrokenGreatSword, itemID::UnbrokenShield, itemID::UnbrokenSword,
		itemID::ElderHelmet, itemID::ElderShoulder, itemID::ElderChest, itemID::ElderArms, itemID::ElderLegs, itemID::ElderFeet, itemID::ElderRod, itemID::UnseenHelmet,
		itemID::UnseenShoulder, itemID::UnseenChest, itemID::UnseenArms, itemID::UnseenLegs, itemID::UnseenFeet,

		//Rare
		itemID::HPTonic, itemID::MPTonic, itemID::SB_GreaterFire, itemID::SB_GreaterHeal,


		//Uncommon
		itemID::HealthyStew, itemID::BakedPotato, itemID::Juice, itemID::DragonFruit, itemID::Pomegranate,


	};

	if (highest == mapID::Meadows)
	{
		for (itemID check : meadows)
		{
			baseItem item = itemDatabase[check];

			if (item.type == itemType::SkillBook)
			{
				bool alreadyHas = checkSkillBook(player, item);

				if (alreadyHas == true) //if player already has skill from that skillbook, dont push it to shop.
					continue;
			}

			pushItem(check, player);
		}

		return;
	}
	else if (highest == mapID::DarkForest)
	{
		for (itemID check : meadows)
		{
			baseItem item = itemDatabase[check];

			if (item.type == itemType::SkillBook)
			{
				bool alreadyHas = checkSkillBook(player, item);

				if (alreadyHas == true) //if player already has skill from that skillbook, dont push it to shop.
					continue;
			}

			pushItem(check, player);
		}

		for (itemID check : darkForest)
		{
			baseItem item = itemDatabase[check];

			if (item.type == itemType::SkillBook)
			{
				bool alreadyHas = checkSkillBook(player, item);

				if (alreadyHas == true) //if player already has skill from that skillbook, dont push it to shop.
					continue;
			}

			pushItem(check, player);
		}

		return;
	}
	else if (highest == mapID::UndeadCatacombs)
	{
		for (itemID check : meadows)
		{
			baseItem item = itemDatabase[check];

			if (item.type == itemType::SkillBook)
			{
				bool alreadyHas = checkSkillBook(player, item);

				if (alreadyHas == true) //if player already has skill from that skillbook, dont push it to shop.
					continue;
			}

			pushItem(check, player);
		}

		for (itemID check : darkForest)
		{
			baseItem item = itemDatabase[check];

			if (item.type == itemType::SkillBook)
			{
				bool alreadyHas = checkSkillBook(player, item);

				if (alreadyHas == true) //if player already has skill from that skillbook, dont push it to shop.
					continue;
			}

			pushItem(check, player);
		}

		for (itemID check : undeadCatacombs)
		{
			baseItem item = itemDatabase[check];

			if (item.type == itemType::SkillBook)
			{
				bool alreadyHas = checkSkillBook(player, item);

				if (alreadyHas == true) //if player already has skill from that skillbook, dont push it to shop.
					continue;
			}

			pushItem(check, player);
		}

		return;
	}
	else if (highest == mapID::SunkenPassage)
	{
		for (itemID check : meadows)
		{
			baseItem item = itemDatabase[check];

			if (item.type == itemType::SkillBook)
			{
				bool alreadyHas = checkSkillBook(player, item);

				if (alreadyHas == true) //if player already has skill from that skillbook, dont push it to shop.
					continue;
			}

			pushItem(check, player);
		}

		for (itemID check : darkForest)
		{
			baseItem item = itemDatabase[check];

			if (item.type == itemType::SkillBook)
			{
				bool alreadyHas = checkSkillBook(player, item);

				if (alreadyHas == true) //if player already has skill from that skillbook, dont push it to shop.
					continue;
			}

			pushItem(check, player);
		}

		for (itemID check : undeadCatacombs)
		{
			baseItem item = itemDatabase[check];

			if (item.type == itemType::SkillBook)
			{
				bool alreadyHas = checkSkillBook(player, item);

				if (alreadyHas == true) //if player already has skill from that skillbook, dont push it to shop.
					continue;
			}

			pushItem(check, player);
		}

		for (itemID check : sunkenPassage)
		{
			baseItem item = itemDatabase[check];

			if (item.type == itemType::SkillBook)
			{
				bool alreadyHas = checkSkillBook(player, item);

				if (alreadyHas == true) //if player already has skill from that skillbook, dont push it to shop.
					continue;
			}

			pushItem(check, player);
		}

		return;
	}
	else if (highest == mapID::DeadShore)
	{
		for (itemID check : meadows)
		{
			baseItem item = itemDatabase[check];

			if (item.type == itemType::SkillBook)
			{
				bool alreadyHas = checkSkillBook(player, item);

				if (alreadyHas == true) //if player already has skill from that skillbook, dont push it to shop.
					continue;
			}

			pushItem(check, player);
		}

		for (itemID check : darkForest)
		{
			baseItem item = itemDatabase[check];

			if (item.type == itemType::SkillBook)
			{
				bool alreadyHas = checkSkillBook(player, item);

				if (alreadyHas == true) //if player already has skill from that skillbook, dont push it to shop.
					continue;
			}

			pushItem(check, player);
		}

		for (itemID check : undeadCatacombs)
		{
			baseItem item = itemDatabase[check];

			if (item.type == itemType::SkillBook)
			{
				bool alreadyHas = checkSkillBook(player, item);

				if (alreadyHas == true) //if player already has skill from that skillbook, dont push it to shop.
					continue;
			}

			pushItem(check, player);
		}

		for (itemID check : sunkenPassage)
		{
			baseItem item = itemDatabase[check];

			if (item.type == itemType::SkillBook)
			{
				bool alreadyHas = checkSkillBook(player, item);

				if (alreadyHas == true) //if player already has skill from that skillbook, dont push it to shop.
					continue;
			}

			pushItem(check, player);
		}

		for (itemID check : deadShore)
		{
			baseItem item = itemDatabase[check];

			if (item.type == itemType::SkillBook)
			{
				bool alreadyHas = checkSkillBook(player, item);

				if (alreadyHas == true) //if player already has skill from that skillbook, dont push it to shop.
					continue;
			}

			pushItem(check, player);
		}


		return;
	}
	
}

void pushItem(itemID& ID, Player& player) 
{
	baseItem item = itemDatabase[ID];

	if (item.rarity == itemRarity::Common)
	{
		int roll = rand() % 100 + 1;

		if (item.type == itemType::Food) //food guaranteed in shop, helps for early game
		{

			if (player.shop.size() < 20)
			{ 
				player.shop.push_back(ID);
				player.shopQTY.push_back(pushItemQTY(item));
			}
			else if (player.shop.size() >= 20 && player.shop2.size() < 20)
			{
				player.shop2.push_back(ID);
				player.shopQTY2.push_back(pushItemQTY(item));
			}
			else if (player.shop.size() >= 20 && player.shop2.size() >= 20 && player.shop3.size() < 20)
			{
				player.shop3.push_back(ID);
				player.shopQTY3.push_back(pushItemQTY(item));
			}
			
			return;
		}

		if (roll <= 50)
		{
			if (player.shop.size() < 20)
			{
				player.shop.push_back(ID);
				player.shopQTY.push_back(pushItemQTY(item));
			}
			else if (player.shop.size() >= 20 && player.shop2.size() < 20)
			{
				player.shop2.push_back(ID);
				player.shopQTY2.push_back(pushItemQTY(item));
			}
			else if (player.shop.size() >= 20 && player.shop2.size() >= 20 && player.shop3.size() < 20)
			{
				player.shop3.push_back(ID);
				player.shopQTY3.push_back(pushItemQTY(item));
			}

			return;
		}
		else
			return;
	}
	else if (item.rarity == itemRarity::Uncommon)
	{
		int roll = rand() % 100 + 1;

		if (roll <= 20)
		{
			if (player.shop.size() < 20)
			{
				player.shop.push_back(ID);
				player.shopQTY.push_back(pushItemQTY(item));
			}
			else if (player.shop.size() >= 20 && player.shop2.size() < 20)
			{
				player.shop2.push_back(ID);
				player.shopQTY2.push_back(pushItemQTY(item));
			}
			else if (player.shop.size() >= 20 && player.shop2.size() >= 20 && player.shop3.size() < 20)
			{
				player.shop3.push_back(ID);
				player.shopQTY3.push_back(pushItemQTY(item));
			}

			return;
		}
		else
			return;
	}
	else if (item.rarity == itemRarity::Rare)
	{
		int roll = rand() % 100 + 1;

		if (roll <= 10)
		{
			if (player.shop.size() < 20)
			{
				player.shop.push_back(ID);
				player.shopQTY.push_back(pushItemQTY(item));
			}
			else if (player.shop.size() >= 20 && player.shop2.size() < 20)
			{
				player.shop2.push_back(ID);
				player.shopQTY2.push_back(pushItemQTY(item));
			}
			else if (player.shop.size() >= 20 && player.shop2.size() >= 20 && player.shop3.size() < 20)
			{
				player.shop3.push_back(ID);
				player.shopQTY3.push_back(pushItemQTY(item));
			}

			return;
		}
		else
			return;
	}
	else if (item.rarity == itemRarity::Epic)
	{
		int roll = rand() % 100 + 1;

		if (roll <= 5)
		{
			if (player.shop.size() < 20)
			{
				player.shop.push_back(ID);
				player.shopQTY.push_back(pushItemQTY(item));
			}
			else if (player.shop.size() >= 20 && player.shop2.size() < 20)
			{
				player.shop2.push_back(ID);
				player.shopQTY2.push_back(pushItemQTY(item));
			}
			else if (player.shop.size() >= 20 && player.shop2.size() >= 20 && player.shop3.size() < 20)
			{
				player.shop3.push_back(ID);
				player.shopQTY3.push_back(pushItemQTY(item));
			}

			return;
		}
		else
			return;
	}
	else if (item.rarity == itemRarity::Legendary)
	{
		int roll = rand() % 100 + 1;

		if (roll <= 1)
		{
			if (player.shop.size() < 20)
			{
				player.shop.push_back(ID);
				player.shopQTY.push_back(pushItemQTY(item));
			}
			else if (player.shop.size() >= 20 && player.shop2.size() < 20)
			{
				player.shop2.push_back(ID);
				player.shopQTY2.push_back(pushItemQTY(item));
			}
			else if (player.shop.size() >= 20 && player.shop2.size() >= 20 && player.shop3.size() < 20)
			{
				player.shop3.push_back(ID);
				player.shopQTY3.push_back(pushItemQTY(item));
			}

			return;
		}
		else
			return;
	}
}

int pushItemQTY(baseItem& item)
{
	if (item.type != itemType::Potion && item.type != itemType::Food)
		return 1;
	else if (item.type == itemType::Potion)
	{
		int roll = rand() % 3 + 1;
		return roll;
	}
	else if (item.type == itemType::Food)
	{
		int roll{ 0 };

		if (item.rarity == itemRarity::Common)
			 roll = rand() % 10 + 5;
		else if (item.rarity == itemRarity::Uncommon)
			roll = rand() % 5 + 2;

		return roll;
	}
}


void buyItem(Player& player)
{
	//Checks if shop is empty (no items left for sale)
	if (player.shop.size() == 0)
	{
		system("cls");
		cout << "========================================================================================================================\n						        Buy items\n"
			"========================================================================================================================\n\n\n\n\n\n\n";

		cout << "                                         No items for sale, come back later!\n\n"
			<< "                                                   > I Understand";

		int keyPressed = _getch();

		while (keyPressed != 8 && keyPressed != 13)
			keyPressed = _getch();

		return;
	}

	int cursorIndex{ 0 };
	int currentPage{ 1 }; 
	int maxPage{ 1 };
	sortShop(player);

	//Find how many pages does shop have (1 to 3)
	if (player.shop2.size() > 0)
		maxPage = 2;
	if (player.shop3.size() > 0)
		maxPage = 3;

	while (true)
	{
		system("cls");
		cout << "========================================================================================================================\n						        Buy items\n"
			"========================================================================================================================\n\n";

		cout << "            Page " << currentPage << "/" << maxPage;
		
		cout << "\n            Coins: ";

		if (player.get_gold() > 0)
			cout << typeColor("Gold") << player.get_gold() << "g ";
		if (player.get_silver() > 0)
			cout << typeColor("Silver") << player.get_silver() << "s ";
	
		cout << typeColor("Copper") << player.get_copper() << "c " << clearColor();

		

		cout << "\n\n                          -------- Item Name --------           --- QTY ---        ---- Price ---- \n\n";

		if (currentPage == 1)
		{
			for (int i{ 0 }; i < player.shop.size(); i++)
			{
				itemID ID = player.shop[i];
				baseItem item = itemDatabase[ID];
				int qty = player.shopQTY[i];

				cout << "                             ";

				if (cursorIndex == i)
					cout << "> ";
				else
					cout << "  ";

				cout << left << get_itemColor(item) << setw(30) << item.name << clearColor() << "        " << setw(2) << qty << "               "; displayBuyValue(item); cout << endl;
			}
		}
		else if (currentPage == 2)
		{
			for (int i{ 0 }; i < player.shop2.size(); i++)
			{
				itemID ID = player.shop2[i];
				baseItem item = itemDatabase[ID];
				int qty = player.shopQTY2[i];

				cout << "                             ";

				if (cursorIndex == i)
					cout << "> ";
				else
					cout << "  ";

				cout << left << get_itemColor(item) << setw(30) << item.name << clearColor() << "        " << setw(2) << qty << "               "; displayBuyValue(item); cout << endl;
			}
		}
		else if (currentPage == 3)
		{
			for (int i{ 0 }; i < player.shop3.size(); i++)
			{
				itemID ID = player.shop3[i];
				baseItem item = itemDatabase[ID];
				int qty = player.shopQTY3[i];

				cout << "                             ";

				if (cursorIndex == i)
					cout << "> ";
				else
					cout << "  ";

				cout << left << get_itemColor(item) << setw(30) << item.name << clearColor() << "        " << setw(2) << qty << "               "; displayBuyValue(item); cout << endl;
			}
		}
		//WHERE LEFT OFF: ADD GETCH FOR A AND D KEYS AND INCREMENT OR DECREMENT CURRENT PAGE ACCORDINGLY

		int keyPressed = _getch();

		while (keyPressed != 13 && keyPressed != 8 && keyPressed != 13 && keyPressed != 119 && keyPressed != 87 && keyPressed != 115 && keyPressed != 83 && keyPressed != 97 && keyPressed != 65 && keyPressed != 68 && keyPressed != 100)
			keyPressed = _getch();

		if (keyPressed == 8)
			return;
		else if (keyPressed == 119 || keyPressed == 87)
		{
			cursorIndex--;

			if (cursorIndex < 0)
				cursorIndex = 0;

			continue;
		}
		else if (keyPressed == 115 || keyPressed == 83)
		{
			cursorIndex++;

			if (currentPage == 1)
			{
				if (cursorIndex > player.shop.size() - 1)
					cursorIndex = player.shop.size() - 1;
			}
			else if (currentPage == 2)
			{
				if (cursorIndex > player.shop2.size() - 1)
					cursorIndex = player.shop2.size() - 1;
			}
			else if (currentPage == 3)
			{
				if (cursorIndex > player.shop3.size() - 1)
					cursorIndex = player.shop3.size() - 1;
			}
			
			continue;
		}
		else if (keyPressed == 97 || keyPressed == 65)
		{
			currentPage--;
			cursorIndex = 0;

			if (currentPage == 0)
				currentPage = 1;

			continue;
		}
		else if (keyPressed == 100 || keyPressed == 68)
		{
			currentPage++;
			cursorIndex = 0;

			if (currentPage > maxPage)
				currentPage = maxPage;

			continue;
		}
		else if (keyPressed == 13)
		{
			itemID ID;
			baseItem item;

			if (currentPage == 1)
			{
				ID = player.shop[cursorIndex];
				item = itemDatabase[ID];
			}
			else if (currentPage == 2)
			{
				ID = player.shop2[cursorIndex];
				item = itemDatabase[ID];
			}
			else if (currentPage == 3)
			{
				ID = player.shop3[cursorIndex];
				item = itemDatabase[ID];
			}
		
		repeat:

			int option = buyItemInfo(player, item, player.shopQTY, cursorIndex);

			if (option == 1) //buy
			{ 
				bool hasSpace{ false }; //does player have space to add the item?
				bool hasMoney{ false }; //does player have enough money to buy the item? (remove money ONLY after confirming player has space)
				int atIndex; //index where to add item in player's inventory

				hasMoney = priceCheck(player, item);

				if (hasMoney == false)
				{
					system("cls");
					cout << "========================================================================================================================\n						        Buy items\n"
						"========================================================================================================================\n\n\n\n";

					cout << "\n\n\n\n                                               You cannot afford this item.";
					cout << "\n\n                                                   > [I Understand]";

					int keyPressed = _getch();

					while (keyPressed != 13 && keyPressed != 8)
						keyPressed = _getch();

					continue;
				}

				//if item is stackable, check if player already has item && check if quantity is below 99
				if (item.stackable == true)
				{
					for (int i{ 0 }; i < player.inventorySize; i++)
					{
						if (player.inventory[i] == ID && player.inventory_QTY[i] < 99)
						{
							atIndex = i;
							hasSpace = true;
						}
					}
				}
				
				if (hasSpace == false) //If we didnt find a stack of the same item that isnt full, check if player has an empty slot
				{
					//Check if player has an empty slot to add item
					for (int i{ 0 }; i < player.inventorySize; i++)
					{
						if (player.inventory[i] == itemID::None)
						{
							atIndex = i;
							hasSpace = true;
						}
					}
				}

				if (hasSpace == true)
				{
					player.inventory[atIndex] = ID;
					player.inventory_QTY[atIndex]++;
					removeCoins(player, item);

					if (currentPage == 1)
					{
						player.shopQTY[cursorIndex]--;

						if (player.shopQTY[cursorIndex] == 0) //if item for sale is depleted
						{
							player.shop.erase(player.shop.begin() + cursorIndex);
							player.shopQTY.erase(player.shopQTY.begin() + cursorIndex);

							if (maxPage == 3)
							{
								deleteEmptySlots(player, currentPage, maxPage);

								if (player.shop3.size() == 0)
								{
									player.shop3.clear();
									player.shopQTY3.clear();
									maxPage = 2;
									currentPage = 2;
								}
							}
							else if (maxPage == 2)
							{
								deleteEmptySlots(player, currentPage, maxPage);

								if (player.shop2.size() == 0)
								{
									player.shop2.clear();
									player.shopQTY2.clear();
									maxPage = 1;
									currentPage = 1;
								}
							}
						
							sortShop(player);
							deleteEmptySlots(player, currentPage, maxPage);
							cursorIndex = 0;

							//if shop is completely empty, return player to previous menu
							if (player.shop.size() == 0)
								return;
						
							continue;
						}
						else
							goto repeat; 
					}
					else if (currentPage == 2)
					{
						player.shopQTY2[cursorIndex]--;

						if (player.shopQTY2[cursorIndex] == 0) 
						{
							player.shop2.erase(player.shop2.begin() + cursorIndex);
							player.shopQTY2.erase(player.shopQTY2.begin() + cursorIndex);

							if (maxPage == 3)
							{
								deleteEmptySlots(player, currentPage, maxPage);

								if (player.shop3.size() == 0)
								{
									player.shop3.clear();
									player.shopQTY3.clear();
									maxPage = 2;
									currentPage = 2;
								}
								
							}
							else if (maxPage == 2)
							{
								deleteEmptySlots(player, currentPage, maxPage);

								if (player.shop2.size() == 0)
								{
									player.shop2.clear();
									player.shopQTY2.clear();
									maxPage = 1;
									currentPage = 1;
								}
								
							}

							sortShop(player);
							deleteEmptySlots(player, currentPage, maxPage);
							cursorIndex = 0;
						
							continue;
						}
						else
							goto repeat; 
					}
					else if (currentPage == 3)
					{
						player.shopQTY3[cursorIndex]--;

						if (player.shopQTY3[cursorIndex] == 0) 
						{
							player.shop3[cursorIndex] = itemID::None;

							bool empty{ true };

							for (itemID check : player.shop3)
								if (check != itemID::None)
									empty = false;

							if (empty == true)
							{
								player.shop3.clear();
								player.shopQTY3.clear();
								maxPage = 2;
								currentPage = 2;

								sortShop(player);
								continue;
							}
							else
							{
								sortShop(player);

								player.shop3.pop_back();
								player.shopQTY3.pop_back();
							}
							
							cursorIndex = 0;
							continue;
						}
						else
							goto repeat; 
					}
			
				}
				else
				{
					system("cls");
					cout << "========================================================================================================================\n						        Buy items\n"
						"========================================================================================================================\n\n\n\n";

					cout << "\n\n\n\n                                     You do not have enough room to purchase this item.";
					cout << "\n\n                                                    > [I Understand]";

					int keyPressed = _getch();

					while (keyPressed != 13 && keyPressed != 8)
						keyPressed = _getch();

					continue;
				}
			}	
			else if (option == -1) //back
				continue;
			

			//gather option (buy or back). if buy, remove item from shop (or quantity-- if has more than 1 for sale), give item to player, refresh page (loop back to buyItemInfo). If back or item runs out, go back to buyItem
		}
	}
}

bool priceCheck(Player& player, baseItem& item)
{
	int playerMoney = (player.get_gold() * 10000) + (player.get_silver() * 100) + player.get_copper();

	if (playerMoney >= item.priceValue)
		return true;
	else
		return false;
}

void removeCoins(Player& player, baseItem& item)
{
	//Calculate player's total money and apply item price
	int playerMoney = (player.get_gold() * 10000) + (player.get_silver() * 100) + player.get_copper();
	playerMoney -= item.priceValue;

	//Recalculate player coins after purchase
	int gold = playerMoney / 10000;
	playerMoney -= gold * 10000;

	int silver = playerMoney / 100;
	playerMoney -= silver * 100;

	int copper = playerMoney;

	//Give remaining coins back to player
	player.set_gold(gold);
	player.set_silver(silver);
	player.set_copper(copper);
}

void displayBuyValue(baseItem& item)
{
	int value = item.priceValue;

	if (value >= 10000)
	{
		int gold = value / 10000;
		value -= gold * 10000;

		int silver = value / 100;
		value -= silver * 100;

		int copper = value;

		cout << typeColor("Gold") << gold << "g " << typeColor("Silver") << silver << "s " << typeColor("Copper") << copper << "c " << clearColor();
	}
	else if (value >= 100 && value < 10000)
	{
		int silver = value / 100;
		value -= silver * 100;

		int copper = value;

		cout << typeColor("Silver") << silver << "s " << typeColor("Copper") << copper << "c " << clearColor();

	}
	else if (value < 100)
	{
		cout << typeColor("Copper") << value << "c " << clearColor();
	}
}

int buyItemInfo(Player& player, baseItem& item, vector<int>& shopQTY, int& atIndex)
{
	//where i left
	system("cls");
	cout << "========================================================================================================================\n						        Buy items\n"
		"========================================================================================================================\n\n\n";

	cout << "            Coins: ";

	if (player.get_gold() > 0)
		cout << typeColor("Gold") << player.get_gold() << "g ";
	if (player.get_silver() > 0)
		cout << typeColor("Silver") << player.get_silver() << "s ";
	
	cout << typeColor("Copper") << player.get_copper() << "c " << clearColor() << "\n\n";

	if (item.type == itemType::Weapon)
	{
		weapon_stats wpn = get<weapon_stats>(item.stats); //weapon_stats : atk, spellPower, hitChance, critChance, twohanded

		cout << left
			<< "\n\n                                        " << get_itemColor(item) << "[" << item.name << "]" << clearColor()
			<< "\n                                        " << item.description << endl;
		if (wpn.atk != 0)
			cout << "\n                                        " << "Attack Damage: " << itemStatColor(wpn.atk) << wpn.atk << clearColor();
		if (wpn.spellPower != 0)
			cout << "\n                                        " << "Spell Power: " << itemStatColor(wpn.spellPower) << wpn.spellPower << clearColor();
		if (wpn.hitChance != 0)
			cout << "\n                                        " << "Hit Chance: " << itemStatColor(wpn.hitChance) << wpn.hitChance << clearColor();
		if (wpn.critChance != 0)
			cout << "\n                                        " << "Critical Chance: " << itemStatColor(wpn.critChance) << wpn.critChance << clearColor();

		cout << "\n\n                                        " << "Weight: "; if (wpn.twoHanded == true) { cout << "Two-Handed"; }
		else { cout << "One-Handed"; }

		if (item.enchants) //If enchant found on item
		{
			cout << "\n\n                                        " << "Enchantments:";
			enchantments e = *item.enchants;
			print_itemEnchants(e);
		}

		cout << "\n\n                                        > [Buy Item]  "; displayBuyValue(item);
	}
	else if (item.type == itemType::Shield || item.type == itemType::Head || item.type == itemType::Shoulders || item.type == itemType::Chest || item.type == itemType::Arms || item.type == itemType::Legs || item.type == itemType::Feet)
	{
		armor_stats armor = get<armor_stats>(item.stats); //weapon_stats : atk, spellPower, hitChance, critChance, twohanded

		cout << left
			<< "\n\n                                        " << get_itemColor(item) << "[" << item.name << "]" << clearColor()
			<< "\n                                        " << item.description << endl;
		if (armor.def != 0)
			cout << "\n                                        " << "Defense: " << itemStatColor(armor.def) << armor.def << clearColor();
		if (armor.magicResist != 0)
			cout << "\n                                        " << "Magic Resist: " << itemStatColor(armor.magicResist) << armor.magicResist << clearColor();
		if (armor.dodgeChance != 0)
			cout << "\n                                        " << "Dodge Chance: " << itemStatColor(armor.dodgeChance) << armor.dodgeChance << clearColor();;

		cout << "\n\n                                        " << "Weight: " << armor.weight;

		if (item.enchants) //If enchant found on item
		{
			cout << "\n\n                                        " << "Enchantments:";
			enchantments e = *item.enchants;
			print_itemEnchants(e);
		}

		cout << "\n\n                                        > [Buy Item]  "; displayBuyValue(item);
	}
	else if (item.type == itemType::Ring || item.type == itemType::Necklace || item.type == itemType::Cape)
	{
		cout << left
			<< "\n\n                                        " << get_itemColor(item) << "[" << item.name << "]" << clearColor()
			<< "\n                                        " << item.description;

		if (item.enchants) //If enchant found on item
		{
			cout << "\n\n                                        " << "Enchantments:";
			enchantments e = *item.enchants;
			print_itemEnchants(e);
		}

		cout << "\n\n                                        > [Buy Item]  "; displayBuyValue(item);
	}
	else if (item.type == itemType::Potion || item.type == itemType::Food)
	{
		consumable_stats consumable = get<consumable_stats>(item.stats);

		cout << left
			<< "\n\n                                        " << get_itemColor(item) << "[" << item.name << "]" << clearColor()
			<< "\n                                        " << item.description << endl;

		if (consumable.type == "Health")
			cout << "\n                                        " << "Restores " << typeColor("Health") << showpos << consumable.powerValue << noshowpos << " HP" << clearColor();
		else if (consumable.type == "Mana")
			cout << "\n                                        " << "Restores " << typeColor("Health") << showpos << consumable.powerValue << noshowpos << " MP" << clearColor();
		else if (consumable.type == "HealthMana")
			cout << "\n                                        " << "Restores " << typeColor("Health") << showpos << consumable.powerValue << noshowpos << " HP" << clearColor() << " and " << typeColor("Mana") << showpos << consumable.powerValue << noshowpos << " MP" << clearColor();

		cout << "\n\n                                        > [Buy Item]  "; displayBuyValue(item);
		
	}
	else if (item.type == itemType::BuffPotion)
	{
		consumable_buffs buffPot = get<consumable_buffs>(item.stats);
		buff b = buffPot.buffs;

		cout << left
			<< "                                        " << get_itemColor(item) << "[" << item.name << "]" << clearColor()
			<< "\n                                        " << item.description;

		cout << "\n\n                                        Effect lasts for " << buffPot.turns-1 << " turns.";
		cout << "\n\n                                        " << "Buffs: ";

		print_buffs(b);

		cout << "\n\n                                        > [Buy Item]  "; displayBuyValue(item);

	}
	else if (item.type == itemType::SkillBook)
	{
		cout << left
			<< "\n\n                                        " << get_itemColor(item) << "[" << item.name << "]" << clearColor()
			<< "\n                                        " << item.description << endl;

		cout << "\n\n                                        > [Buy Item]  "; displayBuyValue(item);

	}

	int keyPressed = _getch();

	while (keyPressed != 13 && keyPressed != 8)
		keyPressed = _getch();

	if (keyPressed == 13)
		return 1;
	else if (keyPressed == 8)
		return -1;
}

void sortShop(Player& player)
{
	vector<itemID> unsorted;
	vector<int> unsortedQTY;

	vector<itemID> sorted;
	vector<int> sortedQTY;

	itemID ID;
	baseItem item;
	int biggestValue;
	int atIndex;

	while (unsorted.size() != player.shop.size())
	{ 
		biggestValue = 0; //reset

		for (int i{ 0 }; i < player.shop.size(); i++)
		{
			ID = player.shop[i];
			item = itemDatabase[ID];

			if (ID == itemID::None)
				continue;
		
			if (item.priceValue >= biggestValue)
			{
				biggestValue = item.priceValue;
				atIndex = i;
			}
		}

		unsorted.push_back(player.shop[atIndex]);
		player.shop[atIndex] = itemID::None;

		unsortedQTY.push_back(player.shopQTY[atIndex]);
	}

	while (unsorted.size()-20 != player.shop2.size() && player.shop2.size() > 0)
	{
		biggestValue = 0; //reset
		
		for (int i{ 0 }; i < player.shop2.size(); i++)
		{
			ID = player.shop2[i];
			item = itemDatabase[ID];

			if (ID == itemID::None)
				continue;

			if (item.priceValue >= biggestValue)
			{
				biggestValue = item.priceValue;
				atIndex = i;
			}
		}

		unsorted.push_back(player.shop2[atIndex]);
		player.shop2[atIndex] = itemID::None;

		unsortedQTY.push_back(player.shopQTY2[atIndex]);
	}

	while (unsorted.size()-40 != player.shop3.size() && player.shop3.size() > 0)
	{
		biggestValue = 0; //reset

		for (int i{ 0 }; i < player.shop3.size(); i++)
		{
			ID = player.shop3[i];
			item = itemDatabase[ID];

			if (ID == itemID::None)
				continue;

			if (item.priceValue >= biggestValue)
			{
				biggestValue = item.priceValue;
				atIndex = i;
			}
		}

		unsorted.push_back(player.shop3[atIndex]);
		player.shop3[atIndex] = itemID::None;

		unsortedQTY.push_back(player.shopQTY3[atIndex]);
	}


	//Sort the unsorted vector
	while (sorted.size() != unsorted.size())
	{
		biggestValue = 0; //reset

		for (int i{ 0 }; i < unsorted.size(); i++)
		{
			ID = unsorted[i];
			item = itemDatabase[ID];

			if (ID == itemID::None)
				continue;

			if (item.priceValue >= biggestValue)
			{
				biggestValue = item.priceValue;
				atIndex = i;
			}
		}

		sorted.push_back(unsorted[atIndex]);
		unsorted[atIndex] = itemID::None;

		sortedQTY.push_back(unsortedQTY[atIndex]);
	}

	//Clear pages to insert sorted items
	player.shop.clear();
	player.shopQTY.clear();
	player.shop2.clear();
	player.shopQTY2.clear();
	player.shop3.clear();
	player.shopQTY3.clear();

	for (int i{ 0 }; i < sorted.size(); i++)
	{
		if (i < 20)
		{
			player.shop.push_back(sorted[i]);
			player.shopQTY.push_back(sortedQTY[i]);
		}
		else if (i >= 20 && i < 40)
		{
			player.shop2.push_back(sorted[i]);
			player.shopQTY2.push_back(sortedQTY[i]);
		}
		else if (i >= 40)
		{
			player.shop3.push_back(sorted[i]);
			player.shopQTY3.push_back(sortedQTY[i]);
		}
	}
}

void sellItem(Player& player) 
{
	int remember{1};

back:
	sortInventory(player);

	int itemSelected = displayInventory(player, remember); //itemSelected represents index of that item

	if (itemSelected == -1) //if user pressed BS, go back
		return;

sellMore: //used for goto

	system("cls");
	cout << "========================================================================================================================\n						        Sell Items\n"
		"========================================================================================================================\n\n\n\n";

	cout << "            Coins: ";

	if (player.get_gold() > 0)
		cout << typeColor("Gold") << player.get_gold() << "g ";
	if (player.get_silver() > 0)
		cout << typeColor("Silver") << player.get_silver() << "s ";
	
	cout << typeColor("Copper") << player.get_copper() << "c " << clearColor() << "\n\n\n\n\n";

	itemID ID = player.inventory[itemSelected];
	remember = itemSelected+1;

	if (ID == itemID::None)
		goto back;

	int qty = player.inventory_QTY[itemSelected];
	baseItem item = itemDatabase[ID];

	cout << "                                                Sell " << get_itemColor(item) << item.name << clearColor() << " for ";  displayValue(item, false, qty); cout << " ?\n\n";
	cout << "                                                Quantity: " << qty;

	int cursorIndex{ 0 };

	while (true)
	{
		cout << "\n\n                                                ";

		for (int i{ 0 }; i < 2; i++)
		{
			if (cursorIndex == i)
				cout << "> ";
			else
				cout << "  ";

			if (i == 0)
				cout << "[Sell]    ";
			
			if (i == 1)
			{
				cout << "[Sell All] ("; displayValue(item, true, qty); cout << ")";
			}
		}

		int keyPressed = _getch();

		while (keyPressed != 13 && keyPressed != 8 && keyPressed != 100 && keyPressed != 68 && keyPressed != 97 && keyPressed != 65)
			keyPressed = _getch();

		if (keyPressed == 8)
			goto back;
		else if (keyPressed == 100 || keyPressed == 68)
		{
			cursorIndex++;

			if (cursorIndex > 1)
				cursorIndex = 1;

			utility_eraseLines(2);
			continue;
		}
		else if (keyPressed == 97 || keyPressed == 65)
		{
			cursorIndex--;

			if (cursorIndex < 0)
				cursorIndex = 0;

			utility_eraseLines(2);
			continue;
		}
		else if (keyPressed == 13)
		{
			if (cursorIndex == 1) //Sell All
			{
				int value;

				if (item.type == itemType::Loot) //if item is loot, sell 100% value
				{
					value = item.priceValue * qty;
				}
				else //if item is not loot, sell 20% value
				{
					double temp = item.priceValue;

					temp /= 100;

					if (temp < 0.05)
						temp = 0.05;

					temp *= 20;
					value = temp;
					value *= qty;
				}

				//Give money to player and remove item stack from inventory
				giveCoins(player, value); 
				recalculateCoins(player);
				player.inventory[itemSelected] = itemID::None;
				player.inventory_QTY[itemSelected] = 0;
				
				goto back;
			}
			else if (cursorIndex == 0) //Sell single
			{
				int value;

				if (item.type == itemType::Loot) //if item is loot, sell 100% value
				{
					value = item.priceValue * qty;
				}
				else //if item is not loot, sell 20% value
				{
					double temp = item.priceValue;

					temp /= 100;

					if (temp < 0.05)
						temp = 0.05;

					temp *= 20;
					value = temp;
				}

				giveCoins(player, value);
				recalculateCoins(player);
				player.inventory_QTY[itemSelected]--;

				if (player.inventory_QTY[itemSelected] == 0) //if item runs out, go back
				{
					player.inventory[itemSelected] = itemID::None;
					goto back;
				}
				else
					goto sellMore; //repeat process until player backs out
			}
		}
	}
}

int displayInventory(Player& player, int& remember)
{
	system("cls");
	cout << "========================================================================================================================\n						        Sell Items\n"
		"========================================================================================================================\n\n\n\n";

	cout << "            Coins: ";

	if (player.get_gold() > 0)
		cout << typeColor("Gold") << player.get_gold() << "g ";
	if (player.get_silver() > 0)
		cout << typeColor("Silver") << player.get_silver() << "s ";
	
	cout << typeColor("Copper") << player.get_copper() << "c "<< clearColor() << "\n\n";

	cout << "                         Item Name                    QTY |   Item name                    QTY\n                                                          |\n";

	int cursorIndex{ remember };

	while (true)
	{
		for (int i{ 1 }; i < player.inventorySize + 1; i++)
		{
			itemID ID = player.inventory[i - 1];
			int qty = player.inventory_QTY[i - 1];

			baseItem item = itemDatabase[ID];

			cout << "                       ";

			if (i == cursorIndex)
			{
				cout << "> ";
			}
			else
				cout << "  ";

			cout << left << get_itemColor(item) << setw(30) << item.name << clearColor();

			if (qty >= 1 && item.stackable == true)
				cout << setw(2) << qty;
			else
				cout << "  ";

			cout << " | ";
			i++;

			ID = player.inventory[i - 1];
			qty = player.inventory_QTY[i - 1];

			item = itemDatabase[ID];

			if (i == cursorIndex)
			{
				cout << "> ";
			}
			else
				cout << "  ";

			cout << left << get_itemColor(item) << setw(30) << item.name << clearColor();

			if (qty >= 1 && item.stackable == true)
				cout << setw(2) << qty;
			else
				cout << "  ";

			cout << endl;
		}

		int keyPressed = _getch(); //Store key pressed by user (W|S|ENTER)

		while (keyPressed != 119 && keyPressed != 87 && keyPressed != 115 && keyPressed != 83 && keyPressed != 68 && keyPressed != 100 && keyPressed != 65 && keyPressed != 97 && keyPressed != 8 && keyPressed != 13)
		{
			keyPressed = _getch();
		}

		if (keyPressed == 119 || keyPressed == 87) //W
		{
			cursorIndex -= 2;
			utility_eraseLines(player.inventorySize / 2);

			if (cursorIndex < 1)
			{
				cursorIndex += 2;
			}
		}

		if (keyPressed == 115 || keyPressed == 83) //S
		{
			cursorIndex += 2;
			utility_eraseLines(player.inventorySize / 2);

			if (cursorIndex > player.inventorySize)
			{
				cursorIndex -= 2;
			}
		}

		if (keyPressed == 68 || keyPressed == 100) //D
		{
			cursorIndex += 1;
			utility_eraseLines(player.inventorySize / 2);

			if (cursorIndex > player.inventorySize)
			{
				cursorIndex = player.inventorySize;
			}
		}

		if (keyPressed == 65 || keyPressed == 97) //A
		{
			cursorIndex -= 1;
			utility_eraseLines(player.inventorySize / 2);


			if (cursorIndex < 1)
			{
				cursorIndex = 1;
			}
		}

		if (keyPressed == 13) //ENTER
		{
			return cursorIndex-1;
		}

		if (keyPressed == 8) //BS
		{
			return -1;
		}

	}
}

void giveCoins(Player& player, int& value) 
{
	if (value < 100)
	{
		player.add_copper(value);
		return;
	}
	else if (value >= 100 && value < 10000)
	{
		int silver = value / 100;
		value -= silver * 100;

		int copper = value;

		player.add_copper(copper);
		player.add_silver(silver);
		return;
	}
	else if (value >= 10000)
	{
		int gold = value / 10000;
		value -= gold * 10000;

		int silver = value / 100;
		value -= silver * 100;

		int copper = value;

		player.add_copper(copper);
		player.add_silver(silver);
		player.add_gold(gold);
		return;
	}
}

void recalculateCoins(Player& player)
{
	int playerMoney = (player.get_gold() * 10000) + (player.get_silver() * 100) + player.get_copper();

	int gold = playerMoney / 10000;
	playerMoney -= gold * 10000;

	int silver = playerMoney / 100;
	playerMoney -= silver * 100;

	int copper = playerMoney;

	player.set_copper(copper);
	player.set_silver(silver);
	player.set_gold(gold);
}

bool checkSkillBook(Player& player, baseItem& item)
{
	skillID skill = *item.skill;
	
	for (skillID check : player.skills) //checks if player already has skill for that skillbook
	{
		if (check == skill)
			return true;
	}

	return false;
}

void deleteEmptySlots(Player& player, int& currentPage, int& maxPage)
{
	for (int i{ 0 }; i < player.shop.size(); i++)
	{
		if (player.shop[i] == itemID::None)
		{
			player.shop.erase(player.shop.begin() + i);
			player.shopQTY.erase(player.shopQTY.begin() + i);
			i--;
		}
	}
	for (int i{ 0 }; i < player.shop2.size(); i++)
	{
		if (player.shop2[i] == itemID::None)
		{
			player.shop2.erase(player.shop2.begin() + i);
			player.shopQTY2.erase(player.shopQTY2.begin() + i);
			i--;
		}
	}
	for (int i{ 0 }; i < player.shop3.size(); i++)
	{
		if (player.shop3[i] == itemID::None)
		{
			player.shop3.erase(player.shop3.begin() + i);
			player.shopQTY3.erase(player.shopQTY3.begin() + i);
			i--;
		}
	}


	if (player.shop3.size() == 0)
	{
		currentPage = 2;
		maxPage = 2;
	}
	if (player.shop2.size() == 0)
	{
		currentPage = 1;
		maxPage = 1;
	}
}


