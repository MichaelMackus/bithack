#include "dungeon.h"
#include "player.h"
#include "mob.h"

#include <stdlib.h>

unsigned char dungeon_tiles[MAP_SIZE];
unsigned char *dungeon_tiles_end;
unsigned char dungeon_depth;

void init_dungeon_tiles()
{
    unsigned short i;
    dungeon_depth = 1;
    dungeon_tiles_end = &dungeon_tiles[0]  +  sizeof(*dungeon_tiles) * MAP_SIZE;
    for (i=0; i<MAP_SIZE; ++i) {
        dungeon_tiles[i] = MAP_ROCK;
    }
}

unsigned char dungeon_tile_at(unsigned char x, unsigned char y)
{
    unsigned short idx;

    if (x >= MAP_COLS || y >= MAP_ROWS)
        return MAP_ROCK;

    idx = xy_to_idx(x, y);

    return dungeon_tiles[idx];
}

unsigned short xy_to_idx(unsigned char x, unsigned char y)
{
    return x + y*MAP_COLS;
}

unsigned char idx_to_x(unsigned short idx)
{
    return idx % MAP_COLS;
}

unsigned char idx_to_y(unsigned short idx)
{
    return idx / MAP_COLS;
}

unsigned char bsp_x(unsigned char x)
{
    return x - (x % BSP_WIDTH);
}

unsigned char bsp_y(unsigned char y)
{
    return y - (y % BSP_HEIGHT);
}

unsigned char is_room(unsigned char tile)
{
    return tile == MAP_ROOM || tile == MAP_WALL || tile == MAP_DOOR || tile == MAP_STAIR;
}

unsigned char *room_start(unsigned char x, unsigned char y)
{
    unsigned char *start_tile = dungeon_tiles + x + y*MAP_COLS;
    unsigned char *cur_tile = start_tile;

    for (y = 0; y < BSP_HEIGHT; ++y) {
        for (x = 0; x < BSP_WIDTH; ++x) {
            if (is_room(*cur_tile)) {
                return cur_tile;
            }
            ++cur_tile;
        }
        start_tile = start_tile + MAP_COLS;
        cur_tile = start_tile;
    }

    /* assert("ERROR: Unable to find start room!" == 0); */
    return dungeon_tiles_end - 2; // invalid start tile
}

void room_dimensions(const unsigned char *room_start, Rect *dimensions)
{
    unsigned char x, y, bsp_offset_x, bsp_offset_y;
    const unsigned char *cur_tile = room_start;

    if (room_start == dungeon_tiles_end - 2) {
        dimensions->x = dimensions->y = dimensions->w = dimensions->h = 0;
        return;
    }

    dimensions->x = idx_to_x(room_start - dungeon_tiles);
    dimensions->y = idx_to_y(room_start - dungeon_tiles);
    dimensions->w = 0;
    dimensions->h = 0;
    bsp_offset_x = dimensions->x - bsp_x(dimensions->x);
    bsp_offset_y = dimensions->y - bsp_y(dimensions->y);

    // find w & h
    for (x = bsp_offset_x; x<BSP_WIDTH; ++x) {
        if (!is_room(*cur_tile)) {
            break;
        }
        ++dimensions->w;
        ++cur_tile;
    }
    if (is_room(*cur_tile)) {
        // ERROR: Unable to find start room!
        dimensions->x = dimensions->y = dimensions->w = dimensions->h = 0;
        return;
    }

    cur_tile = room_start;
    for (y = bsp_offset_y; y<BSP_HEIGHT; ++y) {
        if (!is_room(*cur_tile)) {
            break;
        }
        cur_tile += MAP_COLS;
        ++dimensions->h;
    }
    if (is_room(*cur_tile)) {
        // ERROR: Unable to find start room!
        dimensions->x = dimensions->y = dimensions->w = dimensions->h = 0;
    }
}

unsigned char can_see(unsigned char x, unsigned char y, unsigned char x2, unsigned char y2)
{
    unsigned short idx = xy_to_idx(x, y);
    unsigned short idx2 = xy_to_idx(x2, y2);
    unsigned char tile = dungeon_tiles[idx];
    unsigned char tile2 = dungeon_tiles[idx2];
    unsigned char bsp_col, bsp_row, bsp2_col, bsp2_row, xdiff, ydiff;

    // always can see if we're next to square
    xdiff = abs((signed char) x - (signed char) x2);
    ydiff = abs((signed char) y - (signed char) y2);
    if (xdiff <= 1 && ydiff <= 1) {
        return 1;
    }

    if (!is_room(tile) || !is_room(tile2)) {
        return 0;
    }

    // does a simple test to detect if both X/Y regions are in the same
    // BSP section on the screen (the screen is divided into a 3x3 grid)
    bsp_col = bsp_x(x);
    bsp_row = bsp_y(y);
    bsp2_col = bsp_x(x2);
    bsp2_row = bsp_y(y2);

    return bsp_col == bsp2_col && bsp_row == bsp2_row;
}

unsigned char is_passable(unsigned char x, unsigned char y)
{
    Mob *mob = 0;
    unsigned short idx = xy_to_idx(x, y);

    if (idx > MAP_SIZE)
        return 0;

    mob = mob_at(x, y);
    return (dungeon_tiles[idx] == MAP_ROOM || dungeon_tiles[idx] == MAP_STAIR || dungeon_tiles[idx] == MAP_DOOR || dungeon_tiles[idx] == MAP_CORRIDOR)
            && !mob && !(player.x == x && player.y == y);
}

unsigned char tileset_index(unsigned char ch)
{
    switch (ch) {
        case MAP_WALL:
            return 0x62;
        case MAP_ROOM:
        case MAP_CORRIDOR:
            return 0x61;
        case MAP_STAIR:
            return 0x66;
        case MAP_DOOR:
            return 0x72;
        // player ASCII character
        case '@':
            return 0x64;
        // enemy ASCII characters
        case 'g':
            return 0x63;
        case 'o':
            return 0x63;
        case 'k':
            return 0x63;
    }

    return ch;
}
