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

#define MAP_WALL     0
#define MAP_ROOM     1
#define MAP_DOOR     2
#define MAP_CORRIDOR 3

unsigned char *init_dungeon_tiles();
void draw_line(unsigned short fromX, unsigned short fromY, unsigned short toX, unsigned short toY);

#endif
