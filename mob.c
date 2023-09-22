#include <stdlib.h>
#include <string.h>

#include "mob.h"
#include "dungeon.h"

Mob mobs[MAX_MOBS];
unsigned short num_mobs;

void init_mobs()
{
    clear_mobs();
}

void clear_mobs()
{
    num_mobs = 0;
}

void cleanup_mobs()
{
    unsigned short killed = 0;
    unsigned short i = 0;
    Mob *cur_mobs = mobs;
    for (; i<num_mobs; ++i) {
        if (cur_mobs->hp == 0) {
            memcpy(cur_mobs, cur_mobs + 1, sizeof(mobs) - (MAX_MOBS - i - 1) * sizeof(*mobs));
            ++killed;
        }
        ++cur_mobs;
    }

    num_mobs -= killed;
}

void generate_mob(unsigned char mob_type, unsigned char x, unsigned char y)
{
    if (num_mobs == MAP_SIZE) {
        // TODO error - too many mobs
        return;
    }

    switch (mob_type) {
        case MOB_GOBLIN:
            mobs[num_mobs].hp = mobs[num_mobs].max_hp = 3;
            break;
        case MOB_KOBOLD:
            mobs[num_mobs].hp = mobs[num_mobs].max_hp = 2;
            break;
        case MOB_ORC:
            mobs[num_mobs].hp = mobs[num_mobs].max_hp = 4;
            break;
        case MOB_DRAGON:
            mobs[num_mobs].hp = mobs[num_mobs].max_hp = 45;
            break;
        default:
            // invalid mob
            // TODO error
            return;
    }

    mobs[num_mobs].type = mob_type;
    mobs[num_mobs].x = x;
    mobs[num_mobs].y = y;
    ++num_mobs;
}

unsigned char mob_tile(unsigned char mob_type)
{
    switch (mob_type) {
        case MOB_GOBLIN:
            return 'g';
        case MOB_KOBOLD:
            return 'k';
        case MOB_ORC:
            return 'o';
        case MOB_DRAGON:
            return 'd';
        default:
            // invalid mob
            // TODO error
            return '\0';
    }
}

Mob *mob_at(unsigned char x, unsigned char y)
{
    unsigned short i;
    for (i=0; i<num_mobs; ++i) {
        if (mobs[i].x == x && mobs[i].y == y && mobs[i].hp > 0) {
            return &(mobs[i]);
        }
    }

    return 0;
}

signed char mob_ac(unsigned char mob_type)
{
    switch (mob_type) {
        case MOB_GOBLIN:
            return 6;
        case MOB_KOBOLD:
            return 7;
        case MOB_ORC:
            return 6;
        case MOB_DRAGON:
            return -1;
        default:
            // invalid mob
            // TODO error
            return 0;
    }
}

void hurt(Mob *target, unsigned char damage)
{
    if (target->hp > damage)
        target->hp -= damage;
    else
        target->hp = 0;
}
