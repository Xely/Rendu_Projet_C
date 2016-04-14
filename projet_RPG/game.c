#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "game.h"

#define SAVING_FILE "./gamesave.txt"


/* Saves the game by writing all the player's information in a file */
void saveGame(struct Player *player) {
    FILE *fp = 0;
    char *buffer = 0;

    struct Mob *mob = player->mob;
    struct MobRace *race = player->mob->mobRace;
    struct StuffItem *head = player->mob->equipment->head;
    struct StuffItem *chest = player->mob->equipment->chest;
    struct StuffItem *leggings = player->mob->equipment->leggings;
    struct StuffItem *boots = player->mob->equipment->boots;
    struct StuffItem *left = player->mob->equipment->leftHand;
    struct StuffItem *right = player->mob->equipment->rightHand;

    fp = fopen(SAVING_FILE, "w");
    fclose(fp);


    buffer = malloc(80);
    memset(buffer, 0, 80);

    snprintf(buffer, 80, "%d|%d|%d|\n", player->id, player->lives, player->gold);
    fp = fopen(SAVING_FILE, "a");
    fputs(buffer, fp);
    free(buffer);
    fclose(fp);


    buffer = malloc(80);
    memset(buffer, 0, 80);
    snprintf(buffer, 80, "%d|%s|%d|%d|%d|%d|%d|\n", mob->id, mob->name, mob->hp, mob->attack,
             mob->relativeDefense, mob->absoluteDefense, mob->dodge);
    fp = fopen(SAVING_FILE, "a");
    fputs(buffer, fp);
    free(buffer);
    fclose(fp);


    buffer = malloc(80);
    memset(buffer, 0, 80);
    snprintf(buffer, 80, "%d|%s|%d|%d|%d|%d|\n", race->hp, race->name, race->attack, race->relativeDefense,
             race->absoluteDefense, race->dodge);
    fp = fopen(SAVING_FILE, "a");
    fputs(buffer, fp);
    free(buffer);
    fclose(fp);


    buffer = malloc(80);
    memset(buffer, 0, 80);
    snprintf(buffer, 80, "%s|%d|%d|%d|%d|%d|\n", head->name, head->goldValue, head->typeId, head->attack,
             head->relativeDefense,
             head->absoluteDefense);
    fp = fopen(SAVING_FILE, "a");
    fputs(buffer, fp);
    free(buffer);
    fclose(fp);

    buffer = malloc(80);
    memset(buffer, 0, 80);
    snprintf(buffer, 80, "%s|%d|%d|%d|%d|%d|\n", chest->name, chest->goldValue, chest->typeId, chest->attack,
             chest->relativeDefense,
             chest->absoluteDefense);
    fp = fopen(SAVING_FILE, "a");
    fputs(buffer, fp);
    free(buffer);
    fclose(fp);

    buffer = malloc(80);
    memset(buffer, 0, 80);
    snprintf(buffer, 80, "%s|%d|%d|%d|%d|%d|\n", leggings->name, leggings->goldValue, leggings->typeId,
             leggings->attack, leggings->relativeDefense,
             leggings->absoluteDefense);
    fp = fopen(SAVING_FILE, "a");
    fputs(buffer, fp);
    free(buffer);
    fclose(fp);

    buffer = malloc(80);
    memset(buffer, 0, 80);
    snprintf(buffer, 80, "%s|%d|%d|%d|%d|%d|\n", boots->name, boots->goldValue, boots->typeId, boots->attack,
             boots->relativeDefense,
             boots->absoluteDefense);
    fp = fopen(SAVING_FILE, "a");
    fputs(buffer, fp);
    free(buffer);
    fclose(fp);

    buffer = malloc(80);
    memset(buffer, 0, 80);
    snprintf(buffer, 80, "%s|%d|%d|%d|%d|%d|\n", left->name, left->goldValue, left->typeId, left->attack,
             left->relativeDefense,
             left->absoluteDefense);
    fp = fopen(SAVING_FILE, "a");
    fputs(buffer, fp);
    free(buffer);
    fclose(fp);

    buffer = malloc(80);
    memset(buffer, 0, 80);
    snprintf(buffer, 80, "%s|%d|%d|%d|%d|%d|\n", right->name, right->goldValue, right->typeId, right->attack,
             right->relativeDefense,
             right->absoluteDefense);
    fp = fopen(SAVING_FILE, "a");
    fputs(buffer, fp);
    free(buffer);
    fclose(fp);

    int nbInventory = dlistItems_length(player->playerInventory);
    int i = 0;
    buffer = malloc(80);
    memset(buffer, 0, 80);
    snprintf(buffer, 80, "%d|\n", nbInventory);
    fp = fopen(SAVING_FILE, "a");
    fputs(buffer, fp);
    free(buffer);
    fclose(fp);
    for (i = 0; i < nbInventory; i++) {
        struct StuffItem *item = returnListElementItem(player->playerInventory, i + 1);
        buffer = malloc(80);
        memset(buffer, 0, 80);
        snprintf(buffer, 80, "%s|%d|%d|%d|%d|%d|\n", item->name, item->goldValue, item->typeId, item->attack,
                 item->relativeDefense,
                 item->absoluteDefense);
        fp = fopen(SAVING_FILE, "a");
        fputs(buffer, fp);
        free(buffer);
        fclose(fp);
    }

    int nbPotions = dlistUsable_length(player->playerPotions);
    buffer = malloc(80);
    memset(buffer, 0, 80);
    snprintf(buffer, 80, "%d|\n", nbPotions);
    fp = fopen(SAVING_FILE, "a");
    fputs(buffer, fp);
    free(buffer);
    fclose(fp);
    for (i = 0; i < nbPotions; i++) {
        struct UsableItem *potion = returnListElementUsable(player->playerPotions, i + 1);
        buffer = malloc(80);
        memset(buffer, 0, 80);
        snprintf(buffer, 80, "%s|%d|%d|%d|%d|%d|%d|%d|%d|%d|\n", potion->name, potion->goldValue, potion->totalDuration,
                 potion->durationLeft, potion->hp,
                 potion->attack, potion->relativeDefense, potion->absoluteDefense, potion->dodge, potion->used);
        fp = fopen(SAVING_FILE, "a");
        fputs(buffer, fp);
        free(buffer);
        fclose(fp);
    }
}

