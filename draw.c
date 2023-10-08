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

void draw_room(unsigned char is_lit)
{
    unsigned char *room, *start;
    unsigned char x, y, xstart;
    unsigned short idx;
    room = room_start(current_room_x, current_room_y);
    start = room;

    idx = room - dungeon_tiles;
    x = idx_to_x(idx);
    y = idx_to_y(idx);
    xstart = x;
    do {
        do {
            add_to_draw_buffer(x, y, *room, is_lit ? GAME_COLOR_WHITE : GAME_COLOR_DARK);
            ++room;
            ++idx;
            ++x;
        } while (is_room(*room));
        start += MAP_COLS;
        room = start;
        ++y;
        x = xstart;
    } while (is_room(*start));
}

// draw tiles adjacent to position
void draw_adjacent_tiles(unsigned short idx, unsigned char x, unsigned char y, unsigned char is_lit)
{
    if (player.x < MAP_COLS - 1)
        add_to_draw_buffer(x + 1, y, dungeon_tiles[idx + 1], is_lit ? GAME_COLOR_WHITE : GAME_COLOR_DARK);
    if (x > 0)
        add_to_draw_buffer(x - 1, y, dungeon_tiles[idx - 1], is_lit ? GAME_COLOR_WHITE : GAME_COLOR_DARK);
    if (y < MAP_ROWS - 1)
        add_to_draw_buffer(x, y + 1, dungeon_tiles[idx + MAP_COLS], is_lit ? GAME_COLOR_WHITE : GAME_COLOR_DARK);
    if (y > 0)
        add_to_draw_buffer(x, y - 1, dungeon_tiles[idx - MAP_COLS], is_lit ? GAME_COLOR_WHITE : GAME_COLOR_DARK);
}

// draw corridors adjacent to position
void draw_adjacent_corridors(unsigned short idx, unsigned char x, unsigned char y, unsigned char is_lit)
{
    if (player.x < MAP_COLS - 1 && idx + 1 == MAP_CORRIDOR)
        add_to_draw_buffer(x + 1, y, dungeon_tiles[idx + 1], is_lit ? GAME_COLOR_WHITE : GAME_COLOR_DARK);
    if (x > 0 && idx - 1 == MAP_CORRIDOR)
        add_to_draw_buffer(x - 1, y, dungeon_tiles[idx - 1], is_lit ? GAME_COLOR_WHITE : GAME_COLOR_DARK);
    if (y < MAP_ROWS - 1 && idx + MAP_COLS == MAP_CORRIDOR)
        add_to_draw_buffer(x, y + 1, dungeon_tiles[idx + MAP_COLS], is_lit ? GAME_COLOR_WHITE : GAME_COLOR_DARK);
    if (y > 0 && idx - MAP_COLS == MAP_CORRIDOR)
        add_to_draw_buffer(x, y - 1, dungeon_tiles[idx - MAP_COLS], is_lit ? GAME_COLOR_WHITE : GAME_COLOR_DARK);
}

void draw_tiles_player_can_see(unsigned char prev_x, unsigned char prev_y)
{
    unsigned char room_id = player.x/BSP_WIDTH + player.y/BSP_HEIGHT*ROOMS_PER_ROW;
    unsigned short prev_idx = xy_to_idx(prev_x, prev_y);
    unsigned short idx = xy_to_idx(player.x, player.y);

    // draw the room
    if (is_room(dungeon_tiles[idx]) && room_id != current_room_id) {
        // draw lit room we're in
        current_room_id = room_id;
        current_room_x = bsp_x(player.x);
        current_room_y = bsp_y(player.y);
        draw_room(1);
    } else if (!is_room(dungeon_tiles[idx]) && current_room_id < BSP_SIZE) {
        // draw dark room we're no longer in
        draw_room(0);
        current_room_id = BSP_SIZE;
    }

    // darken tiles adjacent to player before move
    if (!is_room(dungeon_tiles[prev_idx]))
        draw_adjacent_tiles(prev_idx, prev_x, prev_y, 0);
    else
        draw_adjacent_corridors(prev_idx, prev_x, prev_y, 0);
    // lighten tiles adjacent to player after move
    draw_adjacent_tiles(idx, player.x, player.y, 1);

    // draw player
    add_to_draw_buffer(player.x, player.y, player_tile(), GAME_COLOR_WHITE);
}

void draw_initial_scene()
{
    unsigned short i;
    current_room_id = current_room_x = current_room_y = BSP_SIZE;
    wait_for_vblank();
    clear_screen();
    draw_tiles_player_can_see(player.x, player.y);
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

