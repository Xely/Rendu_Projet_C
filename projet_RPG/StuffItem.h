#ifndef STUFFITEM_H_INCLUDED
#define STUFFITEM_H_INCLUDED


struct StuffItem {
    char *name;
    int goldValue;
    /* typeId : 0 = head, 1 = chest, 2 = leggings, 3 = boots, 4 = lefthand, 5 = righthand */
    int typeId;
    int hp;
    int attack;
    int relativeDefense;
    int absoluteDefense;
};

struct nodeItems {
    struct StuffItem stuffItem;
    struct nodeItems *p_next;
    struct nodeItems *p_prev;
};

struct DlistItems {
    size_t length;
    struct nodeItems *p_tail;
    struct nodeItems *p_head;
};


struct StuffItem *StuffItem_ctor(char *name, int goldValue, int typeId, int hp, int attack, int relativeDefense,
                                 int absoluteDefense);

struct DlistItems *dlistItems_new(void);

struct DlistItems *dlistItems_append(struct DlistItems *p_list, struct StuffItem stuffItem);

struct DlistItems *dlistItems_remove_id(struct DlistItems *p_list, int position);

struct DlistItems *dlistItems_remove(struct DlistItems *p_list, struct StuffItem stuffItem);

size_t dlistItems_length(struct DlistItems *p_list);

struct StuffItem *returnListElementItem(struct DlistItems *p_list, int position);

void writeToFileItems(struct DlistItems *p_list);

struct DlistItems *readFromFileItems();

struct DlistItems *createItemsList();

struct DlistItems *getItems();

struct DlistItems *createFirstInventory();


#endif