/* Loads the game by reading the information in a file */
struct Player *loadGame() {
    FILE *fp = 0;
    char *buffer = 0;
    char tokens[] = "|\n";
    struct Player *player = malloc(sizeof(struct Player));
    struct Mob *mob = malloc(sizeof(struct Mob));
    struct MobRace *race = malloc(sizeof(struct MobRace));
    struct StuffItem *head = malloc(sizeof(struct StuffItem));
    struct StuffItem *chest = malloc(sizeof(struct StuffItem));
    struct StuffItem *leggings = malloc(sizeof(struct StuffItem));
    struct StuffItem *boots = malloc(sizeof(struct StuffItem));
    struct StuffItem *left = malloc(sizeof(struct StuffItem));
    struct StuffItem *right = malloc(sizeof(struct StuffItem));
    struct Equipment *equipment = malloc(sizeof(struct Equipment));


    buffer = malloc(80);
    memset(buffer, 0, 80);


    fp = fopen(SAVING_FILE, "r");

    if (fgets(buffer, 80, fp)) {
        char *temp = malloc(sizeof(char) * 20);
        player->id = atoi(strtok(buffer, tokens));
        player->lives = atoi(strtok(NULL, tokens));
        player->gold = atoi(strtok(NULL, tokens));
    }

    if (fgets(buffer, 80, fp)) {
        char *temp = malloc(sizeof(char) * 20);
        mob->id = atoi(strtok(buffer, tokens));
        strcpy(temp, strtok(NULL, tokens));
        mob->name = temp;
        mob->hp = atoi(strtok(NULL, tokens));
        mob->attack = atoi(strtok(NULL, tokens));
        mob->relativeDefense = atoi(strtok(NULL, tokens));
        mob->absoluteDefense = atoi(strtok(NULL, tokens));
        mob->dodge = atoi(strtok(NULL, tokens));
    }

