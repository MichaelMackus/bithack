#include <stdlib.h>
#include <time.h>
#include "ctest/ctest.h"
#include "../dungeon.h"
#include "../dungeon_generator.h"

CTEST_DATA(map) {};
CTEST_SETUP(map) {
    init_dungeon_tiles();
}

CTEST2(map, testThingsAreInitialized) {
    for (unsigned int idx=0; idx<MAP_SIZE; ++idx) {
        ASSERT_EQUAL(dungeon_tiles[idx], MAP_ROCK);
    }
}

CTEST2(map, testIdxConvertedToXY) {
    unsigned char x, y;
    unsigned short idx;

    srand(1000);
    generate_dlevel();
    for (idx=0; idx<MAP_SIZE; ++idx) {
        x = idx_to_x(idx);
        y = idx_to_y(idx);
        ASSERT_EQUAL(dungeon_tiles[idx], dungeon_tiles[x + y*MAP_COLS]);
    }
}

CTEST2(map, testXYConvertedToIdx) {
    unsigned char x, y;
    unsigned short idx;

    srand(1000);
    generate_dlevel();
    for (x=0; x<MAP_COLS; ++x) {
        for (y=0; y<MAP_COLS; ++y) {
            idx = xy_to_idx(x, y);
            ASSERT_EQUAL(dungeon_tiles[idx], dungeon_tiles[x + y*MAP_COLS]);
        }
    }
}

CTEST2(map, testBSPXYWithinBSP) {
    unsigned char x, y;
    unsigned short idx;

    for (y=0; y<MAP_ROWS; ++y) {
        for (x=0; x<MAP_COLS; ++x) {
            ASSERT_EQUAL(bsp_x(x) % ((unsigned char) BSP_WIDTH), 0);
        }
        ASSERT_EQUAL(bsp_y(y) % ((unsigned char) BSP_HEIGHT), 0);
    }
}

CTEST2(map, testFindRoomAtStartWithDimensions) {
    dungeon_tiles[0] = MAP_WALL;
    dungeon_tiles[1] = MAP_WALL;
    dungeon_tiles[2] = MAP_WALL;
    dungeon_tiles[3] = MAP_WALL;
    dungeon_tiles[0 + MAP_COLS] = MAP_WALL;
    dungeon_tiles[1 + MAP_COLS] = MAP_ROOM;
    dungeon_tiles[2 + MAP_COLS] = MAP_ROOM;
    dungeon_tiles[3 + MAP_COLS] = MAP_ROOM;
    dungeon_tiles[0 + MAP_COLS*2] = MAP_WALL;
    dungeon_tiles[1 + MAP_COLS*2] = MAP_ROOM;
    dungeon_tiles[2 + MAP_COLS*2] = MAP_ROOM;
    dungeon_tiles[3 + MAP_COLS*2] = MAP_ROOM;
    dungeon_tiles[0 + MAP_COLS*3] = MAP_WALL;
    dungeon_tiles[1 + MAP_COLS*3] = MAP_WALL;
    dungeon_tiles[2 + MAP_COLS*3] = MAP_WALL;
    dungeon_tiles[3 + MAP_COLS*3] = MAP_WALL;

    Rect dimensions;
    unsigned char *room = room_start(0,0);
    room_dimensions(room,&dimensions);
    ASSERT_EQUAL((int) room, (int) dungeon_tiles);
    ASSERT_EQUAL(dimensions.x, 0);
    ASSERT_EQUAL(dimensions.y, 0);
    ASSERT_EQUAL(dimensions.w, 4);
    ASSERT_EQUAL(dimensions.h, 4);
}

