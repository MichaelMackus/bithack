#include "dungeon_generator.h"
#include "dungeon.h"
#include "mob.h"
#include "player.h"

#include <stdlib.h>

#define MAX_ITERATIONS 20

#define MAP_ERR MAP_SIZE+1

// NOTE: will return MAP_SIZE if reached MAX_ITERATIONS
unsigned short rand_room_idx();

// get room width
unsigned char room_width(unsigned char *start_tile)
{
    unsigned char w = 0;
    while (1) {
        if (start_tile >= dungeon_tiles_end || !is_room(*start_tile)) {
            break;
        }

        ++w;
        ++start_tile;
    }

    return w;
}
// get room height
unsigned char room_height(unsigned char *start_tile)
{
    unsigned char h = 0;
    while (1) {
        if (start_tile >= dungeon_tiles_end || !is_room(*start_tile)) {
            break;
        }

        ++h;
        start_tile = start_tile + MAP_COLS;
    }

    return h;
}

void draw_line(unsigned char from_x, unsigned char from_y, unsigned char to_x, unsigned char to_y, unsigned char start_direction)
{
    unsigned short index, middle_idx;
    unsigned char middle_x, middle_y;
    signed char offset_x = 0;
    signed char offset_y = 0;
    unsigned char diff_x = abs((signed char) from_x - (signed char) to_x);
    unsigned char diff_y = abs((signed char) from_y - (signed char) to_y);

    if (start_direction == DIRECTION_DOWN || start_direction == DIRECTION_UP) {
        if (from_y < to_y) {
            offset_y = 1;
            middle_y = from_y + (diff_y/2);
            middle_idx = xy_to_idx(from_x, middle_y);
        } else if (from_y > to_y) {
            offset_y = -1;
            middle_y = from_y - (diff_y/2);
            middle_idx = xy_to_idx(from_x, middle_y);
        }
    } else {
        if (from_x < to_x) {
            offset_x = 1;
            middle_x = from_x + (diff_x/2);
            middle_idx = xy_to_idx(middle_x, from_y);
        } else if (from_x > to_x) {
            offset_x = -1;
            middle_x = from_x - (diff_x/2);
            middle_idx = xy_to_idx(middle_x, from_y);
        }
    }

    while (offset_x != 0 || offset_y != 0)
    {
        // switch direction
        if (from_x == to_x && offset_x != 0) {
            offset_x = 0;
            if (from_y < to_y) {
                offset_y = 1;
            } else if (from_y > to_y) {
                offset_y = -1;
            }
        } else if (from_y == to_y && offset_y != 0) {
            offset_y = 0;
            if (from_x < to_x) {
                offset_x = 1;
            } else if (from_x > to_x) {
                offset_x = -1;
            }
        } else if (index == middle_idx && from_y != to_y && from_x != to_x) {
            // we're at middle - switch direction to ensure we don't
            // carve multiple walls
            // TODO not perfect - sometimes drawing multiple doors if we
            // TODO switch inside wall
            // TODO perhaps only switch if destination is on corner?
            // TODO or we can go in start direction maybe?
            if (offset_x != 0) {
                offset_x = 0;
                if (from_y < to_y) {
                    offset_y = 1;
                } else if (from_y > to_y) {
                    offset_y = -1;
                }
            } else if (offset_y != 0) {
                offset_y = 0;
                if (from_x < to_x) {
                    offset_x = 1;
                } else if (from_x > to_x) {
                    offset_x = -1;
                }
            }
        }
        from_x += offset_x;
        from_y += offset_y;
        index = from_x + from_y*sizeof(*dungeon_tiles)*MAP_COLS;
        if (dungeon_tiles[index] == MAP_ROCK)
            dungeon_tiles[index] = MAP_CORRIDOR;
        if (dungeon_tiles[index] == MAP_WALL) // TODO secret doors
            dungeon_tiles[index] = MAP_DOOR;
        // TODO switch direction again if we're targeting wall
    }
}


