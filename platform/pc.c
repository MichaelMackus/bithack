#include <curses.h>
#include "../dungeon.h"

unsigned char *draw_buffer_ptr;
unsigned char draw_buffer_idx;

void init()
{
    initscr();
    cbreak();
    curs_set(0);
    noecho();
    draw_buffer_ptr = malloc(sizeof(*draw_buffer_ptr) * MAP_SIZE);
}

void deinit()
{
    endwin();
}

void clrscr()
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
    unsigned char ch;

    nodelay(0, true);
    ch = getch();
    nodelay(0, false);

    if (ch == ERR)
        return 0;
    else
        return ch;
}

unsigned char tile_to_ch(unsigned char tile) {
    switch (tile) {
        case MAP_ROOM:
            return '.';
        case MAP_CORRIDOR:
            return '#';
        case MAP_WALL:
            return ' ';
        case MAP_DOOR:
            return '+';
        case 0:
            return '@';
        default:
            return tile;
    }
}

void render_map(unsigned char *tiles_start, unsigned char *tiles_end)
{
    unsigned char x, y;

    clear();
    for (x=0; x < MAP_COLS; ++x) {
        for (y=0; y < MAP_ROWS; ++y) {
            mvaddch(y, x, tile_to_ch(dungeon_tiles[x + y*MAP_COLS]));
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
        mvaddch(idx_to_y(idx), idx_to_x(idx), tile_to_ch(tile));
        i += 3;
    }

    draw_buffer_idx = 0;
}

void wait_for_vblank() {}
