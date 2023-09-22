#include "dungeon.h"

#include <stdlib.h>

unsigned char *dungeon_tiles;
unsigned char *dungeon_tiles_end;

void init_dungeon_tiles()
{
    dungeon_tiles = malloc(sizeof(*dungeon_tiles) * MAP_SIZE);
    dungeon_tiles_end = dungeon_tiles  +  sizeof(*dungeon_tiles) * MAP_SIZE;
}

// TODO pseudo-code:
//   - choose direction(s) to draw corridors (minimum 1 from each room)
//   - draw line from center of rooms
//      * start line in dir of room
//      * when halfway through line, turn line to match center of target
//      * unable to turn if it causes line to carve out wall adjacent to room(s) - in that case, just continue going towards room
//      * line must go around room wall corners
//   - mask doors - doors are spots with corridor & two adjacent walls
//   - (optionally) hide doors or corridors
void draw_line(unsigned short fromX, unsigned short fromY, unsigned short toX, unsigned short toY)
{
    unsigned short index;
    signed char offset_x, offset_y;

    index = fromX + fromY*sizeof(*dungeon_tiles)*MAP_COLS;
    if (dungeon_tiles[index] == MAP_WALL)
        dungeon_tiles[index] = MAP_CORRIDOR;

    if (fromX < toX) {
        offset_x = 1;
    } else if (fromX > toX) {
        offset_x = -1;
    } else {
        offset_x = 0;
    }

    if (fromY < toY) {
        offset_y = 1;
    } else if (fromY > toY) {
        offset_y = -1;
    } else {
        offset_y = 0;
    }

    while (fromX != toX)
    {
        fromX += offset_x;
        index = fromX + fromY*sizeof(*dungeon_tiles)*MAP_COLS;
        if (dungeon_tiles[index] == MAP_WALL)
            dungeon_tiles[index] = MAP_CORRIDOR;
    }

    while (fromY != toY)
    {
        fromY += offset_y;
        index = fromX + fromY*sizeof(*dungeon_tiles)*MAP_COLS;
        if (dungeon_tiles[index] == MAP_WALL)
            dungeon_tiles[index] = MAP_CORRIDOR;
    }
}

unsigned short dungeon_tile_at(unsigned char x, unsigned char y)
{
    unsigned short idx;

    if (x >= MAP_COLS || y >= MAP_ROWS)
        return MAP_WALL;

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
