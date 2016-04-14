#ifndef MOB_H_INCLUDED
#define MOB_H_INCLUDED

#include "MobRace.h"
#include "Equipement.h"

struct Mob {
    int id;
    char *name;
    struct MobRace *mobRace;
    int hp;
    int attack;
    int relativeDefense;
    int absoluteDefense;
    int dodge;
    struct Equipment *equipment;
    // loot
};

struct nodeMob {
    struct Mob mob;
    struct nodeMob *p_next;
    struct nodeMob *p_prev;
};

struct DlistMob {
    size_t length;
    struct nodeMob *p_tail;
    struct nodeMob *p_head;
};


struct DlistMob *dlistMob_new(void);

struct DlistMob *dlistMob_append(struct DlistMob *p_list, struct Mob mob);

size_t dlistMob_length(struct DlistMob *p_list);

struct Mob *returnListElementMob(struct DlistMob *p_list, int position);

void writeToFileMob(struct DlistMob *p_list);

struct DlistMob *readFromFileMob();

struct Mob *Mob_ctor(int id, char *name, struct MobRace *mobRace, int hp, int attack, int relativeDefense,
                     int absoluteDefense, int dodge, struct Equipment *equipment);

struct DlistMob *createMob(struct Mob *playerMob);

struct DlistMob *getMobs(struct Mob *playerMob);


#endif
