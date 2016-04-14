#include <stdio.h>
#include <stdlib.h>
#include "Equipement.h"

/* Constructor for the Equipment structure */
struct Equipment *Equipment_ctor(struct StuffItem *head, struct StuffItem *chest, struct StuffItem *leggings,
                                 struct StuffItem *boots, struct StuffItem *leftHand, struct StuffItem *rightHand) {
    struct Equipment *p = malloc(sizeof(struct Equipment));
    p->head = head;
    p->chest = chest;
    p->leggings = leggings;
    p->boots = boots;
    p->leftHand = leftHand;
    p->rightHand = rightHand;
    return p;
}
