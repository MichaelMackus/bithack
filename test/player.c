#include "ctest/ctest.h"
#include "../player.h"
#include "../dungeon.h"
#include "../mob.h"

CTEST_DATA(player) {};
CTEST_SETUP(player) {
    init_dungeon_tiles();
    init_player();
    clear_mobs();
}

CTEST2(player, testRest) {
    unsigned short idx = xy_to_idx(player.x, player.y);
    dungeon_tiles[idx] = MAP_ROOM;
    player.hp = 1;
    try_rest();
    ASSERT_EQUAL(player.hp, 2);
}

CTEST2(player, testDoesntRestOutsideRoom) {
    unsigned short idx = xy_to_idx(player.x, player.y);
    dungeon_tiles[idx] = MAP_CORRIDOR;
    player.hp = 1;
    try_rest();
    ASSERT_EQUAL(player.hp, 1);
}

CTEST2(player, testDoesntRestOnDoor) {
    unsigned short idx = xy_to_idx(player.x, player.y);
    dungeon_tiles[idx] = MAP_DOOR;
    player.hp = 1;
    try_rest();
    ASSERT_EQUAL(player.hp, 1);
}

CTEST2(player, testDoesntRestWithMobNearby) {
    player.x = player.y = 0;
    unsigned short idx = xy_to_idx(player.x, player.y);
    dungeon_tiles[idx] = MAP_ROOM;
    dungeon_tiles[idx + 1] = MAP_ROOM;
    generate_mob(MOB_KOBOLD, 1, 0);
    player.hp = 1;
    try_rest();
    ASSERT_EQUAL(player.hp, 1);
}

CTEST2(player, testDoesRestWithDeadMobNearby) {
    player.x = player.y = 0;
    unsigned short idx = xy_to_idx(player.x, player.y);
    dungeon_tiles[idx] = MAP_ROOM;
    dungeon_tiles[idx + 1] = MAP_ROOM;
    generate_mob(MOB_KOBOLD, 1, 0);
    mobs[0].hp = 0;
    player.hp = 1;
    try_rest();
    ASSERT_EQUAL(player.hp, 2);
}

CTEST2(player, testRestDoesntHealMobsWhenNotResting) {
    player.x = player.y = 0;
    unsigned short idx = xy_to_idx(player.x, player.y);
    dungeon_tiles[idx] = MAP_ROOM;
    dungeon_tiles[idx + 1] = MAP_ROOM;
    generate_mob(MOB_KOBOLD, 1, 0);
    mobs[0].hp = 1;
    player.hp = 1;
    try_rest();
    ASSERT_EQUAL(player.hp, 1);
    ASSERT_EQUAL(mobs[0].hp, 1);
}

CTEST2(player, testRestHealsMobs) {
    player.x = player.y = 0;
    unsigned short idx = xy_to_idx(player.x, player.y);
    dungeon_tiles[idx] = MAP_ROOM;
    generate_mob(MOB_KOBOLD, 3, 0);
    mobs[0].hp = 1;
    player.hp = 1;
    try_rest();
    ASSERT_EQUAL(player.hp, 2);
    ASSERT_EQUAL(mobs[0].hp, 2);
}
