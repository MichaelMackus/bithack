#include "dungeon_generator.h"
#include "dungeon.h"
#include "mob.h"
#include "player.h"

#include <stdlib.h>

#define MAX_ITERATIONS 20
#define MIN_MOBS 5

// NOTE: will return MAP_SIZE if reached MAX_ITERATIONS
unsigned short rand_room_idx();

// get room start
unsigned char *room_start(unsigned char x, unsigned char y)
{
    unsigned char *start_tile = dungeon_tiles + x*BSP_WIDTH + y*BSP_HEIGHT*MAP_COLS;
    unsigned char *cur_tile = start_tile;

    for (y = 0; y < BSP_HEIGHT; ++y) {
        for (x = 0; x < BSP_WIDTH; ++x) {
            if (*cur_tile == MAP_ROOM) {
                return cur_tile;
            }
            ++cur_tile;
        }
        start_tile = start_tile + MAP_COLS;
        cur_tile = start_tile;
    }

    return dungeon_tiles_end - 2; // invalid start tile
}

// get room width
unsigned char room_width(unsigned char *start_tile)
{
    unsigned char w = 0;
    while (1) {
        if (start_tile >= dungeon_tiles_end || *start_tile != MAP_ROOM) {
            break;
        }

        ++w;
        ++start_tile;
    }

    return w;
}
// get room height
unsigned char room_height(unsigned char *start_tile)
{
    unsigned char h = 0;
    while (1) {
        if (start_tile >= dungeon_tiles_end || *start_tile != MAP_ROOM) {
            break;
        }

        ++h;
        start_tile = start_tile + MAP_COLS;
    }

    return h;
}

void draw_corridor_right(unsigned char bsp_x, unsigned char bsp_y, unsigned char screen_x, unsigned char screen_y)
{
    unsigned char offset_x, offset_y, w, h;
    unsigned char *next_room_tile;

    // return if we're at right side of screen
    if (bsp_x >= ROOMS_PER_ROW - 1) {
        return;
    }

    // get next room
    next_room_tile = room_start(bsp_x + 1, bsp_y);
    offset_x = (next_room_tile - dungeon_tiles) % MAP_COLS;
    offset_y = (next_room_tile - dungeon_tiles) / MAP_COLS;
    w = room_width(next_room_tile);
    h = room_height(next_room_tile);

    draw_line(screen_x, screen_y, offset_x + (w - 1)/2, offset_y + (h - 1)/2);
}

void draw_corridor_down(unsigned char bsp_x, unsigned char bsp_y, unsigned char screen_x, unsigned char screen_y)
{
    unsigned char offset_x, offset_y, w, h;
    unsigned char *next_room_tile;

    // return if we're at right side of screen
    if (bsp_y >= ROOMS_PER_COL - 1) {
        return;
    }

    // get next room
    next_room_tile = room_start(bsp_x, bsp_y + 1);
    offset_x = (next_room_tile - dungeon_tiles) % MAP_COLS;
    offset_y = (next_room_tile - dungeon_tiles) / MAP_COLS;
    w = room_width(next_room_tile);
    h = room_height(next_room_tile);

    draw_line(screen_x, screen_y, offset_x + (w - 1)/2, offset_y + (h - 1)/2);
}

