#include <conio.h>

extern unsigned char draw_buffer;
unsigned char *draw_buffer_ptr = &draw_buffer;

void deinit()
{
    clrscr();
}
