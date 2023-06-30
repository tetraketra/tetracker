#ifndef PAT_UTILS_H
#define PAT_UTILS_H

#include <ncursesw/curses.h>
#include "pat_info.h"

extern void pattern_draw(WINDOW* windows[], int win_order_code, PATTERN* pattern);
extern void pattern_init_notes_pad(PATTERN* pattern);

#endif