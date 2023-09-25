#ifndef DUNGEON_H
#define DUNGEON_H

#if defined(__NES__)
#define MAP_COLS 32
#define MAP_ROWS 25
#else
#define MAP_COLS 40
#define MAP_ROWS 21
#endif

#define MAP_SIZE MAP_COLS * MAP_ROWS
#define MAX_ROOM_WIDTH  8
#define MAX_ROOM_HEIGHT 4
#define MIN_ROOM_WIDTH  3
#define MIN_ROOM_HEIGHT 3
#define ROOMS_PER_ROW 3
#define ROOMS_PER_COL 3
#define BSP_WIDTH  MAP_COLS/ROOMS_PER_ROW
#define BSP_HEIGHT MAP_ROWS/ROOMS_PER_COL
#define BSP_PADDING 2

#define MAP_ROCK     ' '
#define MAP_WALL     '-'
#define MAP_ROOM     '.'
#define MAP_DOOR     '+'
#define MAP_CORRIDOR '#'
#define MAP_STAIR    '>'

extern unsigned char dungeon_tiles[MAP_SIZE];
extern unsigned char *dungeon_tiles_end;

// initialize our dungeon tiles array
void init_dungeon_tiles();

// returns tile at xy position
unsigned short dungeon_tile_at(unsigned char x, unsigned char y);

// XY to index
unsigned short xy_to_idx(unsigned char x, unsigned char y);

// index to X
unsigned char idx_to_x(unsigned short idx);

// index to Y
unsigned char idx_to_y(unsigned short idx);

// gets x start for current bsp column
unsigned char bsp_x(unsigned char x);

// gets y start for current bsp row
unsigned char bsp_y(unsigned char y);

// tests if a tile is considered a room tile (for FOV)
unsigned char is_room(unsigned char tile);

// get start tile for the room in the current BSP cell
// NOTE: use bsp_x() and bsp_y() function first to get the proper args
unsigned char *room_start(unsigned char x, unsigned char y);

// can mob/player at xy see mob/player at x2/y2
unsigned char can_see(unsigned char x, unsigned char y, unsigned char x2, unsigned char y2);

// checks if target tile is passable - meaning, not a wall and no
// mobs/player
unsigned char is_passable(unsigned char x, unsigned char y);

// returns the index in our graphic tileset for a given ascii character
unsigned char tileset_index(unsigned char ch);

#endif
