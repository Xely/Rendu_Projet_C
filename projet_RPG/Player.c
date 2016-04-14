#include <stdio.h>
#include <stdlib.h>

#include "Player.h"

/* Manages the end of the game when the player has no lives left */
void playerPermanentDeath(struct Player *player) {
    char *userInput = '0';
    system("cls");
    printf("-----GAME OVER-----\n");
    printf("You have died and have no lives left.\n\n");
    printf("Here is a recap of your accomplishments during your quest through Valhora:\n");
    printf("You have killed a total of %d mobs.\n", player->mobsKilled);
    printf("You have completed a total of %d dungeons.\n", player->dungeonsFinished);

    printf("Press (S) to start a new game or (Q) to leave the game");
    fflush(stdin);
    scanf("%c", &userInput);
    if (userInput == 'S' || userInput == 's') {
        startGame();
    } else if (userInput == 'Q' || userInput == 'q') {
        exit(0);
    }
}

/* Prints some lore whenever the player dies but still has lives left */
void playerDeath(struct Player *player) {
    system("cls");
    int strt_tmr = clock();
    printf("After several long minutes of pain, you heart finally stops...\n\n");
    while (clock() < strt_tmr + 5000) {
    }
    printf("But as the light was about to disapear forever inside you, \nyou start feeling energy back...\n\n");
    while (clock() < strt_tmr + 10000) {
    }
    printf("You wake up near the tavern, as if nothing ever happened...\n");
    while (clock() < strt_tmr + 15000) {
    }
    player->lives -= 1;
    player->mob->hp = player->mob->mobRace->hp;
    enterTavern(player);
}

/* Constructor for a Player structure */
struct Player *Player_ctor(int id, struct Mob *mob, int lives, int gold, struct DlistItems *playerInventory,
                           struct DlistUsable *playerPotions, struct DlistMob *mobList) {
    struct Player *p = malloc(sizeof(struct Player));
    p->id = id;
    p->mob = mob;
    p->lives = lives;
    p->gold = gold;
    p->playerInventory = playerInventory;
    p->playerPotions = playerPotions;
    p->mobList = mobList;
    p->dungeonsFinished = 0;
    p->mobsKilled = 0;
    p->playerInventory = dlistItems_new();
    p->playerPotions = dlistUsable_new();
    p->mobList = dlistMob_new();
    return p;
}

/* Manages the creation of the player at the start of the game */
struct Player *createPlayer(char *name, int pointsToAttribut) {
    char userInput = '0';
    struct Equipment *playerEquipment = Equipment_ctor(returnListElementItem(getItems(), 7),
                                                       returnListElementItem(getItems(), 1),
                                                       returnListElementItem(getItems(), 2),
                                                       returnListElementItem(getItems(), 3),
                                                       returnListElementItem(getItems(), 4),
                                                       returnListElementItem(getItems(), 5));

    struct Mob *playerMob = Mob_ctor(0, name, returnListElementRace(getRaces(), 1), 100, 10, 50, 10, 10,
                                     playerEquipment);

    struct Player *player = Player_ctor(0, playerMob, 3, 500, createFirstInventory(), selectFirstPotions(),
                                        createMob(playerMob));

    while (pointsToAttribut>0) {
        system("cls");
        printf("You have %d points left to increase your attributes, which one do you want to increase ?\n", pointsToAttribut);
        printf("Increase Attack: %d (A) \nRelative Defense: %d (R) \nor Absolute Defense: %d (C) ? ",
               player->mob->attack, player->mob->relativeDefense, player->mob->absoluteDefense);
        fflush(stdin);
        scanf("%c", &userInput);
        if (userInput == 'A' || userInput == 'a') {
            player->mob->attack += 1;
            printf("Attack increased by 1!\n");
            pointsToAttribut -= 1;
        } else if (userInput == 'R' || userInput == 'r') {
            player->mob->relativeDefense += 1;
            printf("Relative Defense increased by 1!\n");
            pointsToAttribut -= 1;
        } else if (userInput == 'C' || userInput == 'c') {
            player->mob->absoluteDefense += 2;
            pointsToAttribut -= 1;
            printf("Absolute Defense increased by 2!\n");
        } else {
            printf("Please type a character out of given choices");
        }
        enterToContinue();
    }
    return player;
}
