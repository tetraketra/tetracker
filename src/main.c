#include "win_utils.h"
#include "win_info.h"
#include "pat_info.h"
#include "pat_utils.h"
#include "ncurses_setup.h"

#include <time.h>
#include <unistd.h>

enum WIN_ORDER {
    WO_BASE,
    WO_OTP,
    WO_NOTES_PAD,
    WO_STEP_COUNT_PAD,
    _WINDOWS,
}; 

#define SPECIAL_EXIT_KEY 'q'

void refresh_all(WINDOW* windows[], int maxlines, int maxcols, int pds_y, int pds_x) {
    for (int i = 0; i < _WINDOWS; i++) {
        if (i == WO_STEP_COUNT_PAD) {
            prefresh(windows[i], \
                pds_y, 0, \
                maxlines / 2 + 1, 1, \
                maxlines - 1, 5);
        }
        
        if (i == WO_NOTES_PAD) {
            prefresh(windows[i], \
                pds_y, pds_x, \
                maxlines / 2 + 1, 5, \
                maxlines - 1, maxcols - 2);

        } else {
            wrefresh(windows[i]);
        }
    }
}

int main(void) {

    // setup
    const int FPS = 60;
    const int FRAME_TIME_MICROSECONDS = 1000000 / FPS;
    clock_t start_time, end_time;

    WINDOW* windows[_WINDOWS];

    ncurses_setup(FPS);

    int maxlines = LINES - 1;
    int maxcols = COLS - 1;

    PATTERN pattern;
    pattern_init_notes_pad(&pattern);

    // init windows
    WIN_INFO base = {
        .starty = 0, .startx = 0,
        .height = maxlines, .width = maxcols,
        .border = WB_STANDALONE
    };
    window_init_with_border(windows, WO_BASE, &base);

    WIN_INFO outer_notes_panel = {
        .starty = maxlines / 2, .startx = 0,
        .height = maxlines / 2 + maxlines % 2 + 1, .width = maxcols,
        .border = WB_CONNECTS_UP
    };
    window_init_with_border(windows, WO_OTP, &outer_notes_panel);

    // init pad(s)
    int pad_display_start_y = 0, pad_display_start_x = 0;
    windows[WO_NOTES_PAD] = newpad(STEPS, CHANNELS * CHANNEL_WIDTH);
    pattern_draw(windows, WO_NOTES_PAD, &pattern);
    
    windows[WO_STEP_COUNT_PAD] = newpad(STEPS, 3);
    for (int step = 0; step < STEPS; step++) {
        char str[12];
        snprintf(str, sizeof(str), "%03d", step);
        mvwaddnstr(windows[WO_STEP_COUNT_PAD], step, 0, str, 3);
    }
    
    // push initial states to screen
    refresh_all(windows, maxlines, maxcols, pad_display_start_y, pad_display_start_x);

    // draw loop
    int ch;
    while (TRUE) {
        ch = getch();
        start_time = clock();

        switch (ch) {
            case SPECIAL_EXIT_KEY: // close
                for (int i = 0; i < _WINDOWS; i++) {
                    delwin(windows[i]);
                }
                endwin();
                exit_curses(0);
                return 0;

            case KEY_RESIZE: // re-init windows
                maxlines = LINES - 1;
                maxcols = COLS - 1;

                for (int i = 0; i < _WINDOWS; i++) {
                    werase(windows[i]);
                }

                base = (WIN_INFO) {
                    .starty = 0, .startx = 0,
                    .height = maxlines, .width = maxcols,
                    .border = WB_STANDALONE
                };
                window_move_and_resize(windows, WO_BASE, &base);

                outer_notes_panel = (WIN_INFO) {
                    .starty = maxlines / 2, .startx = 0,
                    .height = maxlines / 2 + maxlines % 2 + 1, .width = maxcols,
                    .border = WB_CONNECTS_UP
                };
                window_move_and_resize(windows, WO_OTP, &outer_notes_panel);

                pattern_draw(windows, WO_NOTES_PAD, &pattern);

                refresh_all(windows, maxlines, maxcols, pad_display_start_y, pad_display_start_x);
                
                break;
        }

        // sync to FPS
        end_time = clock();
        long sleep_time_microseconds = FRAME_TIME_MICROSECONDS - (end_time - start_time);
        if (sleep_time_microseconds > 0) {
            usleep(sleep_time_microseconds);
        }
    }
}