void draw_corridor_right(unsigned char bsp_x, unsigned char bsp_y, unsigned char screen_x, unsigned char screen_y)
{
    unsigned char offset_x, offset_y, w, h;
    unsigned char *next_room_tile;

    // return if we're at right side of screen
    if (bsp_x >= ROOMS_PER_ROW - 1) {
        return;
    }

    // get next room
    next_room_tile = room_start((bsp_x + 1)*BSP_WIDTH, bsp_y*BSP_HEIGHT);
    offset_x = (next_room_tile - dungeon_tiles) % MAP_COLS;
    offset_y = (next_room_tile - dungeon_tiles) / MAP_COLS;
    w = room_width(next_room_tile);
    h = room_height(next_room_tile);

    draw_line(screen_x, screen_y, offset_x + (w - 1)/2, offset_y + (h - 1)/2, DIRECTION_RIGHT);
}

void draw_corridor_down(unsigned char bsp_x, unsigned char bsp_y, unsigned char screen_x, unsigned char screen_y)
{
    unsigned char offset_x, offset_y, w, h;
    unsigned char *next_room_tile;

    // return if we're at right side of screen
    if (bsp_y >= ROOMS_PER_COL - 1) {
        return;
    }

    // get next room
    next_room_tile = room_start(bsp_x*BSP_WIDTH, (bsp_y + 1)*BSP_HEIGHT);
    offset_x = (next_room_tile - dungeon_tiles) % MAP_COLS;
    offset_y = (next_room_tile - dungeon_tiles) / MAP_COLS;
    w = room_width(next_room_tile);
    h = room_height(next_room_tile);

    draw_line(screen_x, screen_y, offset_x + (w - 1)/2, offset_y + (h - 1)/2, DIRECTION_DOWN);
}

void carve_room(unsigned char i)
{
    unsigned char x, y, w, h, offset_x, offset_y;
    unsigned char *start_tile;
    unsigned char *cur_tile;

    // generate random room parameters
    x = i % ROOMS_PER_COL;
    y = i / ROOMS_PER_COL;
    w = rand() % BSP_WIDTH;
    h = rand() % BSP_HEIGHT;
    if (w > MAX_ROOM_WIDTH) w = MAX_ROOM_WIDTH;
    else if (w < MIN_ROOM_WIDTH) w = MIN_ROOM_WIDTH;
    if (h > MAX_ROOM_HEIGHT) h = MAX_ROOM_HEIGHT;
    else if (h < MIN_ROOM_HEIGHT) h = MIN_ROOM_HEIGHT;
    // tile offset
    start_tile = dungeon_tiles + x*BSP_WIDTH + y*BSP_HEIGHT*MAP_COLS;
    if (w < BSP_WIDTH - BSP_PADDING) {
        offset_x = rand() % (BSP_WIDTH - w - BSP_PADDING); // random X offset
        start_tile += offset_x;
    }
    if (h < BSP_HEIGHT - BSP_PADDING) {
        offset_y = rand() % (BSP_HEIGHT - h - BSP_PADDING); // random Y offset
        start_tile += offset_y*MAP_COLS;
    }
    // fill tiles
    cur_tile = start_tile;
    for (y = 0; y <= h; ++y) {
        for (x = 0; x <= w; ++x) {
            if (x == 0 || x == w || y == 0 || y == h)
                *cur_tile = MAP_WALL;
            else
                *cur_tile = MAP_ROOM;
            ++cur_tile;
        }
        start_tile = start_tile + MAP_COLS;
        cur_tile = start_tile;
    }
}

void generate_tiles()
{
    unsigned short i;
    unsigned char x, y, w, h, offset_x, offset_y;
    unsigned char *start_tile;
    unsigned char *cur_tile;
    cur_tile = dungeon_tiles;

    // clear map
    for (i = 0; i < MAP_SIZE; ++i) {
        *cur_tile = MAP_ROCK;
        ++cur_tile;
    }

    // generate rooms
    for (i = 0; i < ROOMS_PER_ROW*ROOMS_PER_COL; ++i) {
        carve_room(i);
    }

    // connect rooms with corridors
    for (i = 0; i < ROOMS_PER_ROW*ROOMS_PER_COL; ++i) {
        // generate random room parameters
        x = i % ROOMS_PER_ROW;
        y = i / ROOMS_PER_ROW;
        // tile offset
        start_tile = room_start(x*BSP_WIDTH, y*BSP_HEIGHT);
        offset_x = (start_tile - dungeon_tiles) % MAP_COLS;
        offset_y = (start_tile - dungeon_tiles) / MAP_COLS;
        w = room_width(start_tile);
        h = room_height(start_tile);
        // TODO only draw sometimes
        draw_corridor_right(x, y, offset_x + (w - 1)/2, offset_y + (h - 1)/2);
        draw_corridor_down(x, y, offset_x + (w - 1)/2, offset_y + (h - 1)/2);
    }
}

