#ifndef MOBRACE_H_INCLUDED
#define MOBRACE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "Usableitem.h"

struct MobRace {
    char *name;
    int hp;
    int attack;
    int relativeDefense;
    int absoluteDefense;
    int dodge;
};

struct nodeRace {
    struct MobRace mobRace;
    struct nodeRace *p_next;
    struct nodeRace *p_prev;
};

struct DlistRace {
    size_t length;
    struct nodeRace *p_tail;
    struct nodeRace *p_head;
};


struct DlistRace *dlistRace_new(void);

struct DlistRace *dlistRace_append(struct DlistRace *p_list, struct MobRace mobRace);

struct MobRace *returnListElementRace(struct DlistRace *p_list, int position);

void writeToFileRace(struct DlistRace *p_list);

struct DlistRace *readFromFileRace();

struct MobRace *MobRace_ctor(char *name, int hp, int attack, int relativeDefense, int absoluteDefense, int dodge);

struct DlistRace *createRaces();

struct DlistRace *getRaces();

#endif
