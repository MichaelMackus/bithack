#include <conio.h>

#define MAX_DRAW_BUFFER_SIZE 1000

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

void add_to_draw_buffer_idx(unsigned short idx, unsigned char ch)
{
    if (draw_buffer_idx > MAX_DRAW_BUFFER_SIZE - 3) {
        // TODO error
        return;
    }

    draw_buffer_ptr[draw_buffer_idx + 0] = ch;
    draw_buffer_ptr[draw_buffer_idx + 1] = idx & 0xFF;
    draw_buffer_ptr[draw_buffer_idx + 2] = (idx >> 8) & 0xFF;
    draw_buffer_idx += 3;
}

