#include <stdlib.h>

#include "player.h"
#include "mob.h"

Player player;

void init_player()
{
    unsigned short i;
    player.hp = 8;
    player.max_hp = 8;
    player.thaco = 19;
    player.exp = 0;
    player.items = malloc(PLAYER_MAX_ITEMS);
    for (i = 0; i < MAP_SIZE; ++i) {
        player.seen[i] = 0;
    }
}

void change_player_dlevel(unsigned short x, unsigned short y)
{
    player.x = x;
    player.y = y;
}

unsigned char player_tile()
{
    return 0; // @ on C64
}

void player_attack(Mob *target)
{
    signed char target_ac = mob_ac(target->type);
    unsigned char to_hit = rand() % 20 + 1;
    unsigned char damage = rand() % 6 + 1;
    if (to_hit >= player.thaco - target_ac)
        hurt(target, damage);
    // TODO miss message
}

void bump_player(unsigned char direction)
{
    Mob *target;
    unsigned char tile;
    unsigned char new_x = player.x;
    unsigned char new_y = player.y;

    switch (direction) {
        case DIRECTION_UP:
            if (player.y > 0)
                new_y -= 1;
            break;
        case DIRECTION_DOWN:
            if (player.y < MAP_ROWS - 1)
                new_y += 1;
            break;
        case DIRECTION_RIGHT:
            if (player.x < MAP_COLS - 1)
                new_x += 1;
            break;
        case DIRECTION_LEFT:
            if (player.x > 0)
                new_x -= 1;
            break;
    }

    tile = dungeon_tile_at(new_x, new_y);
    if (tile == MAP_ROCK || tile == MAP_WALL) {
        return;
    }

    target = mob_at(new_x, new_y);
    if (target) {
        player_attack(target);
    } else {
        player.x = new_x;
        player.y = new_y;
    }
}
