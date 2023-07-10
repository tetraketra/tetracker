#ifndef NCURSES_WINDOW_UTILS_H
#define NCURSES_WINDOW_UTILS_H

#include <ncursesw/curses.h>
#include "win_info.h"

WINDOW* window_init(WIN_INFO* info_struct);
void window_draw_border_to(WINDOW* windows[], int win_order_code, WIN_INFO* info_struct);
void window_init_with_border(WINDOW* windows[], int win_order_code, WIN_INFO* info_struct);
void window_move_and_resize(WINDOW* windows[], int win_order_code, WIN_INFO* info_struct);
void windows_refresh_all(WINDOW* windows[], int win_count);
void windows_clear_all(WINDOW* windows[], int win_count);
void windows_draw_borders_to_all(WINDOW* windows[], int win_count, WIN_INFO* info_structs[]);

#endif