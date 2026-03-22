#pragma once

#include "Player.h"

void print_chooseSaveHeader(); //Prints the loadMenu header
void manage_LoadMenu(Player& player); //Main function
int get_SlotChoice(); //Prompts and gets which save slot user chose
bool checkSlot(int saveSlot); //Checks if save slot selected exists or not
void print_newSaveHeader();
void create_newSave(Player& player, int saveSlot);
void print_loadSaveHeader();
void load_Save(Player& player, int saveSlot);
//void load_equipStats(Player& player);

void gameOver(Player& player, int saveSlot);