unsigned char generate_mobs(unsigned char mobs_to_generate)
{
    // generate mobs on level change
    unsigned short idx;
    unsigned char i, x, y, result;
    unsigned char count = 0;

    if (mobs_to_generate > MAX_MOBS)
        mobs_to_generate = MAX_MOBS;

    // generate some monsters
    // TODO difficulty
    for (i=0; i<mobs_to_generate; ++i) {
        count = 0;
        do {
            idx = rand_room_idx();
            if (idx == MAP_ERR) return i;
            x = idx_to_x(idx);
            y = idx_to_y(idx);
            ++count;
            if (count >= MAX_ITERATIONS) return i;
        } while (!is_passable(x, y));
        result = rand() % 100;
        if (result < 40) {
            generate_mob(MOB_KOBOLD, x, y);
        } else if (result < 75) {
            generate_mob(MOB_GOBLIN, x, y);
        } else {
            generate_mob(MOB_ORC, x, y);
        }
    }

    return mobs_to_generate;
}

// TODO refactor
unsigned char generate_offscreen_mobs(unsigned char mobs_to_generate)
{
    // generate mobs on level change
    unsigned short idx;
    unsigned char i, x, y, result;
    unsigned char count = 0;

    if (mobs_to_generate > MAX_MOBS)
        mobs_to_generate = MAX_MOBS;

    // generate some monsters
    // TODO difficulty
    for (i=0; i<mobs_to_generate; ++i) {
        count = 0;
        do {
            idx = rand_room_idx();
            if (idx == MAP_ERR) return i;
            x = idx_to_x(idx);
            y = idx_to_y(idx);
            ++count;
            if (count >= MAX_ITERATIONS) return i;
        } while (!is_passable(x, y) && can_see(player.x, player.y, x, y));
        result = rand() % 100;
        if (result < 40) {
            generate_mob(MOB_KOBOLD, x, y);
        } else if (result < 75) {
            generate_mob(MOB_GOBLIN, x, y);
        } else {
            generate_mob(MOB_ORC, x, y);
        }
    }

    return mobs_to_generate;
}

unsigned char generate_player_xy()
{
    unsigned short idx;
    unsigned char x, y;
    unsigned char count = 0;

    do {
        idx = rand_room_idx();
        if (idx == MAP_ERR) return 0;
        x = idx_to_x(idx);
        y = idx_to_y(idx);
        ++count;
        if (count >= MAX_ITERATIONS) return 0;
    } while (!is_passable(x, y));
    change_player_dlevel(x, y);

    return 1;
}

unsigned char generate_stair_xy()
{
    unsigned short idx;
    unsigned char x, y;
    unsigned char count = 0;

    do {
        idx = rand_room_idx();
        if (idx == MAP_ERR) return 0;
        x = idx_to_x(idx);
        y = idx_to_y(idx);
        ++count;
        if (count >= MAX_ITERATIONS) return 0;
    } while (mob_at(x, y) || (x == player.x && y == player.y));
    dungeon_tiles[idx] = MAP_STAIR;

    return 1;
}

// TODO we can make this faster with BSP lookup
unsigned short rand_room_idx()
{
    unsigned short idx;
    unsigned char count = 0;

    do
    {
        idx = rand() % MAP_SIZE;
        ++count;
        if (count >= MAX_ITERATIONS) return MAP_ERR;
    } while (dungeon_tiles[idx] != MAP_ROOM || !is_passable(idx_to_x(idx), idx_to_y(idx)));

    return idx;
}

void generate_dlevel()
{
    unsigned char generation_result;
    unsigned char mobs_to_generate;

    while (1) {
        mobs_to_generate = rand() % (NUM_MOBS_DUNGEON_CRAVES - MIN_MOBS) + MIN_MOBS;
        // generate dungeon tile map
        generate_tiles();
        clear_mobs();
        generate_mobs(mobs_to_generate);
        generation_result = generate_player_xy();
        if (!generation_result) {
            continue;
        }
        generation_result = generate_stair_xy();
        if (!generation_result) {
            continue;
        }
        // done generating
        break;
    }
}