    if (fgets(buffer, 80, fp)) {
        char *temp = malloc(sizeof(char) * 20);
        race->hp = atoi(strtok(buffer, tokens));
        strcpy(temp, strtok(NULL, tokens));
        race->name = temp;
        race->attack = atoi(strtok(NULL, tokens));
        race->relativeDefense = atoi(strtok(NULL, tokens));
        race->absoluteDefense = atoi(strtok(NULL, tokens));
        race->dodge = atoi(strtok(NULL, tokens));
    }

    if (fgets(buffer, 80, fp)) {
        char *temp = malloc(sizeof(char) * 20);
        strcpy(temp, strtok(buffer, tokens));
        head->name = temp;
        head->goldValue = atoi(strtok(NULL, tokens));
        head->typeId = atoi(strtok(NULL, tokens));
        head->relativeDefense = atoi(strtok(NULL, tokens));
        head->absoluteDefense = atoi(strtok(NULL, tokens));
        equipment->head = head;
    }


    if (fgets(buffer, 80, fp)) {
        char *temp = malloc(sizeof(char) * 20);
        strcpy(temp, strtok(buffer, tokens));
        chest->name = temp;
        chest->goldValue = atoi(strtok(NULL, tokens));
        chest->typeId = atoi(strtok(NULL, tokens));
        chest->relativeDefense = atoi(strtok(NULL, tokens));
        chest->absoluteDefense = atoi(strtok(NULL, tokens));
        equipment->chest = chest;
    }

    if (fgets(buffer, 80, fp)) {
        char *temp = malloc(sizeof(char) * 20);
        strcpy(temp, strtok(buffer, tokens));
        leggings->name = temp;
        leggings->goldValue = atoi(strtok(NULL, tokens));
        leggings->typeId = atoi(strtok(NULL, tokens));
        leggings->relativeDefense = atoi(strtok(NULL, tokens));
        leggings->absoluteDefense = atoi(strtok(NULL, tokens));
        equipment->leggings = leggings;
    }

    if (fgets(buffer, 80, fp)) {
        char *temp = malloc(sizeof(char) * 20);
        strcpy(temp, strtok(buffer, tokens));
        boots->name = temp;
        boots->goldValue = atoi(strtok(NULL, tokens));
        boots->typeId = atoi(strtok(NULL, tokens));
        boots->relativeDefense = atoi(strtok(NULL, tokens));
        boots->absoluteDefense = atoi(strtok(NULL, tokens));
        equipment->boots = boots;
    }

    if (fgets(buffer, 80, fp)) {
        char *temp = malloc(sizeof(char) * 20);
        strcpy(temp, strtok(buffer, tokens));
        left->name = temp;
        left->goldValue = atoi(strtok(NULL, tokens));
        left->typeId = atoi(strtok(NULL, tokens));
        left->relativeDefense = atoi(strtok(NULL, tokens));
        left->absoluteDefense = atoi(strtok(NULL, tokens));
        equipment->leftHand = left;
    }

    if (fgets(buffer, 80, fp)) {
        char *temp = malloc(sizeof(char) * 20);
        strcpy(temp, strtok(buffer, tokens));
        right->name = temp;
        right->goldValue = atoi(strtok(NULL, tokens));
        right->typeId = atoi(strtok(NULL, tokens));
        right->relativeDefense = atoi(strtok(NULL, tokens));
        right->absoluteDefense = atoi(strtok(NULL, tokens));
        equipment->rightHand = right;
    }

