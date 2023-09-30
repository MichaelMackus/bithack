#include <string.h>
#include <stdio.h>

#include "dungeon.h"
#include "draw.h"
#include "menu.h"

void title_menu()
{
    int ch;
    const char *title = "bithack";

    clear_screen();
    cputsxy(MAP_COLS/2 - strlen(title)/2, MAP_ROWS/2, title);

    seed = 0;
    while (1)
    {
        ++seed;
        ch = read_input();
        if (ch) break;
    }
}
