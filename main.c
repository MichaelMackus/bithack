#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#ifdef PC
#include "platform/pc.c"
#elif defined(__NES__)
#include "platform/nes.c"
#else
#include "platform/c64.c"
#endif

#include "dungeon.h"
#include "dungeon_generator.h"
#include "draw.h"
#include "menu.h"
#include "mob.h"
#include "player.h"

// handle mob AI & draw seen mobs
void mob_ai()
{
    unsigned char x, y;
    unsigned short i;
    for (i=0; i<num_mobs; ++i) {
        if (mobs[i].hp > 0 && can_see(mobs[i].x, mobs[i].y, player.x, player.y)) {
            x = mobs[i].x;
            y = mobs[i].y;
            move_or_attack_towards(&mobs[i], player.x, player.y);
            if (x != mobs[i].x || y != mobs[i].y)
                add_to_draw_buffer(x, y, dungeon_tiles[xy_to_idx(x, y)], GAME_COLOR_WHITE);
            add_to_draw_buffer(mobs[i].x, mobs[i].y, mob_tile(mobs[i].type), mob_color(mobs[i].type));
        }
    }
}

unsigned int seed;
unsigned int turn;

int main()
{
    unsigned char input = 0;
    unsigned char quit = 0;
    unsigned char lost = 0;
    unsigned char prev_x, prev_y;

    turn = 1;
    init();
    title_menu();
    srand(seed); // updated from title_menu

    init_player();
    init_dungeon_tiles();
    init_mobs();

    // generate the level & draw it
    generate_dlevel();
    draw_initial_scene();

    // game loop
    while (!quit && !lost) {
        // handle player input
        do {
            input = read_input();
        } while (input == 0);
        prev_x = player.x;
        prev_y = player.y;
        switch (input) {
            case 'q':
            case 'Q':
                quit = 1;
                break;
            case 'r':
                generate_dlevel();
                clear_screen();
                draw_tiles_player_can_see(player.x, player.y);
                break;
            case 'R':
                generate_dlevel();
                clear_screen();
                render_map(dungeon_tiles, dungeon_tiles_end);
                draw_tiles_player_can_see(prev_x, prev_y);
                break;
            case 'j':
                bump_player(DIRECTION_DOWN);
                draw_tiles_player_can_see(prev_x, prev_y);
                break;
            case 'k':
                bump_player(DIRECTION_UP);
                draw_tiles_player_can_see(prev_x, prev_y);
                break;
            case 'l':
                bump_player(DIRECTION_RIGHT);
                draw_tiles_player_can_see(prev_x, prev_y);
                break;
            case 'h':
                bump_player(DIRECTION_LEFT);
                draw_tiles_player_can_see(prev_x, prev_y);
                break;
            case '>':
            case '\n':
                if (dungeon_tile_at(player.x, player.y) == MAP_STAIR) {
                    ++dungeon_depth;
                    generate_dlevel();
                    draw_initial_scene();
                }
                break;
        }

        // attempt to rest every turn
        try_rest();

        // mob AI
        mob_ai();
        wait_for_vblank();
        render_buffer();
        // update statusline (HP, etc.)
        draw_status_line();
        wait_for_vblank();
        render_buffer();
        // render mobs after AI
        wait_for_vblank();
        render_buffer();

        // chance to generate (offscreen) mobs every 10 turns
        if (turn % 10 == 0 && alive_mobs() < NUM_MOBS_DUNGEON_CRAVES && rand() % 6 == 0)
            generate_offscreen_mobs(MIN_MOBS);

        if (player.hp == 0)
            lost = 1;

        // increment turn counter
        ++turn;
    }

    deinit();

    if (lost) {
#ifndef __NES__
        printf("You were killed!\n");
#endif
    }

#ifndef __NES__
    printf("Seed: %d\n", seed);
#endif

    return 0;
}

// see: https://www.cc65.org/doc/coding.html
// see: https://github.com/ilmenit/CC65-Advanced-Optimizations
// see: https://github.com/gwilymk/bin2c
