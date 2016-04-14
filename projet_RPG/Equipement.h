#ifndef EQUIPEMENT_H_INCLUDED
#define EQUIPEMENT_H_INCLUDED

#include "StuffItem.h"


struct Equipment {
    struct StuffItem *head;
    struct StuffItem *chest;
    struct StuffItem *leggings;
    struct StuffItem *boots;
    struct StuffItem *leftHand;
    struct StuffItem *rightHand;
};

struct Equipment *Equipment_ctor(struct StuffItem *head, struct StuffItem *chest, struct StuffItem *leggings,
                                 struct StuffItem *boots, struct StuffItem *leftHand, struct StuffItem *rightHand);

#endif
