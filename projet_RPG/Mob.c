#include <stdio.h>
#include <stdlib.h>
#include "Mob.h"
#include "Player.h"

int calcAttack(struct Player *player);

int calcRelDef(struct Player *player);

int calcAbsDef(struct Player *player);

int calcHp(struct Player *player);

void hitMob(struct Player *attacker, struct Mob *defender);

void hitPlayer(struct Mob *attacker, struct Player *defender);

char *mobClass(int n);

void createRandomMob(int id, int difficulty, struct Player *player);

void createDungeonMobs(int mobNumber, struct Player *player);


/* Creates a new list of mobs */
struct DlistMob *dlistMob_new(void) {
    struct DlistMob *p_new = malloc(sizeof *p_new);
    if (p_new != NULL) {
        p_new->length = 0;
        p_new->p_head = NULL;
        p_new->p_tail = NULL;
    }
    return p_new;
}

/* Adds element at the end of the list */
struct DlistMob *dlistMob_append(struct DlistMob *p_list, struct Mob mob) {
    if (p_list != NULL) {
        struct nodeMob *p_new = malloc(sizeof *p_new);
        if (p_new != NULL) {
            p_new->mob = mob;
            p_new->p_next = NULL;
            if (p_list->p_tail == NULL) {
                p_new->p_prev = NULL;
                p_list->p_head = p_new;
                p_list->p_tail = p_new;
            } else {
                p_list->p_tail->p_next = p_new;
                p_new->p_prev = p_list->p_tail;
                p_list->p_tail = p_new;
            }
            p_list->length++;
        }
    }
    return p_list;
}

/* Returns the length of a mob list */
size_t dlistMob_length(struct DlistMob *p_list) {
    size_t ret = 0;
    if (p_list != NULL) {
        ret = p_list->length;
    }
    return ret;
}

/* Returns a Mob structure from a list */
struct Mob *returnListElementMob(struct DlistMob *p_list, int position) {
    int i = 0;
    struct nodeMob *p_temp = p_list->p_head;
    for (i = 0; i < position - 1; i++) {
        p_temp = p_temp->p_next;
    }
    struct Mob mob_temp = p_temp->mob;

    struct Mob *p_mob = Mob_ctor(mob_temp.id, mob_temp.name, mob_temp.mobRace, mob_temp.hp, mob_temp.attack,
                                 mob_temp.relativeDefense, mob_temp.absoluteDefense, mob_temp.dodge,
                                 mob_temp.equipment);
    return p_mob;
}

/* Writes the whole list in a file */
void writeToFileMob(struct DlistMob *p_list) {
    FILE *fptr;
    fptr = fopen("./moblist.txt", "w+");

    struct Mob *mob = malloc(sizeof(struct Mob));
    if (p_list != NULL) {
        struct nodeMob *p_temp = p_list->p_head;
        while (p_temp != NULL) {
            mob = &p_temp->mob;
            fwrite(mob, sizeof(struct Mob), 1, fptr);
            p_temp = p_temp->p_next;
        }
    }
    fclose(fptr);
}

/* Returns a mob list from a text file */
struct DlistMob *readFromFileMob() {
    struct DlistMob *p_list = dlistMob_new();
    struct Mob *mob = malloc(sizeof(struct Mob));
    FILE *fptr;

    fptr = fopen("./moblist.txt", "r");

    if (fptr) {
        while (fread(mob, sizeof(struct Mob), 1, fptr) == 1) {
            dlistMob_append(p_list, *Mob_ctor(mob->id, mob->name, mob->mobRace, mob->hp, mob->attack,
                                              mob->relativeDefense, mob->absoluteDefense, mob->dodge, mob->equipment));
        }
    }

    fclose(fptr);
    return p_list;
}

