#include <curses.h>
#include "../dungeon.h"

#define MAX_DRAW_BUFFER_SIZE 1000

unsigned char *draw_buffer_ptr;
unsigned char draw_buffer_idx;

void init()
{
    initscr();
    cbreak();
    curs_set(0);
    noecho();
    draw_buffer_ptr = (unsigned char*) malloc(sizeof(*draw_buffer_ptr) * MAP_SIZE);
}

void deinit()
{
    endwin();
}

void clear_screen()
{
    clear();
}

void cputsxy(unsigned char x, unsigned char y, const char *str)
{
    mvprintw(y, x, str);
}

unsigned char read_input()
{
    return getch();
}

unsigned char kbhit()
{
    int ch;

    nodelay(stdscr, true);
    ch = getch();
    nodelay(stdscr, false);

    if (ch == ERR || ch > 255)
        return 0;
    else
        return ch;
}

void render_map(unsigned char *tiles_start, unsigned char *tiles_end)
{
    unsigned char x, y;

    clear();
    for (x=0; x < MAP_COLS; ++x) {
        for (y=0; y < MAP_ROWS; ++y) {
            mvaddch(y, x, dungeon_tiles[x + y*MAP_COLS]);
        }
    }
}

void render_buffer()
{
    unsigned char tile;
    unsigned short idx;
    unsigned short i = 0;
    while (i < draw_buffer_idx) {
        tile = draw_buffer_ptr[i];
        idx = (draw_buffer_ptr[i + 2] << 8) | draw_buffer_ptr[i + 1];
        mvaddch(idx_to_y(idx), idx_to_x(idx), tile);
        i += 3;
    }

    draw_buffer_idx = 0;
}

void wait_for_vblank() {}

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

