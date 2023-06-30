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
    _PADS_START,
    WO_NOTES_PAD,
    WO_STEP_COUNT_PAD,
    _PADS_END,
    _WINDOWS,
}; 

#define SPECIAL_EXIT_KEY 'q'

void refresh_all(
    WINDOW* windows[], 
    int maxlines, int maxcols, 
    int pds_y, int pds_x,
    int usercursor_window, 
    int usercursor_y_on_window, int usercursor_x_on_window) {
    
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

        wmove(windows[usercursor_window], usercursor_y_on_window, usercursor_x_on_window);
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

    int usercursor_y_on_window = 0;
    int usercursor_x_on_window = 0;
    int usercursor_window = WO_NOTES_PAD;

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
    refresh_all(
        windows, 
        maxlines, maxcols, 
        pad_display_start_y, pad_display_start_x,
        usercursor_window, 
        usercursor_y_on_window, usercursor_x_on_window);

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

            case KEY_DOWN ... KEY_RIGHT:
                switch (ch) {
                    case KEY_DOWN:
                        if (usercursor_window == WO_NOTES_PAD) {
                            if (usercursor_y_on_window + 1 < STEPS - 1)
                                usercursor_y_on_window++;  
                            if (usercursor_y_on_window > pad_display_start_y + maxlines / 2 + maxlines % 2 - 2)
                                pad_display_start_y++;
                        }
                        
                        break;

                    case KEY_UP:
                        if (usercursor_window == WO_NOTES_PAD) { 
                            if (usercursor_y_on_window > 0) 
                                usercursor_y_on_window--;   
                            if (pad_display_start_y > usercursor_y_on_window)
                                pad_display_start_y--;
                        }

                        break;
                    case KEY_LEFT:
                        usercursor_x_on_window--;
                        break;
                    case KEY_RIGHT:
                        usercursor_x_on_window++;
                        break;
                }

                refresh_all(
                    windows, 
                    maxlines, maxcols, 
                    pad_display_start_y, pad_display_start_x,
                    usercursor_window, 
                    usercursor_y_on_window, usercursor_x_on_window);
                break;

            case KEY_RESIZE: // re-init windows
                maxlines = LINES - 1;
                maxcols = COLS - 1;

                for (int i = 0; i < _WINDOWS; i++) {
                    if (i < _PADS_START || i > _PADS_END) {
                        werase(windows[i]);
                    }
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

                usercursor_window = WO_NOTES_PAD;
                usercursor_y_on_window = 0;
                usercursor_x_on_window = 0;

                refresh_all(
                    windows, 
                    maxlines, maxcols, 
                    pad_display_start_y, pad_display_start_x,
                    usercursor_window, 
                    usercursor_y_on_window, usercursor_x_on_window);
                break;

                
        }

        // sync to FPS
        end_time = clock();
        long sleep_time_microseconds = FRAME_TIME_MICROSECONDS - (end_time - start_time);
        if (sleep_time_microseconds > 0) {
            usleep(sleep_time_microseconds);
        }

        // just in case
        // wmove(windows[usercursor_window], usercursor_y_on_window, usercursor_x_on_window);
    }
}