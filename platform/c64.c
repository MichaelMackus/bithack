#include <conio.h>

#include "../dungeon.h"
#include "../draw.h"

#define MAX_DRAW_BUFFER_SIZE 256

extern unsigned char draw_buffer;
extern unsigned char draw_buffer_idx;
unsigned char *draw_buffer_ptr = &draw_buffer;

void deinit()
{
    clrscr();
}

void clear_screen()
{
    clrscr();
}

unsigned char c64_color(unsigned char game_color)
{
    switch (game_color) {
        case GAME_COLOR_DARK:
            return 11;
        case GAME_COLOR_GREY:
            return 12;
        case GAME_COLOR_WHITE:
            return 1;
        case GAME_COLOR_RED:
            return 2;
        case GAME_COLOR_BROWN:
            return 9;
        case GAME_COLOR_GREEN:
            return 5;
        case GAME_COLOR_YELLOW:
            return 7;
        default:
            return 1; // white
    }
}

void add_to_draw_buffer_idx(unsigned short idx, unsigned char ch, unsigned char color)
{
    if (draw_buffer_idx > MAX_DRAW_BUFFER_SIZE - 4) {
        // TODO error
        return;
    }

    draw_buffer_ptr[draw_buffer_idx + 0] = tileset_index(ch);
    draw_buffer_ptr[draw_buffer_idx + 1] = idx & 0xFF;
    draw_buffer_ptr[draw_buffer_idx + 2] = (idx >> 8) & 0xFF;
    draw_buffer_ptr[draw_buffer_idx + 3] = c64_color(color);
    draw_buffer_idx += 4;
}

void add_to_draw_buffer(unsigned char x, unsigned char y, unsigned char ch, unsigned char color)
{
    unsigned short idx = xy_to_idx(x, y);
    add_to_draw_buffer_idx(idx, ch, color);
}
