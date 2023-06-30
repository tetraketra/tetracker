#include <ncursesw/curses.h>
#include "pat_info.h"
#include "pat_utils.h"

void pattern_draw(WINDOW* windows[], int win_order_code, PATTERN* pattern) {
    for (int step = 0; step < STEPS; step++) {
        for (int channel = 0; channel < CHANNELS; channel++) {
            for (int i = 0; i < CHANNEL_WIDTH; i++) {
                mvwaddch(windows[win_order_code], \
                    step, channel * CHANNEL_WIDTH + i, \
                    pattern->steps[step].channels[channel].symbols[i]);
            }
        }
    }
}

void pattern_init_notes_pad(PATTERN* pattern) {
    for (int step = 0; step < STEPS; step++) {
        for (int channel = 0; channel < CHANNELS; channel++) {
            for (int i = 0; i < CHANNEL_WIDTH; i++) {
                char char_to_add = '-';
                if (i == 3 || i == CHANNEL_WIDTH - 1) {
                    char_to_add = ' ';   
                }

                pattern->steps[step].channels[channel].symbols[i] = char_to_add;
            }
        }
    }
}