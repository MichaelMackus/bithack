#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

#include "dungeon.h"
#include "dungeon_generator.h"

extern void init();
extern void render(unsigned char *tiles_start, unsigned char *tiles_end);
extern char prng();
extern short seed;

extern unsigned char *dungeon_tiles;
extern unsigned char *dungeon_tiles_end;

int main()
{
    init_dungeon_tiles();
    seed = (rand() % 256 << 8) | (rand() % 256);
    generate_tiles();

    init();

    // TODO render off by one?
    while (1) {
        generate_tiles();
        render(dungeon_tiles, dungeon_tiles_end);
        cgetc();
    }

    return 0;
}

// see: https://www.cc65.org/doc/coding.html
// see: https://github.com/ilmenit/CC65-Advanced-Optimizations
// see: https://github.com/gwilymk/bin2c
