#include "pattern.h"
#include <string.h>

void patterns_init(PATTERN patterns[]) {
    char args_template[CHANNEL_ARGS]; // "---... args"
    for (int arg; arg < CHANNEL_ARGS; arg++) {
        args_template[arg] = '-';
    }

    char note_template[3] = "---";

    for (int pattern = 0; pattern < PATTERNS; pattern++) {
        for (int step = 0; step < STEPS; step++) {
            patterns[pattern].step_index = step;
            for (int channel = 0; channel < CHANNELS; channel++) {
                strncpy(patterns[pattern].steps[step].channels[channel].note, note_template, 3);
                strncpy(patterns[pattern].steps[step].channels[channel].args, args_template, CHANNEL_ARGS);
            }
        }
    }
}
