#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "Player.h"


void saveGame(struct Player *player);

struct Player *loadGame();

void introStory(struct Player *player);

void printGameWelcomeMenu();

int startGame();

void startMenu();

#endif
