#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ctest/ctest.h"
#include "../dungeon_generator.h"
#include "../dungeon.h"
#include "../mob.h"
#include "../player.h"

#define RUN_N 10000

CTEST_DATA(dungeongen) {};
CTEST_SETUP(dungeongen) {
    unsigned int seed = time(0);
    printf("SEED: %d\n", seed);
    srand(seed);
    init_player();
    init_dungeon_tiles();
    init_mobs();
}

CTEST2(dungeongen, testThingsAreInitialized) {
    ASSERT_EQUAL(num_mobs, 0);
    for (int idx=0; idx<MAX_MOBS; ++idx) {
        ASSERT_EQUAL(mobs[idx].hp, 0);
    }
    for (int idx=0; idx<MAP_SIZE; ++idx) {
        ASSERT_EQUAL(dungeon_tiles[idx], MAP_ROCK);
    }
    ASSERT_EQUAL(player.exp, 0);
}

CTEST2(dungeongen, testMobIsGenerated) {
    int n = 0;
    generate_dlevel();
    clear_mobs();
    while (n < 1) {
        n = generate_mobs(1);
    }
    ASSERT_EQUAL(n, num_mobs);
    ASSERT_EQUAL(n, 1);
}

CTEST2(dungeongen, testOffscreenMobIsGenerated) {
    int n = 0;
    generate_dlevel();
    clear_mobs();
    while (n < 1) {
        n = generate_offscreen_mobs(1);
    }
    ASSERT_EQUAL(n, num_mobs);
    ASSERT_EQUAL(n, 1);
}

// TODO max mobs not working
CTEST2_SKIP(dungeongen, testMaxMobsAreGenerated) {
    int n = 0;
    generate_dlevel();
    clear_mobs();
    while (n < MAX_MOBS - 1) {
        n = generate_mobs(MAX_MOBS - 1);
    }
    ASSERT_EQUAL(n, num_mobs);
    ASSERT_EQUAL(n, MAX_MOBS);
}

CTEST2(dungeongen, testPlayerXYIsRoom) {
    unsigned char tile;
    for (int i=0; i<RUN_N; ++i) {
        generate_dlevel();
        tile = dungeon_tile_at(player.x, player.y);
        ASSERT_EQUAL(tile, MAP_ROOM);
    }
}

CTEST2(dungeongen, testStairXYExists) {
    unsigned short idx;
    unsigned char tile = MAP_ROCK;

    for (int i=0; i<RUN_N; ++i) { // run test multiple times to ensure reasonable distribution
        generate_dlevel();
        // find the stair tile
        for (idx=0; idx<MAP_SIZE; ++idx) {
            tile = dungeon_tiles[idx];
            if (tile == MAP_STAIR) {
                break;
            }
        }

        ASSERT_EQUAL(tile, MAP_STAIR);
    }
}

CTEST2(dungeongen, testStairXYIsInRoom) {
    unsigned short idx;
    unsigned char x, y, room1, room2, room3, room4;
    unsigned char tile = MAP_ROCK;

    for (int i=0; i<RUN_N; ++i) { // run test multiple times to ensure reasonable distribution
        generate_dlevel();

        // find the stair tile
        for (x=0; x<MAP_COLS; ++x) {
            for (y=0; y<MAP_ROWS; ++y) {
                idx = xy_to_idx(x, y);
                tile = dungeon_tiles[idx];
                if (tile == MAP_STAIR) break;
            }
            if (tile == MAP_STAIR) break;
        }

        ASSERT_EQUAL(tile, MAP_STAIR);

        room1 = room2 = room3 = room4 = 0;
        if (x > 0)
            room1 = is_room(dungeon_tiles[x - 1 + y*MAP_COLS]);
        if (x < MAP_COLS - 1)
            room2 = is_room(dungeon_tiles[x + 1 + y*MAP_COLS]);
        if (y > 0)
            room3 = is_room(dungeon_tiles[x + (y + 1)*MAP_COLS]);
        if (y < MAP_ROWS - 1)
            room4 = is_room(dungeon_tiles[x + (y - 1)*MAP_COLS]);

        // assert found at least 2 touching rooms
        ASSERT_GT(room1 + room2 + room3 + room4, 1);
    }
}
