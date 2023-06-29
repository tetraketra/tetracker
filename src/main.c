#define WINDOW_COUNT 2
#define SPECIAL_EXIT_KEY 'q'

#include "ncurses_window_utils.h"
#include "win_info.h"
#include "ncurses_setup.h"

#include <time.h>
#include <unistd.h>

int main(void) {
    const int FPS = 60;
    const int FRAME_TIME_MICROSECONDS = 1000000 / FPS;
    clock_t start_time, end_time;

    // ncurses setup
    WINDOW* windows[WINDOW_COUNT];

    ncurses_setup(FPS);

    int maxlines = LINES - 1;
    int maxcols = COLS - 1;

    // window initializations
    enum WIN_ORDER {
        WO_BASE,
        WO_OTP,
    };

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

    for (int i = 0; i < WINDOW_COUNT; i++) {
        wrefresh(windows[i]);
    }

    // draw loop
    int ch;
    while (TRUE) {
        ch = getch();
        start_time = clock();

        // handle input
        switch (ch) {
            case SPECIAL_EXIT_KEY:
                endwin();
                return 0;

            case KEY_RESIZE:
                maxlines = LINES - 1;
                maxcols = COLS - 1;

                for (int i = 0; i < WINDOW_COUNT; i++) {
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

                refresh();

                for (int i = 0; i < WINDOW_COUNT; i++) {
                    wrefresh(windows[i]);
                }
                
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

