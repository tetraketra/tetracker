#include "win_utils.h"
#include "win_info.h"
#include "ncurses_setup.h"
#include "pattern.h"

#include <time.h>
#include <unistd.h>
#include <stdlib.h>

enum WIN_ORDER {
    WO_BASE,
    WO_OTP,
    _WINDOWS,
}; 

#define SPECIAL_EXIT_KEY 'q'

int main(void) {

    // setup
    const int FPS = 60;
    const int FRAME_TIME_MICROSECONDS = 1000000 / FPS;
    clock_t start_time, end_time;

    WINDOW* windows[_WINDOWS];

    ncurses_setup(FPS);

    int maxlines = LINES - 1;
    int maxcols = COLS - 1;

    // init patterns
    PATTERN* patterns = malloc(sizeof(PATTERN) * PATTERNS);
    patterns_init(patterns);

    // init windows
    WIN_INFO base = {
        .starty = 0, .startx = 0,
        .height = maxlines, .width = maxcols,
        .border = WB_STANDALONE,
        .draw_border = true
    };
    window_init_with_border(windows, WO_BASE, &base);

    WIN_INFO outer_notes_panel = {
        .starty = maxlines / 2, .startx = 0,
        .height = maxlines / 2 + maxlines % 2 + 1, .width = maxcols,
        .border = WB_CONNECTS_UP,
        .draw_border = true
    };
    window_init_with_border(windows, WO_OTP, &outer_notes_panel);

    WIN_INFO step = {
        .starty = 0, .startx = 2,
        .height = 1, .width = maxcols - 2,
        .border = WB_STANDALONE,
        .draw_border = false
    };
    // TODO: pattern draw

    windows_refresh_all(windows, _WINDOWS); // push initial states to screen
    
    // draw loop
    int ch;
    while (ch = getch(), start_time = clock()) {
    
        windows_clear_all(windows, _WINDOWS);

        switch (ch) {
            case SPECIAL_EXIT_KEY: // close
                for (int i = 0; i < _WINDOWS; i++) {
                    delwin(windows[i]);
                }
                endwin();
                exit_curses(0);
                free(patterns);
                return 0;

            case KEY_DOWN ... KEY_RIGHT:            
                break;

            case KEY_RESIZE: // re-init windows
                maxlines = LINES - 1;
                maxcols = COLS - 1;

                base = (WIN_INFO) {
                    .starty = 0, .startx = 0,
                    .height = maxlines, .width = maxcols,
                    .border = WB_STANDALONE,
                    .draw_border = true
                };
                window_move_and_resize(windows, WO_BASE, &base);

                outer_notes_panel = (WIN_INFO) {
                    .starty = maxlines / 2, .startx = 0,
                    .height = maxlines / 2 + maxlines % 2 + 1, .width = maxcols,
                    .border = WB_CONNECTS_UP,
                    .draw_border = true
                };
                window_move_and_resize(windows, WO_OTP, &outer_notes_panel);
                break;
        }

        refresh(); // ncurses needs this more than I need a life
        windows_refresh_all(windows, _WINDOWS);
        

        // sync to FPS
        end_time = clock();
        long sleep_time_microseconds = FRAME_TIME_MICROSECONDS - (end_time - start_time);
        if (sleep_time_microseconds > 0) {
            usleep(sleep_time_microseconds);
        }
    }
}