/* Constructor for Mob structure */
struct Mob *Mob_ctor(int id, char *name, struct MobRace *mobRace, int hp, int attack, int relativeDefense,
                     int absoluteDefense, int dodge, struct Equipment *equipment) {
    struct Mob *p = malloc(sizeof(struct Mob));
    p->id = id;
    p->name = name;
    p->mobRace = mobRace;
    p->hp = hp;
    p->attack = attack;
    p->relativeDefense = relativeDefense;
    p->absoluteDefense = absoluteDefense;
    p->dodge = dodge;
    p->equipment = equipment;

    return p;
}

/* Generates a list of mobs with only the player in it */
struct DlistMob *createMob(struct Mob *playerMob) {
    struct DlistMob *mobList = dlistMob_new();
    dlistMob_append(mobList, *playerMob);

    writeToFileMob(mobList);
    return mobList;
}

/* Returns a list of mobs,  either through the text file
 * or by generating it */
struct DlistMob *getMobs(struct Mob *playerMob) {
    struct DlistMob *mobList = dlistMob_new();
    if (mobList == readFromFileMob()) {
        return mobList;
    } else {
        return createMob(playerMob);
    }
}

/* Returns the actual attack of a player, taking his item and used potions into account */
int calcAttack(struct Player *player) {
    int i = 0;
    int attack = player->mob->attack;
    int nbPotions = dlistUsable_length(player->playerPotions);
    for (i = 0; i < nbPotions; i++) {
        struct UsableItem *potion = returnListElementUsable(player->playerPotions, i + 1);
        if (potion->used = 1 && potion->attack != 0) {
            attack += potion->attack;
        }
    }
    attack += player->mob->equipment->head->attack;
    attack += player->mob->equipment->chest->attack;
    attack += player->mob->equipment->leggings->attack;
    attack += player->mob->equipment->boots->attack;
    attack += player->mob->equipment->leftHand->attack;
    attack += player->mob->equipment->rightHand->attack;

    return attack;
}

/* Returns the actual relative defense of a player,
 * taking his item and used potions into account*/
int calcRelDef(struct Player *player) {
    int i = 0;
    int relDef = player->mob->relativeDefense;
    int nbPotions = dlistUsable_length(player->playerPotions);
    for (i = 0; i < nbPotions; i++) {
        struct UsableItem *potion = returnListElementUsable(player->playerPotions, i + 1);
        if (potion->used = 1 && potion->relativeDefense != 0) {
            relDef += potion->relativeDefense;
        }
    }
    relDef += player->mob->equipment->head->relativeDefense;
    relDef += player->mob->equipment->chest->relativeDefense;
    relDef += player->mob->equipment->leggings->relativeDefense;
    relDef += player->mob->equipment->boots->relativeDefense;
    relDef += player->mob->equipment->leftHand->relativeDefense;
    relDef += player->mob->equipment->rightHand->relativeDefense;

    return relDef;
}

/* Returns the actual absolute defense of a player, taking his items and used potions into account */
int calcAbsDef(struct Player *player) {
    int i = 0;
    int absDef = player->mob->absoluteDefense;
    int nbPotions = dlistUsable_length(player->playerPotions);
    for (i = 0; i < nbPotions; i++) {
        struct UsableItem *potion = returnListElementUsable(player->playerPotions, i + 1);
        if (potion->used = 1 && potion->absoluteDefense != 0) {
            absDef += potion->absoluteDefense;
        }
    }
    absDef += player->mob->equipment->head->absoluteDefense;
    absDef += player->mob->equipment->chest->absoluteDefense;
    absDef += player->mob->equipment->leggings->absoluteDefense;
    absDef += player->mob->equipment->boots->absoluteDefense;
    absDef += player->mob->equipment->leftHand->absoluteDefense;
    absDef += player->mob->equipment->rightHand->absoluteDefense;

    return absDef;
}

