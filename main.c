#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <joystick.h>
#include <ctype.h>

#include "dungeon.h"
#include "dungeon_generator.h"
#include "player.h"
#include "mob.h"

#define MAX_DRAW_BUFFER_SIZE 255

extern void init();
extern void render_map(unsigned char *tiles_start, unsigned char *tiles_end);
extern void render_buffer();
extern void wait_for_vblank();
extern unsigned char read_input();
extern short seed;

extern unsigned char draw_buffer;
extern unsigned char draw_buffer_idx;

void add_to_draw_buffer(unsigned char x, unsigned char y, unsigned char ch)
{
    unsigned short idx;
    unsigned char *draw_buffer_ptr;
    draw_buffer_ptr = &draw_buffer;

    if (draw_buffer_idx > MAX_DRAW_BUFFER_SIZE - 3) {
        // TODO error
        return;
    }

    idx = xy_to_idx(x, y);
    draw_buffer_ptr[draw_buffer_idx + 0] = ch;
    draw_buffer_ptr[draw_buffer_idx + 1] = idx & 0xFF;
    draw_buffer_ptr[draw_buffer_idx + 2] = (idx >> 8) & 0xFF;
    draw_buffer_idx += 3;
}

void title_screen()
{
    int ch;
    unsigned int seed = 0;
    const char *title = "8bithack";

    cputsxy(MAP_COLS/2 - strlen(title)/2, MAP_ROWS/2, title);

    while (1)
    {
        ++seed;
        ch = kbhit();
        if (ch) break;
    }

    srand(seed);
}

// return dungeon or player tile at location
unsigned short tile_at(unsigned char x, unsigned char y)
{
    unsigned short i;

    if (player.x == x && player.y == y) {
        return player_tile();
    }

    return dungeon_tile_at(x, y);
}

int main()
{
    unsigned char input, quit;
    unsigned short i;
    init();
    title_screen();

    init_player();
    init_dungeon_tiles();
    init_mobs();
    generate_dlevel();

    render_map(dungeon_tiles, dungeon_tiles_end);
    add_to_draw_buffer(player.x, player.y, player_tile());

    // game loop
    while (!quit) {
        // TODO mobs AI & draw seen mobs
        for (i=0; i<num_mobs; ++i) {
            if (mobs[i].hp > 0)
                add_to_draw_buffer(mobs[i].x, mobs[i].y, mob_tile(mobs[i].type));
            else
                add_to_draw_buffer(mobs[i].x, mobs[i].y, tile_at(mobs[i].x, mobs[i].y));
        }
        cleanup_mobs(); // remove killed mobs after tile updated
        // wait for vblank then render
        wait_for_vblank();
        render_buffer();
        // handle player input
        do {
            input = read_input();
        } while (input == 0);
        switch (input) {
            case 'q':
            case 'Q':
                quit = 1;
                break;
            case 'r':
                generate_dlevel();
                render_map(dungeon_tiles, dungeon_tiles_end);
                add_to_draw_buffer(player.x, player.y, player_tile());
                break;
            case 'j':
                add_to_draw_buffer(player.x, player.y, dungeon_tile_at(player.x, player.y));
                bump_player(DIRECTION_DOWN);
                add_to_draw_buffer(player.x, player.y, tile_at(player.x, player.y));
                break;
            case 'k':
                add_to_draw_buffer(player.x, player.y, dungeon_tile_at(player.x, player.y));
                bump_player(DIRECTION_UP);
                add_to_draw_buffer(player.x, player.y, tile_at(player.x, player.y));
                break;
            case 'l':
                add_to_draw_buffer(player.x, player.y, dungeon_tile_at(player.x, player.y));
                bump_player(DIRECTION_RIGHT);
                add_to_draw_buffer(player.x, player.y, tile_at(player.x, player.y));
                break;
            case 'h':
                add_to_draw_buffer(player.x, player.y, dungeon_tile_at(player.x, player.y));
                bump_player(DIRECTION_LEFT);
                add_to_draw_buffer(player.x, player.y, tile_at(player.x, player.y));
                break;
        }
    }

    clrscr();

    return 0;
}

// see: https://www.cc65.org/doc/coding.html
// see: https://github.com/ilmenit/CC65-Advanced-Optimizations
// see: https://github.com/gwilymk/bin2c
