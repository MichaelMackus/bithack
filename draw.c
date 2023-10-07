#include <string.h>
#include <stdio.h>

#include "dungeon.h"
#include "draw.h"
#include "mob.h"
#include "player.h"

unsigned char current_room_id, current_room_x, current_room_y;

void draw_status_line()
{
    char message[MAP_COLS];
    sprintf(message, "HP: %d/%d Room: %d    ", player.hp, player.max_hp, current_room_id);
    cputsxy(0, 23, message);
}

// TODO need function to draw previously seen tiles (before movement)
void draw_tiles_player_can_see()
{
    unsigned char x, y, xstart;
    unsigned char *room, *start;
    unsigned char room_col = player.x / BSP_WIDTH;
    unsigned char room_row = player.y / BSP_HEIGHT;
    unsigned char room_id = room_col + room_row*ROOMS_PER_ROW;
    unsigned short idx = xy_to_idx(player.x, player.y);

    // draw adjacent corridors TODO diagonals
    if (player.x < MAP_COLS - 1)
        add_to_draw_buffer(player.x + 1, player.y, dungeon_tiles[idx + 1], GAME_COLOR_WHITE);
    if (player.x > 0)
        add_to_draw_buffer(player.x - 1, player.y, dungeon_tiles[idx - 1], GAME_COLOR_WHITE);
    if (player.y < MAP_ROWS - 1)
        add_to_draw_buffer(player.x, player.y + 1, dungeon_tiles[idx + MAP_COLS], GAME_COLOR_WHITE);
    if (player.y > 0)
        add_to_draw_buffer(player.x, player.y - 1, dungeon_tiles[idx - MAP_COLS], GAME_COLOR_WHITE);

    if (is_room(dungeon_tiles[idx]) && room_id != current_room_id) {
        // draw lit room we're in
        current_room_id = room_id;
        current_room_x = bsp_x(player.x);
        current_room_y = bsp_y(player.y);
        room = room_start(current_room_x, current_room_y);
        start = room;

        idx = room - dungeon_tiles;
        x = idx_to_x(idx);
        y = idx_to_y(idx);
        xstart = x;
        do {
            do {
                add_to_draw_buffer(x, y, *room, GAME_COLOR_WHITE);
                ++room;
                ++idx;
                ++x;
            } while (is_room(*room));
            start += MAP_COLS;
            room = start;
            ++y;
            x = xstart;
        } while (is_room(*start));
    } else if (!is_room(dungeon_tiles[idx]) && current_room_id < BSP_SIZE) {
        // draw dark room we're no longer in
        room = room_start(current_room_x, current_room_y);
        start = room;

        idx = room - dungeon_tiles;
        x = idx_to_x(idx);
        y = idx_to_y(idx);
        xstart = x;
        do {
            do {
                add_to_draw_buffer(x, y, *room, GAME_COLOR_DARK);
                ++room;
                ++idx;
                ++x;
            } while (is_room(*room));
            start += MAP_COLS;
            room = start;
            ++y;
            x = xstart;
        } while (is_room(*start));
        current_room_id = BSP_SIZE;
    }

    // draw player
    add_to_draw_buffer(player.x, player.y, player_tile(), GAME_COLOR_WHITE);
}

void draw_initial_scene()
{
    unsigned short i;
    current_room_id = current_room_x = current_room_y = BSP_SIZE;
    wait_for_vblank();
    clear_screen();
    draw_tiles_player_can_see();
    wait_for_vblank();
    render_buffer();
    for (i=0; i<num_mobs; ++i) {
        if (mobs[i].hp > 0 && can_see(player.x, player.y, mobs[i].x, mobs[i].y)) {
            add_to_draw_buffer(mobs[i].x, mobs[i].y, mob_tile(mobs[i].type), mob_color(mobs[i].type));
        }
    }
    wait_for_vblank();
    render_buffer();
    draw_status_line();
}

