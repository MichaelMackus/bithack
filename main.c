#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>

#include "dungeon.h"
#include "dungeon_generator.h"
#include "player.h"

#define MAX_DRAW_BUFFER_SIZE 255

extern void init();
extern void render_map(unsigned char *tiles_start, unsigned char *tiles_end);
extern void render_buffer();
extern char prng();
extern short seed;

extern unsigned char *dungeon_tiles;
extern unsigned char *dungeon_tiles_end;
extern unsigned char draw_buffer;
extern unsigned char draw_buffer_idx;
extern Player player;

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
    draw_buffer_idx = 3;
}

void generate_dlevel()
{
    unsigned char x, y;
    unsigned int idx;

    // generate dungeon tile map
    generate_tiles();
    // generate player location
    idx = rand_room_idx();
    x = idx_to_x(idx);
    y = idx_to_y(idx);
    change_player_dlevel(x, y);
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

int main()
{
    char input;
    init();
    title_screen();

    init_player();
    init_dungeon_tiles();
    generate_dlevel();

    render_map(dungeon_tiles, dungeon_tiles_end);
    add_to_draw_buffer(player.x, player.y, player_tile());

    while (1) {
        render_buffer();
        input = cgetc();
        if (input == 'q' || input == 'Q') break;
        if (input == 'r') {
            generate_dlevel();
            render_map(dungeon_tiles, dungeon_tiles_end);
            add_to_draw_buffer(player.x, player.y, player_tile());
        }
    }

    return 0;
}

// see: https://www.cc65.org/doc/coding.html
// see: https://github.com/ilmenit/CC65-Advanced-Optimizations
// see: https://github.com/gwilymk/bin2c
