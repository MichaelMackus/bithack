#ifndef PLAYER_H
#define PLAYER_H

#define PLAYER_MAX_ITEMS 26

#define DIRECTION_UP    1
#define DIRECTION_RIGHT 2
#define DIRECTION_DOWN  3
#define DIRECTION_LEFT  4

#include "dungeon.h"
#include "item.h"

struct Player {
    unsigned char hp, max_hp;
    unsigned char exp;
    unsigned char seen[MAP_SIZE];
    unsigned char thaco;
    unsigned int x, y;
    Item *items; // max items?
};
typedef struct Player Player;

extern Player player;

// initialize player at start of game
void init_player();

// initialize player at start of dungeon level
void change_player_dlevel(unsigned short x, unsigned short y);

// get player tile symbol
unsigned char player_tile();

// bump player in a direction
void bump_player(unsigned char direction);

#endif