#pragma once

#include "Player.h"
#include "map_ID.h"
#include "map_dataBase.h"
#include "mob_dataBase.h"
#include "skill_ID.h"
#include "skill_dataBase.h"

int manage_combat(Player& player, mapID mID, string actionChoice);
void generateEnemy(Player& player, baseMob*& enemyMob, mapID mID, bool boss);
int playerTurn(Player& player, baseMob*& enemyMob, vector<string>& logs, int& turnCount);

void battleLogs(Player& player, baseMob*& enemyMob, vector<string>& logs); //Displays what's hapenning in the fight, above battleMenu. This function handles the logs vector.
int battleMenu(Player& player, baseMob*& enemyMob, int& rememberOption); //Displays the battle menu for the player
void updateLogs (Player& player, baseMob*& enemyMob, vector<string>& logs); //Updates logs (useful when enemy attacks)

void useSkill(Player& player, baseMob*& enemyMob, skillID skill, vector<string>& logs);

bool rollCrit(Player& player,int n); //rolls for a crit before performing a physical attack // int n in case we have a skill that adds a crit bonus to the attack
bool checkMiss(Player& player, baseMob*& enemyMob); //rolls and checks if the Skill/Attack will land or miss

void sortLogs(vector<string>& logs); //Pushes back every log back by 1, to make place for a new 15th log. (Used only if logs.size() == 15). 

void checkThorns(Player& player, baseMob*& enemyMob, vector<string>& logs);
void checkLifesteal(Player& player, baseMob*& enemyMob, vector<string>& logs, int& dmg);
void checkSpellvamp(Player& player, baseMob*& enemyMob, vector<string>& logs, int& dmg);
void checkOnHit(Player& player, baseMob*& enemyMob, vector<string>& logs, skillID skill, int& dmg); //Performs the 3 on-hit checks (thorns, lifesteal, spellvamp)

void combatStatus(Player& player, baseMob*& enemyMob); //Displays combat menu status screen
void combatPotions(Player& player, vector<string>& logs, bool& drink); //Displays combat menu item screen.
int usePotion(Player& player, vector<itemID>& items, vector<int>& itemsQTY, int atIndex, int cursorIndex, vector<string>& logs, bool& drink);
bool combatSkills(Player& player, vector<string>& logs, baseMob*& enemyMob);
bool combatSkillInfo(Player& player, skillID ID);
bool checkCost(Player& player, baseSkill skill); //checks if player has enough mp|hp to use skill
bool checkCooldown(Player& player, skillID& ID); //checks if selected skill is on cooldown

bool checkPlayerAlive(Player& player);
bool checkMobAlive(baseMob*& enemyMob);

void enemyTurn(Player& player, baseMob*& enemyMob, vector<string>& logs, int& turnCount);

void combatRewards(Player& player, baseMob*& enemyMob, vector<string>& logs);
void levelUp(Player& player);

void checkRegen(Player& player, vector<string>& logs, baseMob*& enemyMob, string s); //Checks and applies hp/mp regen for player or mob
void unlockMap(Player& player);

void applyBuffPot(Player& player, consumable_buffs b);
void applyBuffSkill(Player& player, skillBuff sBuff);
void clearPlayerBuffs(Player& player); //clears all player buffs
void tickPlayerBuffs(Player& player); //decrement player buffs and if buff turnAmount is at 0, remove buff

void clearPlayerDebuffs(Player& player);
void tickPlayerDebuffs(Player& player, vector<string>& logs, baseMob*& enemyMob);

void clearSkillCooldowns(Player& player); //Clears all of player's skill cooldowns
void tickSkillCooldowns(Player& player);

