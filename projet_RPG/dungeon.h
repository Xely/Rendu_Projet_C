#ifndef DUNGEON_H_INCLUDED
#define DUNGEON_H_INCLUDED

#include "Player.h"



void checkPotionsDuration(struct Player *player);

void resetPotions(struct Player *player);

void printPotionFight(struct Player *player, int n);

int usePotionFight(struct Player *player);

void fightMob(struct Player *player, struct Mob *mob);

void endDungeon(struct Player *player);

void startDungeon(struct Player *player);

#endif // DUNGEON_H_INCLUDED
