#include <stdio.h>
#include <stdlib.h>

#define LIST_TYPE Items

#include "Player.h"


void eqpStuffItem(struct StuffItem *item, struct Mob *mob);

void unEqpStuffItem(struct StuffItem *item, struct Mob *mob);

void replaceItemEquip(struct Player *player, int itemPosition);

void sellStuffItem(struct StuffItem *item, struct Player *player);

/* Constructor for a StuffItem structure */
struct StuffItem *StuffItem_ctor(char *name, int goldValue, int typeId, int hp, int attack, int relativeDefense,
                                 int absoluteDefense) {
    struct StuffItem *p = malloc(sizeof(struct StuffItem));
    p->name = name;
    p->goldValue = goldValue;
    p->typeId = typeId;
    p->hp = hp;
    p->attack = attack;
    p->relativeDefense = relativeDefense;
    p->absoluteDefense = absoluteDefense;
    return p;
}

/* Creates a new list of items */
struct DlistItems *dlistItems_new(void) {
    struct DlistItems *p_new = malloc(sizeof *p_new);
    if (p_new != NULL) {
        p_new->length = 0;
        p_new->p_head = NULL;
        p_new->p_tail = NULL;
    }
    return p_new;
}

/* Adds element at the end of the list */
struct DlistItems *dlistItems_append(struct DlistItems *p_list, struct StuffItem stuffItem) {
    if (p_list != NULL) {
        struct nodeItems *p_new = malloc(sizeof *p_new);
        if (p_new != NULL) {
            p_new->stuffItem = stuffItem;
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

/* Removes an item from a list */
struct DlistItems *dlistItems_remove_id(struct DlistItems *p_list, int position) {
    if (p_list != NULL) {
        struct nodeItems *p_temp = p_list->p_head;
        int i = 1;
        while (p_temp != NULL && i <= position) {
            if (position == i) {
                if (p_temp->p_next == NULL) {
                    p_list->p_tail = p_temp->p_prev;
                    p_list->p_tail->p_next = NULL;
                } else if (p_temp->p_prev == NULL) {
                    p_list->p_head = p_temp->p_next;
                    p_list->p_head->p_prev = NULL;
                } else {
                    p_temp->p_next->p_prev = p_temp->p_prev;
                    p_temp->p_prev->p_next = p_temp->p_next;
                }
                free(p_temp);
                p_list->length--;
            } else {
                p_temp = p_temp->p_next;
            }
            i++;
        }
    }
    return p_list;
}

/* Removes an item from a list */
struct DlistItems *dlistItems_remove(struct DlistItems *p_list, struct StuffItem stuffItem) {
    if (p_list != NULL) {
        struct nodeItems *p_temp = p_list->p_head;
        int found = 0;
        while (p_temp != NULL && !found) {
            if (&p_temp->stuffItem == &stuffItem) {
                if (p_temp->p_next == NULL) {
                    p_list->p_tail = p_temp->p_prev;
                    p_list->p_tail->p_next = NULL;
                } else if (p_temp->p_prev == NULL) {
                    p_list->p_head = p_temp->p_next;
                    p_list->p_head->p_prev = NULL;
                } else {
                    p_temp->p_next->p_prev = p_temp->p_prev;
                    p_temp->p_prev->p_next = p_temp->p_next;
                }
                free(p_temp);
                p_list->length--;
                found = 1;
            } else {
                p_temp = p_temp->p_next;
            }
        }
    }
    return p_list;
}

/* Returns the length of an item list */
size_t dlistItems_length(struct DlistItems *p_list) {
    size_t ret = 0;
    if (p_list != NULL) {
        ret = p_list->length;
    }
    return ret;
}

/* Returns a StuffItem from a list */
struct StuffItem *returnListElementItem(struct DlistItems *p_list, int position) {
    int i = 0;
    struct nodeItems *p_temp = p_list->p_head;
    for (i = 0; i < position - 1; i++) {
        p_temp = p_temp->p_next;
    }
    struct StuffItem item_temp = p_temp->stuffItem;

    struct StuffItem *p_item = StuffItem_ctor(item_temp.name, item_temp.goldValue, item_temp.typeId, item_temp.hp,
                                              item_temp.attack,
                                              item_temp.relativeDefense, item_temp.absoluteDefense);
    return p_item;
}

/* Writes the whole list in a file */
void writeToFileItems(struct DlistItems *p_list) {
    FILE *fptr;
    fptr = fopen("./itemslist.txt", "w+");

    struct StuffItem *stuffItem = malloc(sizeof(struct StuffItem));
    if (p_list != NULL) {
        struct nodeItems *p_temp = p_list->p_head;
        while (p_temp != NULL) {
            stuffItem = &p_temp->stuffItem;
            fwrite(stuffItem, sizeof(struct StuffItem), 1, fptr);
            p_temp = p_temp->p_next;
        }
    }
    fclose(fptr);
}

/* Returns a list of items from a text file */
struct DlistItems *readFromFileItems() {
    struct DlistItems *p_list = dlistItems_new();
    struct StuffItem *stuffItem = malloc(sizeof(struct StuffItem));
    FILE *fptr;

    fptr = fopen("./itemslist.txt", "r");

    if (fptr) {
        while (fread(stuffItem, sizeof(struct StuffItem), 1, fptr) == 1) {
            dlistItems_append(p_list,
                              *StuffItem_ctor(stuffItem->name, stuffItem->goldValue, stuffItem->typeId, stuffItem->hp,
                                              stuffItem->attack, stuffItem->relativeDefense,
                                              stuffItem->absoluteDefense));
        }
    }

    fclose(fptr);
    return p_list;
}

/* Generates the list of items */
struct DlistItems *createItemsList() {
    struct DlistItems *itemsList = dlistItems_new();

    struct StuffItem *nohelmet = StuffItem_ctor("Empty", 0, 0, 0, 0, 0, 0);
    dlistItems_append(itemsList, *nohelmet);
    struct StuffItem *nochest = StuffItem_ctor("Empty", 0, 1, 0, 0, 0, 0);
    dlistItems_append(itemsList, *nochest);
    struct StuffItem *nolegs = StuffItem_ctor("Empty", 0, 2, 0, 0, 0, 0);
    dlistItems_append(itemsList, *nolegs);
    struct StuffItem *noboots = StuffItem_ctor("Empty", 0, 3, 0, 0, 0, 0);
    dlistItems_append(itemsList, *noboots);
    struct StuffItem *nolefthand = StuffItem_ctor("Empty", 0, 4, 0, 0, 0, 0);
    dlistItems_append(itemsList, *nolefthand);
    struct StuffItem *norighthand = StuffItem_ctor("Empty", 0, 5, 0, 0, 0, 0);
    dlistItems_append(itemsList, *norighthand);


    struct StuffItem *head1 = StuffItem_ctor("Old leather helmet", 150, 0, 12, 0, 7, 2);
    dlistItems_append(itemsList, *head1);
    struct StuffItem *head2 = StuffItem_ctor("Reinforced Leather Helmet", 150, 0, 12, 0, 7, 2);
    dlistItems_append(itemsList, *head2);
    struct StuffItem *head3 = StuffItem_ctor("Massive Plate Greathelm", 150, 0, 12, 0, 7, 2);
    dlistItems_append(itemsList, *head3);
    struct StuffItem *head4 = StuffItem_ctor("Bronze Visage of Fallen Sorrow", 150, 0, 12, 0, 7, 2);
    dlistItems_append(itemsList, *head4);
    struct StuffItem *head5 = StuffItem_ctor("Wrathful Crown of the South", 150, 0, 12, 0, 7, 2);
    dlistItems_append(itemsList, *head5);

    struct StuffItem *chest1 = StuffItem_ctor("Hellish Plate Mail", 230, 1, 18, 1, 12, 3);
    dlistItems_append(itemsList, *chest1);
    struct StuffItem *chest2 = StuffItem_ctor("War-Forged Breastplate", 230, 1, 18, 1, 12, 3);
    dlistItems_append(itemsList, *chest2);
    struct StuffItem *chest3 = StuffItem_ctor("Recruit's Chainmail", 230, 1, 18, 1, 12, 3);
    dlistItems_append(itemsList, *chest3);
    struct StuffItem *chest4 = StuffItem_ctor("Steel Cuirass of Illusion", 230, 1, 18, 1, 12, 3);
    dlistItems_append(itemsList, *chest4);
    struct StuffItem *chest5 = StuffItem_ctor("Mercenary Ebon Chestplate", 230, 1, 18, 1, 12, 3);
    dlistItems_append(itemsList, *chest5);


    struct StuffItem *leg1 = StuffItem_ctor("Kilt of the Sacred Warlords", 200, 2, 20, 0, 9, 2);
    dlistItems_append(itemsList, *leg1);
    struct StuffItem *leg2 = StuffItem_ctor("Legguards of the Ancient Kings", 200, 2, 20, 0, 9, 2);
    dlistItems_append(itemsList, *leg2);
    struct StuffItem *leg3 = StuffItem_ctor("Golden Greaves", 200, 2, 20, 0, 9, 2);
    dlistItems_append(itemsList, *leg3);
    struct StuffItem *leg4 = StuffItem_ctor("Loyal Obsidian Leggings", 200, 2, 20, 0, 9, 2);
    dlistItems_append(itemsList, *leg4);
    struct StuffItem *leg5 = StuffItem_ctor("Platelegs of ending Fire", 200, 2, 20, 0, 9, 2);
    dlistItems_append(itemsList, *leg5);


    struct StuffItem *boots1 = StuffItem_ctor("Champion's Warboots", 80, 3, 12, 0, 5, 1);
    dlistItems_append(itemsList, *boots1);
    struct StuffItem *boots2 = StuffItem_ctor("Spurs of the Corrupted", 80, 3, 12, 0, 5, 1);
    dlistItems_append(itemsList, *boots2);
    struct StuffItem *boots3 = StuffItem_ctor("Hatred Bronze Boots", 80, 3, 12, 0, 5, 1);
    dlistItems_append(itemsList, *boots3);
    struct StuffItem *boots4 = StuffItem_ctor("Brutal Skeleton Sabatons", 80, 3, 12, 0, 5, 1);
    dlistItems_append(itemsList, *boots4);
    struct StuffItem *boots5 = StuffItem_ctor("Holy Walkers of Past Dreams", 80, 3, 12, 0, 5, 1);
    dlistItems_append(itemsList, *boots5);


    struct StuffItem *shield1 = StuffItem_ctor("Sentinel Keeper", 180, 4, 0, 0, 15, 5);
    dlistItems_append(itemsList, *shield1);
    struct StuffItem *shield2 = StuffItem_ctor("Bandit's Ivory Guardian", 180, 4, 0, 0, 15, 5);
    dlistItems_append(itemsList, *shield2);
    struct StuffItem *shield3 = StuffItem_ctor("Banished Light Shield", 180, 4, 0, 0, 15, 5);
    dlistItems_append(itemsList, *shield3);
    struct StuffItem *shield4 = StuffItem_ctor("Ancient Tome of the Illusionist", 180, 4, 0, 0, 15, 5);
    dlistItems_append(itemsList, *shield4);
    struct StuffItem *shield5 = StuffItem_ctor("Skull of the Demon", 180, 4, 0, 0, 15, 5);
    dlistItems_append(itemsList, *shield5);


    struct StuffItem *weapon1 = StuffItem_ctor("Diamond Sword", 350, 5, 0, 8, 0, 0);
    dlistItems_append(itemsList, *weapon1);
    struct StuffItem *weapon2 = StuffItem_ctor("Staff of the Magician", 350, 5, 0, 8, 0, 0);
    dlistItems_append(itemsList, *weapon2);
    struct StuffItem *weapon3 = StuffItem_ctor("Double-Edged Axe", 350, 5, 0, 8, 0, 0);
    dlistItems_append(itemsList, *weapon3);
    struct StuffItem *weapon4 = StuffItem_ctor("Halberd of the Titans", 350, 5, 0, 8, 0, 0);
    dlistItems_append(itemsList, *weapon4);
    struct StuffItem *weapon5 = StuffItem_ctor("Thunderstorm Mallet", 350, 5, 0, 8, 0, 0);
    dlistItems_append(itemsList, *weapon5);
    struct StuffItem *weapon6 = StuffItem_ctor("Banished Staff of the Warden", 350, 5, 0, 8, 0, 0);
    dlistItems_append(itemsList, *weapon6);
    struct StuffItem *weapon7 = StuffItem_ctor("Blazewing, Soul of the Depth", 350, 5, 0, 8, 0, 0);
    dlistItems_append(itemsList, *weapon7);

    writeToFileItems(itemsList);
    return itemsList;
}

/* Returns a list of items either thtough the text file
 * or by generating it */
struct DlistItems *getItems() {
    struct DlistItems *itemsList = dlistItems_new();
    if (itemsList == readFromFileItems()) {
        return itemsList;
    } else {
        return createItemsList();
    }
}

/* Where the player chooses his first items
 * at the start of a game */
struct DlistItems *createFirstInventory() {
    struct DlistItems *firstInventory = dlistItems_new();


    return firstInventory;
}

/* Equips an item for a mob */
void eqpStuffItem(struct StuffItem *item, struct Mob *mob) {
    switch (item->typeId) {
        case 0:
            mob->equipment->head = item;
            break;
        case 1:
            mob->equipment->chest = item;
            break;
        case 2:
            mob->equipment->leggings = item;
            break;
        case 3:
            mob->equipment->boots = item;
            break;
        case 4:
            mob->equipment->leftHand = item;
            break;
        case 5:
            mob->equipment->rightHand = item;
            break;
    }
}

/* unequips an item for a mob */
void unEqpStuffItem(struct StuffItem *item, struct Mob *mob) {
    switch (item->typeId) {
        case 0:
            mob->equipment->head = returnListElementItem(getItems(), 0);
            break;
        case 1:
            mob->equipment->chest = returnListElementItem(getItems(), 1);
            break;
        case 2:
            mob->equipment->leggings = returnListElementItem(getItems(), 2);
            break;
        case 3:
            mob->equipment->boots = returnListElementItem(getItems(), 3);
            break;
        case 4:
            mob->equipment->leftHand = returnListElementItem(getItems(), 4);
            break;
        case 5:
            mob->equipment->rightHand = returnListElementItem(getItems(), 5);
            break;
    }
}

/* Equips an item from his position in the player's inventory
 * and unequips the previous one */
void replaceItemEquip(struct Player *player, int itemPosition) {
    struct StuffItem *itemToEquip = returnListElementItem(player->playerInventory, itemPosition);
    struct StuffItem *currentItem;

    switch (itemToEquip->typeId) {
        case 0:
            currentItem = player->mob->equipment->head;
            break;
        case 1:
            currentItem = player->mob->equipment->chest;
            break;
        case 2:
            currentItem = player->mob->equipment->leggings;
            break;
        case 3:
            currentItem = player->mob->equipment->boots;
            break;
        case 4:
            currentItem = player->mob->equipment->leftHand;
            break;
        case 5:
            currentItem = player->mob->equipment->rightHand;
            break;
    }
    printf("You have equipped %s. ", itemToEquip->name);
    if (currentItem->name != "Empty") {
        printf("%s has been placed in your inventory.", currentItem->name);
        dlistItems_append(player->playerInventory, *currentItem);
    }
    printf("\n");
    eqpStuffItem(itemToEquip, player->mob);
    if (dlistItems_length(player->playerInventory) == 1) {
        player->playerInventory = dlistItems_new();
    } else {
        dlistItems_remove_id(player->playerInventory, itemPosition);
    }
}

/* Sells an item from the player's inventory */
void sellStuffItem(struct StuffItem *item, struct Player *player) {
    int goldGained = item->goldValue * 0.7;
    player->gold += goldGained;
}
