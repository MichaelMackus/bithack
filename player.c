#include "player.h"

Player player;

void init_player()
{
    player.hp = 8;
    player.max_hp = 8;
}

void change_player_dlevel(unsigned int x, unsigned int y)
{
    player.x = x;
    player.y = y;
}
