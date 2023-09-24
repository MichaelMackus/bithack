#include <stdlib.h>

#ifdef PC
#include "platform/pc.c"
#elif defined(__NES__)
#include "platform/nes.c"
#else
#include "platform/c64.c"
#endif

#include "dungeon.h"
#include "dungeon_generator.h"
#include "mob.h"
#include "player.h"

#define MAX_DRAW_BUFFER_SIZE 255

extern void init();
extern void deinit();
extern void render_map(unsigned char *tiles_start, unsigned char *tiles_end);
extern void render_buffer();
extern void wait_for_vblank();
extern unsigned char read_input();
extern unsigned char *draw_buffer_ptr;
extern unsigned char draw_buffer_idx;
extern void cputsxy(unsigned char x, unsigned char y, const char *str);
extern void clear_screen();
extern unsigned char kbhit();

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

void add_to_draw_buffer(unsigned char x, unsigned char y, unsigned char ch)
{
    unsigned short idx;

    idx = xy_to_idx(x, y);
    add_to_draw_buffer_idx(idx, ch);
}

unsigned int seed;
void title_screen()
{
    int ch;
    const char *title = "bithack";

    clear_screen();
    /* cputsxy(MAP_COLS/2 - strlen(title)/2, MAP_ROWS/2, title); */

    seed = 0;
    while (1)
    {
        ++seed;
        ch = kbhit();
        if (ch) break;
    }

    srand(seed);
}

void draw_status_line()
{
    /* char message[MAP_COLS]; */
    /* sprintf(message, "HP: %d/%d", player.hp, player.max_hp); */
    /* cputsxy(0, 23, message); */
}

// TODO don't draw duplicate tiles, flashing on c64
void draw_tiles_player_can_see()
{
    unsigned char x, y;
    unsigned char *room, *start;
    unsigned short idx_start;
    unsigned short idx = xy_to_idx(player.x, player.y);

    // draw adjacent corridors TODO diagonals
    if (player.x < MAP_COLS - 1)
        add_to_draw_buffer(player.x + 1, player.y, dungeon_tiles[idx + 1]);
    if (player.x > 0)
        add_to_draw_buffer(player.x - 1, player.y, dungeon_tiles[idx - 1]);
    if (player.y < MAP_ROWS - 1)
        add_to_draw_buffer(player.x, player.y + 1, dungeon_tiles[idx + MAP_COLS]);
    if (player.y > 0)
        add_to_draw_buffer(player.x, player.y - 1, dungeon_tiles[idx - MAP_COLS]);

    if (is_room(dungeon_tiles[idx])) {
        // draw lit room we're in
        x = bsp_x(player.x);
        y = bsp_y(player.y);
        room = room_start(x, y);
        start = room;
        
        idx = room - dungeon_tiles;
        idx_start = idx;
        do {
            do {
                add_to_draw_buffer_idx(idx, *room);
                ++room;
                ++idx;
            } while (is_room(*room));
            start += MAP_COLS;
            room = start;
            idx_start += MAP_COLS;
            idx = idx_start;
        } while (is_room(*start));
    }

    // draw player
    add_to_draw_buffer(player.x, player.y, player_tile());
}

// handle mob AI & draw seen mobs
void mob_ai()
{
    unsigned short i;
    for (i=0; i<num_mobs; ++i) {
        if (mobs[i].hp > 0 && can_see(mobs[i].x, mobs[i].y, player.x, player.y)) {
            add_to_draw_buffer(mobs[i].x, mobs[i].y, dungeon_tiles[xy_to_idx(mobs[i].x, mobs[i].y)]); // TODO check seen tiles
            move_or_attack_towards(&mobs[i], player.x, player.y);
            add_to_draw_buffer(mobs[i].x, mobs[i].y, mob_tile(mobs[i].type));
        }
    }
}

void draw_initial_scene()
{
    unsigned short i;
    wait_for_vblank();
    clear_screen();
    draw_tiles_player_can_see();
    add_to_draw_buffer(player.x, player.y, player_tile());
    wait_for_vblank();
    render_buffer();
    for (i=0; i<num_mobs; ++i) {
        if (mobs[i].hp > 0 && can_see(player.x, player.y, mobs[i].x, mobs[i].y)) {
            add_to_draw_buffer(mobs[i].x, mobs[i].y, mob_tile(mobs[i].type));
        }
    }
    wait_for_vblank();
    render_buffer();
    draw_status_line();
}

int main()
{
    unsigned char input = 0;
    unsigned char quit = 0;
    unsigned char lost = 0;

    init();
    /* title_screen(); */

    read_input();

    init_player();
    init_dungeon_tiles();
    init_mobs();

    // generate the level & draw it
    generate_dlevel();
    draw_initial_scene();

    // game loop
    while (!quit && !lost) {
        // handle player input
        do {
            input = read_input();
        } while (input == 0);
        switch (input) {
            case 'q':
            case 'Q':
                quit = 1;
                break;
            case 'r':
                generate_dlevel();
                clear_screen();
                draw_tiles_player_can_see();
                break;
            case 'R':
                generate_dlevel();
                clear_screen();
                render_map(dungeon_tiles, dungeon_tiles_end);
                draw_tiles_player_can_see();
                break;
            case 'j':
                bump_player(DIRECTION_DOWN);
                draw_tiles_player_can_see();
                break;
            case 'k':
                bump_player(DIRECTION_UP);
                draw_tiles_player_can_see();
                break;
            case 'l':
                bump_player(DIRECTION_RIGHT);
                draw_tiles_player_can_see();
                break;
            case 'h':
                bump_player(DIRECTION_LEFT);
                draw_tiles_player_can_see();
                break;
            case '>':
                if (dungeon_tile_at(player.x, player.y) == MAP_STAIR) {
                    // TODO increase depth/difficulty
                    generate_dlevel();
                    draw_initial_scene();
                }
                break;
        }
        // mob AI & render mobs
        mob_ai();
        // draw statusline
        draw_status_line();
        // render after vblank
        wait_for_vblank();
        render_buffer();

        if (player.hp == 0)
            lost = 1;
    }

    deinit();

    if (lost) {
        /* printf("You were killed!\n"); */
    }

    /* printf("Seed: %d\n", seed); */

    return 0;
}

// see: https://www.cc65.org/doc/coding.html
// see: https://github.com/ilmenit/CC65-Advanced-Optimizations
// see: https://github.com/gwilymk/bin2c
