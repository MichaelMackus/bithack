#include "../dungeon.h"

#define NES_NAMETABLE 0x2000
#define MAX_DRAW_BUFFER_SIZE 256

extern unsigned char draw_buffer;
extern unsigned char draw_buffer_idx;
unsigned char *draw_buffer_ptr = &draw_buffer;
extern void render_buffer();
extern void wait_for_vblank();

void deinit()
{
}

void add_to_draw_buffer_idx(unsigned short idx, unsigned char ch)
{
    if (draw_buffer_idx > MAX_DRAW_BUFFER_SIZE - 3) {
        // TODO error
        return;
    }

    idx += NES_NAMETABLE;

    draw_buffer_ptr[draw_buffer_idx + 0] = tileset_index(ch);
    draw_buffer_ptr[draw_buffer_idx + 1] = idx & 0xFF;
    draw_buffer_ptr[draw_buffer_idx + 2] = (idx >> 8) & 0xFF;
    draw_buffer_idx += 3;
}

// TODO color (perhaps use sprites?)
void add_to_draw_buffer(unsigned char x, unsigned char y, unsigned char ch, unsigned char color)
{
    unsigned short idx;

    idx = xy_to_idx(x, y);
    add_to_draw_buffer_idx(idx, ch);
}

void cputsxy(unsigned char x, unsigned char y, const char *str)
{
    unsigned char ch;
    const char *curstr = str;
    unsigned char i = 0;
    unsigned short idx = xy_to_idx(x, y);
    while (1) {
        if (*curstr == '\0') break;
        ch = (unsigned char) *curstr;

        // convert char to tileset
        if (ch == '@') {
            ch = 0;
        } else if (ch >= 'a' && ch <= 'z') {
            ch = ch - 'a' + 0x01; // a is at col 1 row 0
        } else if (ch >= 'A' && ch <= 'Z') {
            ch = ch - 'A' + 0x41; // A is at row 4 col 1
        } else if (ch >= ' ' && ch <= '?') {
            ch = ch - ' ' + 0x20; // space is at row 2 col 0
        } else {
            ch = 0x3F; // ?
        }

        add_to_draw_buffer_idx(idx, ch);

        ++idx;
        ++curstr;
    }

    wait_for_vblank();
    render_buffer();
}
