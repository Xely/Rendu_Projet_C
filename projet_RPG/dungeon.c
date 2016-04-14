#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "dungeon.h"


/* Removes one turn of duration to the potions in effect */
void checkPotionsDuration(struct Player *player) {
    int nbPotions = dlistUsable_length(player->playerPotions);
    int i = 0;

    for (i = 0; i < nbPotions; i++) {
        struct UsableItem *potion = returnListElementUsable(player->playerPotions, i + 1);
        if (potion->used == 1 && potion->durationLeft != 0) {
            potion->durationLeft -= 1;
            if (potion->hp != 0) {
                printf("You recover %d hp from your %s potion.\n", potion->hp, potion->name);
            }
        }
        if (potion->durationLeft == 0) {
            dlistUsable_remove_id(player->playerPotions, i + 1);
            printf("The effect of the %s has wore off!\n", potion->name);
        }
    }
}

/* Delete the potions in effect from player's inventory */
void resetPotions(struct Player *player) {
    int n = dlistUsable_length(player->playerPotions);
    int i = 0;

    for (i = 0; i < n; i++) {
        struct UsableItem *potion = returnListElementUsable(player->playerPotions, i + 1);
        if (potion->used) {
            dlistUsable_remove_id(player->playerPotions, i + 1);
        }
    }
}

/* Prints all inforamtion of a selected potion in order
 * to be used or not, n being the number of the potion in the list */
void printPotionFight(struct Player *player, int n) {
    int potionMax = dlistUsable_length(player->playerPotions);
    struct UsableItem *potion = returnListElementUsable(player->playerPotions, n);
    int potionUsableMax = 0;
    int j = 0;

    for (j = 0; j < potionMax; j++) {
        struct UsableItem *potion = returnListElementUsable(player->playerPotions, j + 1);
        if (potion->used == 0) {
            potionUsableMax++;
        }
    }

    printf("\n#### Potion %d/%d: ####\n", n, potionUsableMax);
    printf("%s\n", potion->name);
    printf("This potion gives");
    if (potion->hp != 0) {
        printf(" %d hp every turn ", potion->hp);
    } else if (potion->attack != 0) {
        printf(" %d attack ", potion->attack);
    } else if (potion->relativeDefense != 0) {
        printf(" %d relative defense ", potion->relativeDefense);
    } else if (potion->absoluteDefense != 0) {
        printf(" %d absolute defense ", potion->absoluteDefense);
    } else if (potion->dodge != 0) {
        printf(" %d dodge ", potion->dodge);
    }
    if (potion->totalDuration != 0) {
        printf("for %d turns", potion->totalDuration);
    }
    printf(".\n");
    printf("Use it (U), check the next potion (N) or go back to the fight (Q) ? ");
}

/* Menu where the player chooses to use a potion */
int usePotionFight(struct Player *player) {
    int i = 1;
    int j = 0;
    char userInput = '0';
    int potionMax = dlistUsable_length(player->playerPotions);

    if (potionMax == 0) {
        printf("You have no potions.\n");
        return 0;
    }

    Label :
    while (1) {
        struct UsableItem *potion = malloc(sizeof(struct UsableItem));
        potion = returnListElementUsable(player->playerPotions, i);
        if (potion->used == 0) {
            printPotionFight(player, i);
        } else {
            i++;
            goto Label;
        }
        fflush(stdin);
        scanf("%c", &userInput);
        if (userInput == 'U' || userInput == 'u') {
            struct UsableItem *potion = returnListElementUsable(player->playerPotions, i);
            potion->used = 1;
            printf("%s used!\n", potion->name);
            return 1;
        } else if (userInput == 'N' || userInput == 'n') {
            if (i == dlistUsable_length(player->playerPotions)) {
                i = 1;
            } else {
                i++;
            }
        } else if (userInput == 'Q' || userInput == 'q') {
            return 0;
        }
    }
}

/* Simulates the battle between the player and a mob, until one of them dies */
void fightMob(struct Player *player, struct Mob *mob) {
    /* Flag will detect if either the player or the mob dies during the encounter */
    int flag = 1;
    int timer;
    char *userInput = '0';

    fflush(stdout);
    fflush(stdin);
    printf("You encounter a new monster: %s.\n", mob->name);
    printf(">>> The fight starts! <<<\n");

    while (flag) {
        checkPotionsDuration(player);

        while (userInput != 'A' && userInput != 'a') {
            printf("\nWhat do you want to do ?\n");
            printf("Attack the monster (A) or use a potion (P) ? ");
            fflush(stdin);
            scanf("%c", &userInput);
            if (userInput == 'P' || userInput == 'p') {
                usePotionFight(player);
            }
        }
        hitMob(player, mob);
        if (mob->hp < 1) {
            flag = 0;
            fflush(stdout);
            printf("You defeated %s!\n", mob->name);
            player->mobsKilled += 1;
            /* Gives the player some gold based on the dungeon's difficulty */
            int goldEarned = doRand(player->dungeonsFinished * 5 + 5, player->dungeonsFinished * 5 + 15);
            printf("You earned %d gold.\n\n", goldEarned);
            player->gold += goldEarned;
        }

        if (flag) {
            hitPlayer(mob, player);
            if (calcHp(player) < 1) {
                flag = 0;
                printf("You died! RIP\n");
                printf("(Press (ENTER) to continue)");
                enterToContinue();
                if (player->lives > 0) {
                    playerDeath(player);
                } else {
                    playerPermanentDeath(player);
                }
            }
        }
        userInput = '0';
    }
}

/* Manages the end of a dungeon by writing some lore and giving the player a reward */
void endDungeon(struct Player *player) {
    player->dungeonsFinished += 1;
    resetPotions(player);
    int userInput = '0';
    int timer = clock();

    system("cls");
    printf("\nAs you defeat the last remaining monster in the dungeon, you walk forward \n");
    printf("and enter in what appears to be the former treasure room of the dungeon...\n\n");
    while (clock() < timer + 6000) {
    }
    printf("Most of the treasures seems to be gone...\n\n");
    while (clock() < timer + 10000) {
    }
    printf("However, in the back of the room, under a pile of junk,\n");
    printf("you find a item that could very well serve you...\n\n");
    while (clock() < timer + 16000) {
    }

    int n = dlistItems_length(getItems());
    int rand = doRand(7, n);
    struct StuffItem *loot = returnListElementItem(getItems(), rand);
    dlistItems_append(player->playerInventory, *loot);
    printf("--%s has been added to your inventory!!!--\n\n", loot->name);
    printf("After an entire day spent in this dungeon, you feel exhausted and could use some rest at the local tavern.\n");
    fflush(stdin);
    fflush(stdout);
    printf("(Press (C) to continue)");
    while (userInput != 'C' && userInput != 'c') {
        scanf("%c", &userInput);
    }
}

/* Manages the dungeons fights and ending */
void startDungeon(struct Player *player) {
    int i = 0;
    int mobNumber = doRand(4, 7);
    player->mobList = dlistMob_new();

    system("cls");
    printf("You enter the dungeon.\n");
    createDungeonMobs(mobNumber, player);
    printf("There are %d monsters in the dungeon.\n", dlistMob_length(player->mobList) - 1);

    for (i = 0; i < dlistMob_length(player->mobList) - 1; i++) {
        struct Mob *mob = returnListElementMob(player->mobList, i + 2);
        fightMob(player, mob);
        if (i == dlistMob_length(player->mobList) - 2) {
            printf("(Press (ENTER) to continue) ");
            enterToContinue();
            endDungeon(player);
        }
    }
}
