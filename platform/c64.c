#include <conio.h>

#include "../dungeon.h"

#define C64_SCREEN_RAM       0xC400
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

void add_to_draw_buffer(unsigned char x, unsigned char y, unsigned char ch, unsigned char color)
{
    unsigned short idx;

    idx = xy_to_idx(x, y);

    if (draw_buffer_idx > MAX_DRAW_BUFFER_SIZE - 3) {
        // TODO error
        return;
    }

    idx += C64_SCREEN_RAM;
    draw_buffer_ptr[draw_buffer_idx + 0] = tileset_index(ch);
    draw_buffer_ptr[draw_buffer_idx + 1] = idx & 0xFF;
    draw_buffer_ptr[draw_buffer_idx + 2] = (idx >> 8) & 0xFF;
    draw_buffer_idx += 3;
}