/* Returns the actual hp of a player, taking his items into account */
int calcHp(struct Player *player) {
    int i = 0;
    int hp = player->mob->hp;
    hp += player->mob->equipment->head->hp;
    hp += player->mob->equipment->chest->hp;
    hp += player->mob->equipment->leggings->hp;
    hp += player->mob->equipment->boots->hp;
    hp += player->mob->equipment->leftHand->hp;
    hp += player->mob->equipment->rightHand->hp;

    return hp;
}

/* Attack of the player againt a mob */
void hitMob(struct Player *attacker, struct Mob *defender) {
    int hit = 0;
    int playerAttack = calcAttack(attacker);

    int dodgeRand = dice(defender->dodge);
    printf("You attack %s ", defender->name);

    hit += dice(6) + dice(playerAttack) + dice(playerAttack);
    hit = hit * (1 - (int) (defender->relativeDefense / 100));
    hit = hit - defender->absoluteDefense;
    hit = max(hit, 0);

    if (dodgeRand != 1) {
        defender->hp -= hit;
        printf("and inflicts %d damage.\n", hit);
    } else {
        printf("but %s dodges the hit!\n", defender->name);
    }
}

/* Attack of a monster against the player */
void hitPlayer(struct Mob *attacker, struct Player *defender) {
    int playerRelDef = calcRelDef(defender);
    int playerAbsDef = calcAbsDef(defender);
    int playerDodge = defender->mob->dodge;
    int hit = 0;
    int dodgeRand = dice(playerDodge);


    printf("%s attacks you ", attacker->name);

    hit += dice(6) + dice(attacker->attack) + dice(attacker->attack);
    hit = hit * (1 - (int) (playerRelDef / 100));
    hit = hit - playerAbsDef;
    hit = max(hit, 0);

    if (dodgeRand != 1) {
        defender->mob->hp -= hit;
        printf("and inflicts %d damage to you!\n", hit);
    } else {
        printf("but you dodge the hit!\n");
    }
}

/* Returns a class name for a mob from the random number argument */
char *mobClass(int n) {
    char *classMob = malloc(sizeof(char) * 15);
    switch (n) {
        case 0 :
            classMob = " elementalist";
            break;
        case 1 :
            classMob = " ranger";
            break;
        case 2 :
            classMob = " barbarian";
            break;
        case 3 :
            classMob = " necromancer";
            break;
        case 4 :
            classMob = " bandit";
            break;
        case 5 :
            classMob = " illusionist";
            break;
        case 6 :
            classMob = " engineer";
            break;
    }
    return classMob;
}

/* Creates a single mob from a dungeon's difficulty
 * and adds it to the player's moblist */
void createRandomMob(int id, int difficulty, struct Player *player) {
    int mobRaceRandom = doRand(0, 6);
    struct StuffItem *mobEquipment = Equipment_ctor(returnListElementItem(getItems(), 0),
                                                    returnListElementItem(getItems(), 1),
                                                    returnListElementItem(getItems(), 2),
                                                    returnListElementItem(getItems(), 3),
                                                    returnListElementItem(getItems(), 4),
                                                    returnListElementItem(getItems(), 5));

    char *mobName = malloc(sizeof(char) * 30);
    char *race = malloc(sizeof(char) * 10);
    race = returnListElementRace(getRaces(), mobRaceRandom)->name;
    mobName = strcat(mobName, race);
    mobName = strcat(mobName, mobClass(doRand(0, 7)));

    struct Mob *randomMob = Mob_ctor(id, mobName, returnListElementRace(getRaces(), mobRaceRandom),
                                     70 + difficulty * 5, 9 + floor(difficulty / 5), 20 + difficulty,
                                     3 + floor(difficulty / 5), 20, mobEquipment);

    dlistMob_append(player->mobList, *randomMob);
}

/* Creates the list of mobs for a new dungeon */
void createDungeonMobs(int mobNumber, struct Player *player) {

    int i = 0;
    for (i = 0; i < mobNumber; i++) {
        createRandomMob(i + 1, player->dungeonsFinished + 1, player);
    }
}
