#ifndef DUNGEON_H
#define DUNGEON_H

#define MAP_COLS 40
#define MAP_ROWS 21
#define MAP_SIZE MAP_COLS * MAP_ROWS
#define MAX_ROOM_WIDTH  8
#define MAX_ROOM_HEIGHT 4
#define MIN_ROOM_WIDTH  3
#define MIN_ROOM_HEIGHT 2
#define ROOMS_PER_ROW 3
#define ROOMS_PER_COL 3
#define BSP_WIDTH  MAP_COLS/ROOMS_PER_ROW
#define BSP_HEIGHT MAP_ROWS/ROOMS_PER_COL
#define BSP_PADDING 2

#define MAP_WALL     0x20
#define MAP_ROOM     0x2E
#define MAP_DOOR     0x2B
#define MAP_CORRIDOR 0x23

extern unsigned char dungeon_tiles[MAP_SIZE];
extern unsigned char *dungeon_tiles_end;

// initialize our dungeon tiles array
void init_dungeon_tiles();

// draw line (e.g. to connect dungeon rooms)
void draw_line(unsigned short fromX, unsigned short fromY, unsigned short toX, unsigned short toY);

// returns tile at xy position
unsigned short dungeon_tile_at(unsigned char x, unsigned char y);

// XY to index
unsigned short xy_to_idx(unsigned char x, unsigned char y);

// index to X
unsigned char idx_to_x(unsigned short idx);

// index to Y
unsigned char idx_to_y(unsigned short idx);

#endif