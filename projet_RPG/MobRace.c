#include <stdio.h>
#include <stdlib.h>

#include "MobRace.h"

/* Creats a new list of MobRace */
struct DlistRace *dlistRace_new(void) {
    struct DlistRace *p_new = malloc(sizeof *p_new);
    if (p_new != NULL) {
        p_new->length = 0;
        p_new->p_head = NULL;
        p_new->p_tail = NULL;
    }
    return p_new;
}

/* Adds element at the end of the list */
struct DlistRace *dlistRace_append(struct DlistRace *p_list, struct MobRace mobRace) {
    if (p_list != NULL) {
        struct nodeRace *p_new = malloc(sizeof *p_new);
        if (p_new != NULL) {
            p_new->mobRace = mobRace;
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

/* Returns a MobRace structure from a list and a position */
struct MobRace *returnListElementRace(struct DlistRace *p_list, int position) {
    int i = 0;
    struct nodeRace *p_temp = p_list->p_head;
    for (i = 0; i < position - 1; i++) {
        p_temp = p_temp->p_next;
    }
    struct MobRace race_temp = p_temp->mobRace;

    struct MobRace *p_race = MobRace_ctor(race_temp.name, race_temp.hp, race_temp.attack, race_temp.relativeDefense,
                                          race_temp.absoluteDefense, race_temp.dodge);
    return p_race;
}

/* Writes the whole list in a file */
void writeToFileRace(struct DlistRace *p_list) {
    FILE *fptr;
    fptr = fopen("./raceslist.txt", "w+");

    struct MobRace *mobRace = malloc(sizeof(struct MobRace));
    if (p_list != NULL) {
        struct nodeRace *p_temp = p_list->p_head;
        while (p_temp != NULL) {
            mobRace = &p_temp->mobRace;
            fwrite(mobRace, sizeof(struct MobRace), 1, fptr);
            p_temp = p_temp->p_next;
        }
    }
    fclose(fptr);
}

/* Returns a list of MobRace structures from a file */
struct DlistRace *readFromFileRace() {
    struct DlistRace *p_list = dlistRace_new();
    struct MobRace *mobRace = malloc(sizeof(struct MobRace));
    FILE *fptr;

    fptr = fopen("./raceslist.txt", "r");

    if (fptr) {
        while (fread(mobRace, sizeof(struct MobRace), 1, fptr) == 1) {
            dlistRace_append(p_list,
                             *MobRace_ctor(mobRace->name, mobRace->hp, mobRace->attack, mobRace->relativeDefense,
                                           mobRace->absoluteDefense, mobRace->dodge));
        }
    }

    fclose(fptr);
    return p_list;
}

/* MobRace constructor */
struct MobRace *MobRace_ctor(char *name, int hp, int attack, int relativeDefense, int absoluteDefense, int dodge) {
    struct MobRace *p = malloc(sizeof(struct MobRace));
    p->name = name;
    p->hp = hp;
    p->attack = attack;
    p->relativeDefense = relativeDefense;
    p->absoluteDefense = absoluteDefense;
    p->dodge = dodge;
    return p;
}

/* Generates a list of all the races in the game */
struct DlistRace *createRaces() {
    struct DlistRace *racesList = dlistRace_new();

    struct MobRace *human = MobRace_ctor("Human", 100, 10, 50, 10, 10);
    dlistRace_append(racesList, *human);
    struct MobRace *elf = MobRace_ctor("Elf", 100, 10, 50, 10, 10);
    dlistRace_append(racesList, *elf);
    struct MobRace *dwarf = MobRace_ctor("Dwarf", 100, 10, 50, 10, 10);
    dlistRace_append(racesList, *dwarf);
    struct MobRace *goblin = MobRace_ctor("Goblin", 100, 10, 50, 10, 10);
    dlistRace_append(racesList, *goblin);
    struct MobRace *skeleton = MobRace_ctor("Skeleton", 100, 10, 50, 10, 10);
    dlistRace_append(racesList, *skeleton);
    struct MobRace *troll = MobRace_ctor("Troll", 100, 10, 50, 10, 10);
    dlistRace_append(racesList, *troll);

    writeToFileRace(racesList);
    return racesList;
}

/* Function returning the list of the races
 * through the text file or by generating them */
struct DlistRace *getRaces() {
    struct DlistRace *racesList = dlistRace_new();
    if (racesList == readFromFileRace()) {
        return racesList;
    } else {
        return createRaces();
    }
}
