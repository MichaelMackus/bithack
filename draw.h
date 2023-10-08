#ifndef DRAW_H
#define DRAW_H

#define GAME_COLOR_DARK   1
#define GAME_COLOR_WHITE  2
#define GAME_COLOR_GREY   3
#define GAME_COLOR_BROWN  4
#define GAME_COLOR_GREEN  5
#define GAME_COLOR_YELLOW 6
#define GAME_COLOR_RED    7

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
extern void add_to_draw_buffer(unsigned char x, unsigned char y, unsigned char ch, unsigned char color);

void draw_status_line();
void draw_tiles_player_can_see(unsigned char prev_x, unsigned char prev_y);
void draw_initial_scene();

#endif
