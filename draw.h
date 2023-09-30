#ifndef DRAW_H
#define DRAW_H

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
extern void add_to_draw_buffer_idx(unsigned short idx, unsigned char ch);

void add_to_draw_buffer(unsigned char x, unsigned char y, unsigned char ch);
void draw_status_line();
void draw_tiles_player_can_see();
void draw_initial_scene();

#endif
