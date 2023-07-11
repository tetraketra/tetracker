#include "ncurses_setup.h"
#include "window.h"
#include "pattern.h"

#include <time.h>
#include <unistd.h>
#include <stdlib.h>

#define SPECIAL_EXIT_KEY 'q'

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })
#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

enum WIN_ORDER {
    WO_BASE,
    WO_ONP,
    WO_ONP_RF,
    _WINDOWS,
}; 

int main(void) {

    // setup
    const int FPS = 10000;
    const int FRAME_TIME_MICROSECONDS = 1000000 / FPS;
    clock_t start_time, end_time;

    WINDOW* windows[_WINDOWS];

    ncurses_setup(FPS);

    int maxlines = LINES - 1;
    int maxcols = COLS;

    // init patterns
    int active_pattern = 0;
    int step_offset = 0;
    int channel_offset = 0;
    int channels_can_fit = min(CHANNELS, (maxcols - 2 - 5)/CHANNEL_CHAR_WIDTH);

    PATTERN* patterns = calloc(PATTERNS, sizeof(PATTERN));
    patterns_init(patterns);

    // init windows
    WIN_INFO base = {
        .starty = 0, .startx = 0,
        .height = maxlines, .width = maxcols,
        .border = (WIN_BORDER) {.ls = "║", .rs = "║", .ts = "═", .bs = "═", .tl = "╔", .tr = "╗", .bl = "╚", .br = "╝"},
        .draw_border = true
    };

    WIN_INFO outer_notes_panel = {
        .starty = maxlines / 2, .startx = 0,
        .height = maxlines / 2 + maxlines % 2 + 1, .width = maxcols,
        .border = (WIN_BORDER) {.ls = "║", .rs = "║", .ts = "═", .bs = "═", .tl = "╠", .tr = "╣", .bl = "╚", .br = "╝"},
        .draw_border = true
    };

    int onprf_startx = channels_can_fit*CHANNEL_CHAR_WIDTH + 6;
    WIN_INFO onp_right_filler = {
        .starty = maxlines / 2, .startx = onprf_startx,
        .height = maxlines / 2 + maxlines % 2 + 1, .width = maxcols - onprf_startx,
        .border = (WIN_BORDER) {.ls = "║", .rs = "║", .ts = "═", .bs = "═", .tl = "╦", .tr = "╣", .bl = "╩", .br = "╝"},
        .draw_border = true
    };

    // all the draw calls // TODO make macro ?
    window_init_with_border(windows, WO_BASE, &base);
    window_init_with_border(windows, WO_ONP, &outer_notes_panel);
    window_init_with_border(windows, WO_ONP_RF, &onp_right_filler);
    patterns_draw_active(windows, WO_ONP, patterns, active_pattern, step_offset, channel_offset, (maxcols - 2 - 5)/CHANNEL_CHAR_WIDTH, maxlines / 2 + maxlines % 2 - 1);
    // all the draw calls
    
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
                maxcols = COLS;
                channels_can_fit = min(CHANNELS, (maxcols - 2 - 5)/CHANNEL_CHAR_WIDTH);

                base = (WIN_INFO) {
                    .starty = 0, .startx = 0,
                    .height = maxlines, .width = maxcols,
                    .border = (WIN_BORDER) {.ls = "║", .rs = "║", .ts = "═", .bs = "═", .tl = "╔", .tr = "╗", .bl = "╚", .br = "╝"},
                    .draw_border = true
                };
                
                outer_notes_panel = (WIN_INFO) {
                    .starty = maxlines / 2, .startx = 0,
                    .height = maxlines / 2 + maxlines % 2 + 1, .width = maxcols,
                    .border = (WIN_BORDER) {.ls = "║", .rs = "║", .ts = "═", .bs = "═", .tl = "╠", .tr = "╣", .bl = "╚", .br = "╝"},
                    .draw_border = true
                };

                int onprf_startx = channels_can_fit*CHANNEL_CHAR_WIDTH + 6;
                WIN_INFO onp_right_filler = {
                    .starty = maxlines / 2, .startx = onprf_startx,
                    .height = maxlines / 2 + maxlines % 2 + 1, .width = maxcols - onprf_startx,
                    .border = (WIN_BORDER) {.ls = "║", .rs = "║", .ts = "═", .bs = "═", .tl = "╦", .tr = "╣", .bl = "╩", .br = "╝"},
                    .draw_border = true
                };

                // all the draw calls // TODO make macro ?
                window_move_and_resize_and_draw_border_to(windows, WO_BASE, &base);
                window_move_and_resize_and_draw_border_to(windows, WO_ONP, &outer_notes_panel);
                window_move_and_resize_and_draw_border_to(windows, WO_ONP_RF, &onp_right_filler);
                patterns_draw_active(windows, WO_ONP, patterns, active_pattern, step_offset, channel_offset, channels_can_fit, maxlines / 2 + maxlines % 2 - 1);
                // all the draw calls

                break;
        }

        windows_refresh_all(windows, _WINDOWS);

        // sync to FPS
        end_time = clock();
        long sleep_time_microseconds = FRAME_TIME_MICROSECONDS - (end_time - start_time);
        if (sleep_time_microseconds > 0) {
            usleep(sleep_time_microseconds);
        }
    }
}