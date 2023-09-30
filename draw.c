#include <string.h>
#include <stdio.h>

#include "dungeon.h"
#include "draw.h"
#include "mob.h"
#include "player.h"

void add_to_draw_buffer(unsigned char x, unsigned char y, unsigned char ch)
{
    unsigned short idx;

    idx = xy_to_idx(x, y);
    add_to_draw_buffer_idx(idx, ch);
}

void draw_status_line()
{
    char message[MAP_COLS];
    sprintf(message, "HP: %d/%d", player.hp, player.max_hp);
    cputsxy(0, 23, message);
}

// TODO don't draw duplicate tiles for performance
void draw_tiles_player_can_see()
{
    unsigned char x, y;
    unsigned char *room, *start;
    unsigned short idx_start;
    unsigned short idx = xy_to_idx(player.x, player.y);

    // draw adjacent corridors TODO diagonals
    if (player.x < MAP_COLS - 1)
        add_to_draw_buffer(player.x + 1, player.y, dungeon_tiles[idx + 1]);
    if (player.x > 0)
        add_to_draw_buffer(player.x - 1, player.y, dungeon_tiles[idx - 1]);
    if (player.y < MAP_ROWS - 1)
        add_to_draw_buffer(player.x, player.y + 1, dungeon_tiles[idx + MAP_COLS]);
    if (player.y > 0)
        add_to_draw_buffer(player.x, player.y - 1, dungeon_tiles[idx - MAP_COLS]);

    if (is_room(dungeon_tiles[idx])) {
        // draw lit room we're in
        x = bsp_x(player.x);
        y = bsp_y(player.y);
        room = room_start(x, y);
        start = room;
        
        idx = room - dungeon_tiles;
        idx_start = idx;
        do {
            do {
                add_to_draw_buffer_idx(idx, *room);
                ++room;
                ++idx;
            } while (is_room(*room));
            start += MAP_COLS;
            room = start;
            idx_start += MAP_COLS;
            idx = idx_start;
        } while (is_room(*start));
    }

    // draw player
    add_to_draw_buffer(player.x, player.y, player_tile());
}

void draw_initial_scene()
{
    unsigned short i;
    wait_for_vblank();
    clear_screen();
    draw_tiles_player_can_see();
    add_to_draw_buffer(player.x, player.y, player_tile());
    wait_for_vblank();
    render_buffer();
    for (i=0; i<num_mobs; ++i) {
        if (mobs[i].hp > 0 && can_see(player.x, player.y, mobs[i].x, mobs[i].y)) {
            add_to_draw_buffer(mobs[i].x, mobs[i].y, mob_tile(mobs[i].type));
        }
    }
    wait_for_vblank();
    render_buffer();
    draw_status_line();
}