    int nbInventory = 0;
    int i = 0;
    struct DlistItems *inventory = dlistItems_new();
    if (fgets(buffer, 80, fp)) {
        nbInventory = atoi(strtok(buffer, tokens));
    }
    for (i = 0; i < nbInventory; i++) {
        if (fgets(buffer, 80, fp)) {
            struct StuffItem *item = malloc(sizeof(struct StuffItem));
            char *temp = malloc(sizeof(char) * 20);
            strcpy(temp, strtok(buffer, tokens));
            item->name = temp;
            item->goldValue = atoi(strtok(NULL, tokens));
            item->typeId = atoi(strtok(NULL, tokens));
            item->relativeDefense = atoi(strtok(NULL, tokens));
            item->absoluteDefense = atoi(strtok(NULL, tokens));
            dlistItems_append(inventory,
                              *StuffItem_ctor(item->name, item->goldValue, item->typeId, item->hp, item->attack,
                                              item->relativeDefense, item->absoluteDefense));
        }
    }

    fclose(fp);
    free(buffer);
    mob->mobRace = race;
    mob->equipment = equipment;
    player->mob = mob;
    player->playerInventory = inventory;
    return player;
}

/* Displays some lore for the start of the game */
void introStory(struct Player *player) {
    int timer = clock();

    printGameWelcomeMenu();

    printf("\n%s, it is time to step out of your comfortable home, the world of Valhrora needs you and your skills.\n",
           player->mob->name);
    while (clock() < timer + 4000) {
    }
    printf("A large number of castles, dungeons and other places have been infested by malicious creatures.\n");
    while (clock() < timer + 8000) {
    }
    printf("One of these dungeons is along this road, five minutes of walk to the north.\n");
    while (clock() < timer + 12000) {
    }
    printf("You will need all the help you can have, so take these potions I have concocted.\n");
    printf("(Three potions have been added to your inventory.)\n");
    player->playerPotions = selectFirstPotions();
    while (clock() < timer + 16000) {
    }
    printf("Go, %s, save this world.\n", player->mob->name);
    while (clock() < timer + 20000) {
    }
    printf("After a short walk, the dungeon appears before you...");
    while (clock() < timer + 24000) {
    }
}

/* Prints a menu for a new game */
void printGameWelcomeMenu() {
    system("cls");
    printf(" =================================  \n");
    printf("||                               || \n");
    printf("||                               || \n");
    printf("||           Welcome to          || \n");
    printf("||                               || \n");
    printf("||       THE CHRONICLES OF       || \n");
    printf("||                               || \n");
    printf("||            VALHORA            || \n");
    printf("||                               || \n");
    printf("||                               || \n");
    printf(" =================================  \n\n");
}


/* Manages the main parts of the game */
int startGame() {
    printGameWelcomeMenu();
    char name[20];

    printf("Hello young aventurer, and welcome to the Chronicles of Valhora.\nWhat is your name ? ");
    fflush(stdin);
    fflush(stdout);
    scanf("%s", name);

    struct Player *playerCharacter = createPlayer(name, 10);

    introStory(playerCharacter);

    while (1) {
        startDungeon(playerCharacter);
        enterTavern(playerCharacter);
    }
}

/* Displays a menu for the start of the game */
void startMenu() {
    char userInput = '0';

    printf(" =================================  \n");
    printf("||                               || \n");
    printf("||  1- Start new game            || \n");
    printf("||                               || \n");
    printf("||  2- Load last game            || \n");
    printf("||                               || \n");
    printf("||  3- Leave game                || \n");
    printf("||                               || \n");
    printf("||                               || \n");
    printf("||                               || \n");
    printf(" =================================  \n\n");

    fflush(stdin);
    scanf("%c", &userInput);

    if (userInput == '1') {
        startGame();
    } else if (userInput == '2') {
        struct Player *player = loadGame();
        enterTavern(player);
    } else if (userInput == '3') {
        exit(0);
    } else {
        startMenu();
    }

}