CTEST2(map, testDoesntFindRoom) {
    dungeon_tiles[BSP_WIDTH + 1] = MAP_WALL;
    dungeon_tiles[BSP_WIDTH + 2] = MAP_WALL;
    dungeon_tiles[BSP_WIDTH + 3] = MAP_WALL;
    dungeon_tiles[BSP_WIDTH + 4] = MAP_WALL;
    dungeon_tiles[BSP_WIDTH + 5] = MAP_WALL;
    dungeon_tiles[BSP_WIDTH + 1 + MAP_COLS] = MAP_WALL;
    dungeon_tiles[BSP_WIDTH + 2 + MAP_COLS] = MAP_ROOM;
    dungeon_tiles[BSP_WIDTH + 4 + MAP_COLS] = MAP_ROOM;
    dungeon_tiles[BSP_WIDTH + 4 + MAP_COLS] = MAP_ROOM;
    dungeon_tiles[BSP_WIDTH + 5 + MAP_COLS] = MAP_WALL;
    dungeon_tiles[BSP_WIDTH + 1 + MAP_COLS*2] = MAP_WALL;
    dungeon_tiles[BSP_WIDTH + 2 + MAP_COLS*2] = MAP_ROOM;
    dungeon_tiles[BSP_WIDTH + 3 + MAP_COLS*2] = MAP_ROOM;
    dungeon_tiles[BSP_WIDTH + 4 + MAP_COLS*2] = MAP_ROOM;
    dungeon_tiles[BSP_WIDTH + 5 + MAP_COLS*2] = MAP_WALL;
    dungeon_tiles[BSP_WIDTH + 1 + MAP_COLS*3] = MAP_WALL;
    dungeon_tiles[BSP_WIDTH + 2 + MAP_COLS*3] = MAP_WALL;
    dungeon_tiles[BSP_WIDTH + 3 + MAP_COLS*3] = MAP_WALL;
    dungeon_tiles[BSP_WIDTH + 4 + MAP_COLS*3] = MAP_WALL;
    dungeon_tiles[BSP_WIDTH + 5 + MAP_COLS*3] = MAP_WALL;

    unsigned char *room = room_start(0,0);
    ASSERT_EQUAL((int) room, (int) dungeon_tiles_end - 2);
}

CTEST2(map, testFindRoomStartWithDimensionsWithOffset) {
    dungeon_tiles[1 + MAP_COLS] = MAP_WALL;
    dungeon_tiles[2 + MAP_COLS] = MAP_WALL;
    dungeon_tiles[3 + MAP_COLS] = MAP_WALL;
    dungeon_tiles[4 + MAP_COLS] = MAP_WALL;
    dungeon_tiles[5 + MAP_COLS] = MAP_WALL;
    dungeon_tiles[1 + MAP_COLS*2] = MAP_WALL;
    dungeon_tiles[2 + MAP_COLS*2] = MAP_ROOM;
    dungeon_tiles[4 + MAP_COLS*2] = MAP_ROOM;
    dungeon_tiles[4 + MAP_COLS*2] = MAP_ROOM;
    dungeon_tiles[5 + MAP_COLS*2] = MAP_WALL;
    dungeon_tiles[1 + MAP_COLS*3] = MAP_WALL;
    dungeon_tiles[2 + MAP_COLS*3] = MAP_ROOM;
    dungeon_tiles[3 + MAP_COLS*3] = MAP_ROOM;
    dungeon_tiles[4 + MAP_COLS*3] = MAP_ROOM;
    dungeon_tiles[5 + MAP_COLS*3] = MAP_WALL;
    dungeon_tiles[1 + MAP_COLS*4] = MAP_WALL;
    dungeon_tiles[2 + MAP_COLS*4] = MAP_WALL;
    dungeon_tiles[3 + MAP_COLS*4] = MAP_WALL;
    dungeon_tiles[4 + MAP_COLS*4] = MAP_WALL;
    dungeon_tiles[5 + MAP_COLS*4] = MAP_WALL;

    Rect dimensions;
    unsigned char *room = room_start(0,0);
    room_dimensions(room,&dimensions);
    ASSERT_EQUAL((int) room, (int) dungeon_tiles + 1 + MAP_COLS);
    ASSERT_EQUAL(dimensions.x, 1);
    ASSERT_EQUAL(dimensions.y, 1);
    ASSERT_EQUAL(dimensions.w, 5);
    ASSERT_EQUAL(dimensions.h, 4);
}

