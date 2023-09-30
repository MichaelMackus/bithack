#include <stdlib.h>

#include "mob.h"
#include "player.h"
#include "dungeon.h"

Mob mobs[MAX_MOBS];
unsigned short num_mobs;

void init_mobs()
{
    clear_mobs();
}

void clear_mobs()
{
    unsigned short i = 0;
    for (; i<MAX_MOBS; ++i) {
        mobs[i].hp = 0;
    }
    num_mobs = 0;
}

void generate_mob(unsigned char mob_type, unsigned char x, unsigned char y)
{
    if (num_mobs == MAX_MOBS) {
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

unsigned char mob_thaco(unsigned char mob_type)
{
    switch (mob_type) {
        case MOB_GOBLIN:
        case MOB_KOBOLD:
        case MOB_ORC:
            return 19;
        case MOB_DRAGON:
            return 11;
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

void attack_player(Mob *mob)
{
    signed char target_ac = player_ac();
    unsigned char to_hit = rand() % 20 + 1;
    unsigned char damage = rand() % 6 + 1;
    if (to_hit >= mob_thaco(mob->type) - target_ac)
        hurt_player(damage);
    // TODO combat messages
}

void move_or_attack_towards(Mob *mob, unsigned char target_x, unsigned char target_y)
{
    signed char offset_x = 0;
    signed char offset_y = 0;
    unsigned char diff_x = abs((signed char) mob->x - (signed char) target_x);
    unsigned char diff_y = abs((signed char) mob->y - (signed char) target_y);
    unsigned char new_x;
    unsigned char new_y;

    if (diff_y > diff_x) {
        if (mob->y < target_y) {
            offset_y = 1;
        } else if (mob->y > target_y) {
            offset_y = -1;
        }
    } else {
        if (mob->x < target_x) {
            offset_x = 1;
        } else if (mob->x > target_x) {
            offset_x = -1;
        }
    }

    new_x = mob->x + offset_x;
    new_y = mob->y + offset_y;

    if (is_passable(new_x, new_y)) {
        mob->x = new_x;
        mob->y = new_y;

        return;
    }

    if (player.x == new_x && player.y == new_y) {
        attack_player(mob);

        return;
    }

    if (offset_x != 0) {
        if (mob->y < target_y) {
            offset_y = 1;
        } else if (mob->y > target_y) {
            offset_y = -1;
        }
        offset_x = 0;
    } else {
        if (mob->x < target_x) {
            offset_x = 1;
        } else if (mob->x > target_x) {
            offset_x = -1;
        }
        offset_y = 0;
    }

    new_x = mob->x + offset_x;
    new_y = mob->y + offset_y;

    if (is_passable(new_x, new_y)) {
        mob->x = new_x;
        mob->y = new_y;

        return;
    }

    if (player.x == new_x && player.y == new_y) {
        attack_player(mob);

        return;
    }

}

unsigned char alive_mobs()
{
    unsigned char i = 0, alive = 0;
    for (; i < num_mobs; ++i) {
        if (mobs[i].hp > 0) ++alive;
    }

    return alive;
}
