#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>

#include "map_ID.h"
using namespace std;


struct baseMap
{
	string name;
	string description; //Used to tell lore about map
};


inline unordered_map<mapID, baseMap> mapDataBase
{
	{mapID::Meadows , {"The Meadows", ""}},
	{mapID::DarkForest , {"The Dark Forest", ""}},
	{mapID::UndeadCatacombs , {"The Undead Catacombs", ""}},
	{mapID::SunkenPassage , {"The Sunken Passage", ""}},
	{mapID::DeadShore , {"The Dead Shore", ""}}
};