CTEST2(map, testFindRoomWithDimensions) {
    dungeon_tiles[BSP_WIDTH + 1] = MAP_WALL;
    dungeon_tiles[BSP_WIDTH + 2] = MAP_WALL;
    dungeon_tiles[BSP_WIDTH + 3] = MAP_WALL;
    dungeon_tiles[BSP_WIDTH + 4] = MAP_WALL;
    dungeon_tiles[BSP_WIDTH + 5] = MAP_WALL;
    dungeon_tiles[BSP_WIDTH + 1 + MAP_COLS] = MAP_WALL;
    dungeon_tiles[BSP_WIDTH + 2 + MAP_COLS] = MAP_ROOM;
    dungeon_tiles[BSP_WIDTH + 4 + MAP_COLS] = MAP_ROOM;
    dungeon_tiles[BSP_WIDTH + 4 + MAP_COLS] = MAP_ROOM;
    dungeon_tiles[BSP_WIDTH + 5 + MAP_COLS] = MAP_WALL;
    dungeon_tiles[BSP_WIDTH + 1 + MAP_COLS*2] = MAP_WALL;
    dungeon_tiles[BSP_WIDTH + 2 + MAP_COLS*2] = MAP_ROOM;
    dungeon_tiles[BSP_WIDTH + 3 + MAP_COLS*2] = MAP_ROOM;
    dungeon_tiles[BSP_WIDTH + 4 + MAP_COLS*2] = MAP_ROOM;
    dungeon_tiles[BSP_WIDTH + 5 + MAP_COLS*2] = MAP_WALL;
    dungeon_tiles[BSP_WIDTH + 1 + MAP_COLS*3] = MAP_WALL;
    dungeon_tiles[BSP_WIDTH + 2 + MAP_COLS*3] = MAP_WALL;
    dungeon_tiles[BSP_WIDTH + 3 + MAP_COLS*3] = MAP_WALL;
    dungeon_tiles[BSP_WIDTH + 4 + MAP_COLS*3] = MAP_WALL;
    dungeon_tiles[BSP_WIDTH + 5 + MAP_COLS*3] = MAP_WALL;

    Rect dimensions;
    unsigned char *room = room_start(BSP_WIDTH,0);
    room_dimensions(room,&dimensions);
    ASSERT_EQUAL((int) room, (int) dungeon_tiles + BSP_WIDTH + 1);
    ASSERT_EQUAL(dimensions.x, BSP_WIDTH+1);
    ASSERT_EQUAL(dimensions.y, 0);
    ASSERT_EQUAL(dimensions.w, 5);
    ASSERT_EQUAL(dimensions.h, 4);
}

CTEST2(map, testFindRoomWithDimensionsInAnotherBSP) {
    dungeon_tiles[BSP_WIDTH + 1] = MAP_WALL;
    dungeon_tiles[BSP_WIDTH + 2] = MAP_WALL;
    dungeon_tiles[BSP_WIDTH + 3] = MAP_WALL;
    dungeon_tiles[BSP_WIDTH + 4] = MAP_WALL;
    dungeon_tiles[BSP_WIDTH + 5] = MAP_WALL;
    dungeon_tiles[BSP_WIDTH + 1 + MAP_COLS] = MAP_WALL;
    dungeon_tiles[BSP_WIDTH + 2 + MAP_COLS] = MAP_ROOM;
    dungeon_tiles[BSP_WIDTH + 4 + MAP_COLS] = MAP_ROOM;
    dungeon_tiles[BSP_WIDTH + 4 + MAP_COLS] = MAP_ROOM;
    dungeon_tiles[BSP_WIDTH + 5 + MAP_COLS] = MAP_WALL;
    dungeon_tiles[BSP_WIDTH + 1 + MAP_COLS*2] = MAP_WALL;
    dungeon_tiles[BSP_WIDTH + 2 + MAP_COLS*2] = MAP_ROOM;
    dungeon_tiles[BSP_WIDTH + 3 + MAP_COLS*2] = MAP_ROOM;
    dungeon_tiles[BSP_WIDTH + 4 + MAP_COLS*2] = MAP_ROOM;
    dungeon_tiles[BSP_WIDTH + 5 + MAP_COLS*2] = MAP_WALL;
    dungeon_tiles[BSP_WIDTH + 1 + MAP_COLS*3] = MAP_WALL;
    dungeon_tiles[BSP_WIDTH + 2 + MAP_COLS*3] = MAP_WALL;
    dungeon_tiles[BSP_WIDTH + 3 + MAP_COLS*3] = MAP_WALL;
    dungeon_tiles[BSP_WIDTH + 4 + MAP_COLS*3] = MAP_WALL;
    dungeon_tiles[BSP_WIDTH + 5 + MAP_COLS*3] = MAP_WALL;

    Rect dimensions;
    unsigned char *room = room_start(BSP_WIDTH,0);
    room_dimensions(room,&dimensions);
    ASSERT_EQUAL((int) room, (int) dungeon_tiles + BSP_WIDTH + 1);
    ASSERT_EQUAL(dimensions.x, BSP_WIDTH+1);
    ASSERT_EQUAL(dimensions.y, 0);
    ASSERT_EQUAL(dimensions.w, 5);
    ASSERT_EQUAL(dimensions.h, 4);
}

