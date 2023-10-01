#include "ctest/ctest.h"
#include "../player.h"
#include "../dungeon.h"

CTEST_DATA(player) {};
CTEST_SETUP(player) {
    init_player();
    clear_mobs();
}

CTEST(player, testRest) {
    unsigned short idx = xy_to_idx(player.x, player.y);
    dungeon_tiles[idx] = MAP_ROOM;
    player.hp = 1;
    try_rest();
    ASSERT_EQUAL(player.hp, 2);
}

CTEST(player, testDoesntRestOutsideRoom) {
    unsigned short idx = xy_to_idx(player.x, player.y);
    dungeon_tiles[idx] = MAP_CORRIDOR;
    player.hp = 1;
    try_rest();
    ASSERT_EQUAL(player.hp, 1);
}

CTEST(player, testDoesntRestOnDoor) {
    unsigned short idx = xy_to_idx(player.x, player.y);
    dungeon_tiles[idx] = MAP_DOOR;
    player.hp = 1;
    try_rest();
    ASSERT_EQUAL(player.hp, 1);
}
