#include "player.h"

Player player;

void init_player()
{
    player.hp = 8;
    player.max_hp = 8;
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