CTEST2(map, testFindRoomWithDimensionsInAnotherBSPY) {
    dungeon_tiles[BSP_WIDTH + 1 + MAP_COLS*BSP_HEIGHT] = MAP_WALL;
    dungeon_tiles[BSP_WIDTH + 2 + MAP_COLS*BSP_HEIGHT] = MAP_WALL;
    dungeon_tiles[BSP_WIDTH + 3 + MAP_COLS*BSP_HEIGHT] = MAP_WALL;
    dungeon_tiles[BSP_WIDTH + 4 + MAP_COLS*BSP_HEIGHT] = MAP_WALL;
    dungeon_tiles[BSP_WIDTH + 5 + MAP_COLS*BSP_HEIGHT] = MAP_WALL;
    dungeon_tiles[BSP_WIDTH + 1 + MAP_COLS*(BSP_HEIGHT+1)] = MAP_WALL;
    dungeon_tiles[BSP_WIDTH + 2 + MAP_COLS*(BSP_HEIGHT+1)] = MAP_ROOM;
    dungeon_tiles[BSP_WIDTH + 4 + MAP_COLS*(BSP_HEIGHT+1)] = MAP_ROOM;
    dungeon_tiles[BSP_WIDTH + 4 + MAP_COLS*(BSP_HEIGHT+1)] = MAP_ROOM;
    dungeon_tiles[BSP_WIDTH + 5 + MAP_COLS*(BSP_HEIGHT+1)] = MAP_WALL;
    dungeon_tiles[BSP_WIDTH + 1 + MAP_COLS*(BSP_HEIGHT+2)] = MAP_WALL;
    dungeon_tiles[BSP_WIDTH + 2 + MAP_COLS*(BSP_HEIGHT+2)] = MAP_ROOM;
    dungeon_tiles[BSP_WIDTH + 3 + MAP_COLS*(BSP_HEIGHT+2)] = MAP_ROOM;
    dungeon_tiles[BSP_WIDTH + 4 + MAP_COLS*(BSP_HEIGHT+2)] = MAP_ROOM;
    dungeon_tiles[BSP_WIDTH + 5 + MAP_COLS*(BSP_HEIGHT+2)] = MAP_WALL;
    dungeon_tiles[BSP_WIDTH + 1 + MAP_COLS*(BSP_HEIGHT+3)] = MAP_WALL;
    dungeon_tiles[BSP_WIDTH + 2 + MAP_COLS*(BSP_HEIGHT+3)] = MAP_WALL;
    dungeon_tiles[BSP_WIDTH + 3 + MAP_COLS*(BSP_HEIGHT+3)] = MAP_WALL;
    dungeon_tiles[BSP_WIDTH + 4 + MAP_COLS*(BSP_HEIGHT+3)] = MAP_WALL;
    dungeon_tiles[BSP_WIDTH + 5 + MAP_COLS*(BSP_HEIGHT+3)] = MAP_WALL;

    Rect dimensions;
    unsigned char *room = room_start(BSP_WIDTH,BSP_HEIGHT);
    room_dimensions(room,&dimensions);
    ASSERT_TRUE((int) room);
    ASSERT_EQUAL((int) room, (int) dungeon_tiles + BSP_WIDTH + 1 + MAP_COLS*BSP_HEIGHT);
    ASSERT_EQUAL(dimensions.x, BSP_WIDTH+1);
    ASSERT_EQUAL(dimensions.y, BSP_HEIGHT);
    ASSERT_EQUAL(dimensions.w, 5);
    ASSERT_EQUAL(dimensions.h, 4);
}

// TODO Test room at end of map
