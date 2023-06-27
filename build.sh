gcc ./src/ncurses_setup.c ./src/win_info.c ./src/ncurses_window_utils.c ./src/main.c -o ./bin/tetracker -W -Werror -Wextra -lncursesw -lpanelw -Wno-multichar -Wall
chmod a+x ./bin/tetracker