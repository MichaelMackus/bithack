#ifndef PLAYER_H
#define PLAYER_H

#include "dungeon.h"
#include "item.h"

struct Player {
    unsigned char hp, max_hp;
    unsigned char exp;
    unsigned char seen[MAP_SIZE];
    unsigned int x, y;
    Item *items; // max items?
};
typedef struct Player Player;

// initialize player at start of game
void init_player();

// initialize player at start of dungeon level
void change_player_dlevel(unsigned short x, unsigned short y);

// get player tile symbol
unsigned char player_tile();

#endif
