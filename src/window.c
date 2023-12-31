#include <ncursesw/curses.h>
#include "window.h"

WINDOW* window_init(WIN_INFO* info_struct) {
    return newwin(info_struct->height, info_struct->width, info_struct->starty, info_struct->startx);
}

void window_fill_with_unicode_four_wide(WINDOW* windows[], int win_order_code, WIN_INFO* info_struct, char fill_char[4], bool respect_border) {
    for (int h = 0; h < info_struct->height; h++) {
        for (int w = 0; w < info_struct->width; w++) {
            if (respect_border && (w == 0 || w == info_struct->width - 1 || h == 0 || h == info_struct->height - 1))
                continue;
            
            mvwaddstr(windows[win_order_code], h, w, fill_char);
        }
    }
}

void window_fill_with_char(WINDOW* windows[], int win_order_code, WIN_INFO* info_struct, char fill_char, bool respect_border) {
    for (int h = 0; h < info_struct->height; h++) {
        for (int w = 0; w < info_struct->width; w++) {
            if (respect_border && (w == 0 || w == info_struct->width - 1 || h == 0 || h == info_struct->height - 1))
                continue;

            mvwaddch(windows[win_order_code], h, w, fill_char);
        }
    }
}

void window_draw_border_to(WINDOW* windows[], int win_order_code, WIN_INFO* info_struct) {
    mvwaddstr(windows[win_order_code], 0, 0, info_struct->border.tl);
    mvwaddstr(windows[win_order_code], 0, info_struct->width - 1, info_struct->border.tr);
    mvwaddstr(windows[win_order_code], info_struct->height - 1, 0, info_struct->border.bl);
    mvwaddstr(windows[win_order_code], info_struct->height - 1, info_struct->width - 1, info_struct->border.br);
    for (int i = 1; i < info_struct->width - 1; i++) { mvwaddstr(windows[win_order_code], 0, i, info_struct->border.ts); };
    for (int i = 1; i < info_struct->width - 1; i++) { mvwaddstr(windows[win_order_code], info_struct->height - 1, i, info_struct->border.bs); }
    for (int i = 1; i < info_struct->height - 1; i++) { mvwaddstr(windows[win_order_code], i, 0, info_struct->border.ls); };
    for (int i = 1; i < info_struct->height - 1; i++) { mvwaddstr(windows[win_order_code], i, info_struct->width - 1, info_struct->border.rs); };
}

void window_init_with_border(WINDOW* windows[], int win_order_code, WIN_INFO* info_struct) {
    windows[win_order_code] = window_init(info_struct);
    if (info_struct->draw_border)
        window_draw_border_to(windows, win_order_code, info_struct);
}

void window_move_and_resize_and_draw_border_to(WINDOW* windows[], int win_order_code, WIN_INFO* info_struct) {
    mvwin(windows[win_order_code], info_struct->starty, info_struct->startx);
    wresize(windows[win_order_code], info_struct->height, info_struct->width);
    if (info_struct->draw_border)
        window_draw_border_to(windows, win_order_code, info_struct);
}

void windows_refresh_all(WINDOW* windows[], int win_count, int active_window, int cursor_y, int cursor_x) {
    refresh(); // ncurses needs this more than I need a life
    for (int i = 0; i < win_count; i++) {
        wrefresh(windows[i]);
    }
    wrefresh(windows[active_window]);
    wmove(windows[active_window], cursor_y, cursor_x);
    
}

void windows_clear_all(WINDOW* windows[], int win_count) {
    for (int i = 0; i < win_count; i++) {
        werase(windows[i]);
    }
}

void windows_draw_borders_to_all(WINDOW* windows[], int win_count, WIN_INFO* info_structs[]) {
    for (int i = 0; i < win_count; i++) {
        if (info_structs[i]->draw_border)
            window_draw_border_to(windows, i, info_structs[i]);
    }
}
