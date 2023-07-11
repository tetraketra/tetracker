#ifndef WINDOW_H
#define WINDOW_H

#include <ncursesw/curses.h>

typedef struct {
	char ls[4], rs[4], ts[4], bs[4], 
	 	 tl[4], tr[4], bl[4], br[4];
} WIN_BORDER;

typedef struct {
	int startx, starty;
	int height, width;
	WIN_BORDER border;
	bool draw_border;
} WIN_INFO;

extern WIN_BORDER WB_STANDALONE;

extern WIN_BORDER WB_CONNECTS_RIGHT;
extern WIN_BORDER WB_CONNECTS_LEFT;
extern WIN_BORDER WB_CONNECTS_UP;
extern WIN_BORDER WB_CONNECTS_DOWN;

extern WIN_BORDER WB_CONNECTS_RIGHT_DOWN; 
extern WIN_BORDER WB_CONNECTS_RIGHT_UP;
extern WIN_BORDER WB_CONNECTS_RIGHT_UP_DOWN;

extern WIN_BORDER WB_CONNECTS_LEFT_DOWN;
extern WIN_BORDER WB_CONNECTS_LEFT_UP;
extern WIN_BORDER WB_CONNECTS_LEFT_UP_DOWN;

extern WIN_BORDER WB_CONNECTS_LR_DOWN;
extern WIN_BORDER WB_CONNECTS_LR_UP;
extern WIN_BORDER WB_CONNECTS_LR_UP_DOWN;

WINDOW* window_init(WIN_INFO* info_struct);
void window_draw_border_to(WINDOW* windows[], int win_order_code, WIN_INFO* info_struct);
void window_init_with_border(WINDOW* windows[], int win_order_code, WIN_INFO* info_struct);
void window_move_and_resize_and_draw_border_to(WINDOW* windows[], int win_order_code, WIN_INFO* info_struct);
void windows_refresh_all(WINDOW* windows[], int win_count);
void windows_clear_all(WINDOW* windows[], int win_count);
void windows_draw_borders_to_all(WINDOW* windows[], int win_count, WIN_INFO* info_structs[]);
void window_fill_with_unicode_four_wide(WINDOW* windows[], int win_order_code, WIN_INFO* info_struct, char fill_char[4], bool respect_border);
void window_fill_with_char(WINDOW* windows[], int win_order_code, WIN_INFO* info_struct, char fill_char, bool respect_border);

#endif