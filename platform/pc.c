#include <curses.h>
#include "../dungeon.h"

#define MAX_DRAW_BUFFER_SIZE MAP_SIZE * sizeof(int)

unsigned char *draw_buffer_ptr;
unsigned char draw_buffer_idx;

void init()
{
    initscr();
    cbreak();
    curs_set(0);
    noecho();
    draw_buffer_ptr = (unsigned char*) malloc(MAX_DRAW_BUFFER_SIZE);
    nodelay(stdscr, true);
    keypad(stdscr, true);
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
    int ch = getch();

    if (ch == ERR)
        return 0;

    if (ch == KEY_UP)
        return 'k';
    if (ch == KEY_LEFT)
        return 'h';
    if (ch == KEY_RIGHT)
        return 'l';
    if (ch == KEY_DOWN)
        return 'j';

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

void add_to_draw_buffer(unsigned char x, unsigned char y, unsigned char ch, unsigned char color)
{
    unsigned short idx;

    idx = xy_to_idx(x, y);

    if (draw_buffer_idx > MAX_DRAW_BUFFER_SIZE - 3) {
        // TODO error
        return;
    }

    draw_buffer_ptr[draw_buffer_idx + 0] = ch;
    draw_buffer_ptr[draw_buffer_idx + 1] = idx & 0xFF;
    draw_buffer_ptr[draw_buffer_idx + 2] = (idx >> 8) & 0xFF;
    draw_buffer_idx += 3;
}
