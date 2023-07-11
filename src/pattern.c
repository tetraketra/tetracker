#include "pattern.h"
#include "window.h"
#include <string.h>

void patterns_init(PATTERN patterns[]) {
    char args_template[CHANNEL_ARGS + 1]; // "---... args"
    for (int arg = 0; arg < CHANNEL_ARGS; arg++) {
        args_template[arg] = '-';
    }

    char note_template[3 + 1] = "---";

    for (int pattern = 0; pattern < PATTERNS; pattern++) {
        for (int step = 0; step < STEPS; step++) {
            patterns[pattern].steps[step].step_index = step;
            for (int channel = 0; channel < CHANNELS; channel++) {
                strncpy(patterns[pattern].steps[step].channels[channel].note, note_template, 3);
                strncpy(patterns[pattern].steps[step].channels[channel].args, args_template, CHANNEL_ARGS);
            }
        }
    }
}

void patterns_draw_active(WINDOW* windows[], int win_order_code, PATTERN* patterns, int active_pattern, int step_offset, int channel_offset, int num_channels_can_fit, int max_printable_lines) {
    for (int current_step = step_offset, s_print_offset = 1; current_step < step_offset + max_printable_lines; current_step++, s_print_offset++) {
        char step_str[3 + 1];
        snprintf(step_str, sizeof(step_str), "%03d", patterns[active_pattern].steps[current_step].step_index);

        mvwaddstr(windows[win_order_code], s_print_offset, 2, step_str); // step num

        int c_print_offset = 5;
        for (int current_channel = channel_offset; current_channel < channel_offset + num_channels_can_fit; current_channel++) {
            mvwaddstr(windows[win_order_code], s_print_offset, c_print_offset + 1, patterns[active_pattern].steps[current_step].channels[current_channel].note);
            mvwaddstr(windows[win_order_code], s_print_offset, c_print_offset + 5, patterns[active_pattern].steps[current_step].channels[current_channel].args);
            c_print_offset += CHANNEL_CHAR_WIDTH;
        }
        

    }
}