void generate_tiles()
{
    unsigned short i;
    unsigned char x, y, w, h, offset_x, offset_y;
    unsigned char *start_tile;
    unsigned char *cur_tile;
    cur_tile = dungeon_tiles;

    // clear map
    for (i = 0; i < MAP_SIZE; ++i) {
        *cur_tile = 0;
        ++cur_tile;
    }

    // generate rooms
    for (i = 0; i < ROOMS_PER_ROW*ROOMS_PER_COL; ++i) {
        // generate random room parameters
        x = i % ROOMS_PER_COL;
        y = i / ROOMS_PER_COL;
        w = rand() % BSP_WIDTH;
        h = rand() % BSP_HEIGHT;
        if (w > MAX_ROOM_WIDTH) w = MAX_ROOM_WIDTH;
        else if (w < MIN_ROOM_WIDTH) w = MIN_ROOM_WIDTH;
        if (h > MAX_ROOM_HEIGHT) h = MAX_ROOM_HEIGHT;
        else if (h < MIN_ROOM_HEIGHT) h = MIN_ROOM_HEIGHT;
        // tile offset
        start_tile = dungeon_tiles + x*BSP_WIDTH + y*BSP_HEIGHT*MAP_COLS;
        if (w < BSP_WIDTH - BSP_PADDING) {
            offset_x = rand() % (BSP_WIDTH - w - BSP_PADDING); // random X offset
            start_tile += offset_x;
        }
        if (h < BSP_HEIGHT - BSP_PADDING) {
            offset_y = rand() % (BSP_HEIGHT - h - BSP_PADDING); // random Y offset
            start_tile += offset_y*MAP_COLS;
        }
        // fill tiles
        cur_tile = start_tile;
        for (y = 0; y <= h; ++y) {
            for (x = 0; x <= w; ++x) {
                *cur_tile = MAP_ROOM;
                ++cur_tile;
            }
            start_tile = start_tile + MAP_COLS;
            cur_tile = start_tile;
        }
    }

    // connect rooms with corridors
    for (i = 0; i < ROOMS_PER_ROW*ROOMS_PER_COL; ++i) {
        // generate random room parameters
        x = i % ROOMS_PER_ROW;
        y = i / ROOMS_PER_ROW;
        // tile offset
        start_tile = room_start(x, y);
        offset_x = (start_tile - dungeon_tiles) % MAP_COLS;
        offset_y = (start_tile - dungeon_tiles) / MAP_COLS;
        w = room_width(start_tile);
        h = room_height(start_tile);
        draw_corridor_right(x, y, offset_x + (w - 1)/2, offset_y + (h - 1)/2);
        draw_corridor_down(x, y, offset_x + (w - 1)/2, offset_y + (h - 1)/2);
    }
}

unsigned char generate_mobs(unsigned char num_mobs)
{
    // generate mobs on level change
    unsigned short idx;
    unsigned char i, x, y, result;
    unsigned char count = 0;

    // generate some monsters
    // TODO difficulty
    for (i=0; i<num_mobs; ++i) {
        do {
            idx = rand_room_idx();
            if (idx >= MAP_SIZE) return i;
            x = idx_to_x(idx);
            y = idx_to_y(idx);
            ++count;
            if (count >= MAX_ITERATIONS) return i;
        } while (mob_at(x, y));
        result = rand() % 100;
        if (result < 40) {
            generate_mob(MOB_KOBOLD, x, y);
        } else if (result < 75) {
            generate_mob(MOB_GOBLIN, x, y);
        } else {
            generate_mob(MOB_ORC, x, y);
        }
    }

    return num_mobs;
}

unsigned char generate_player_xy()
{
    unsigned short idx;
    unsigned char x, y;
    unsigned char count = 0;

    do {
        idx = rand_room_idx();
        if (idx >= MAP_SIZE) return 0;
        x = idx_to_x(idx);
        y = idx_to_y(idx);
        ++count;
        if (count >= MAX_ITERATIONS) return 0;
    } while (mob_at(x, y));
    change_player_dlevel(x, y);

    return 1;
}

unsigned short rand_room_idx()
{
    unsigned short idx;
    unsigned char count = 0;

    do
    {
        idx = rand() % MAP_SIZE;
        ++count;
        if (count >= MAX_ITERATIONS) return MAP_SIZE;
    } while (dungeon_tiles[idx] != MAP_ROOM);

    return idx;
}

void generate_dlevel()
{
    unsigned char num_mobs = rand() % 11 + MIN_MOBS;
    unsigned char generation_result;

    // generate dungeon tile map
    generate_tiles();
    clear_mobs();
    generation_result = generate_mobs(num_mobs);
    /* if (generation_result < MIN_MOBS) { */
    /*     generate_dlevel(); */
    /*     return; */
    /* } */
    generation_result = generate_player_xy();
    if (!generation_result) {
        generate_dlevel();
        return;
    }
}
