#ifndef ITEM_H
#define ITEM_H

#define MAX_ITEMS MAP_SIZE

enum ItemType {
    ITEM_SCROLL = 1,
    ITEM_POTION = 2,
    ITEM_WEAPON = 4,
    ITEM_WAND = 8,
    ITEM_ARMOR = 16,
    
};
typedef enum ItemType ItemType;

struct Item {
    enum ItemType type;
    // TODO item qualities union
};
typedef struct Item Item;

extern Item screen_items[MAX_ITEMS];

#endif
