#ifndef TAVERN_H_INCLUDED
#define TAVERN_H_INCLUDED

#include "Player.h"


void enterToContinue();

void printPotion(struct Player *player, int n);

void managePotions(struct Player *player);

void printItem(struct Player *player, int n);

void manageItems(struct Player *player);

void printProfile(struct Player *player);

int manageInventory(struct Player *player);

int printWeaponBuy(int n);

int printArmorBuy(int n);

int printPotionBuy(int n);

void tavernBuyStuff(struct Player *player, char C);

void tavernBuy(struct Player *player);

void printItemSell(struct Player *player, int n);

void printPotionSell(struct Player *player, int n);

void tavernSellStuff(struct Player *player, int n);

void tavernSell(struct Player *player);

int tavernTrade(struct Player *player);

void tavernRest(struct Player *player);

void quitGame(struct Player *player);

void enterTavern(struct Player *player);

#endif // TAVERN_H_INCLUDED
