#pragma once

#include "Player.h"
#include "map_ID.h"
#include "map_dataBase.h"

int manage_mapMenu(Player& player);
void print_mapMenuHeader();
int mapMenu(Player& player, int& rememberMap);
void print_mapHeader(baseMap map);
string mapAction(Player& player, baseMap& map, mapID& ID);
