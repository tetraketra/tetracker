#ifndef PATTERN_H
#define PATTERN_H

#include "window.h"

#define CHANNEL_ARGS 4
#define CHANNEL_CHAR_WIDTH (3 + 1 + CHANNEL_ARGS + 1)
#define CHANNELS 32
#define STEPS 128
#define PATTERNS 256

typedef struct CHANNEL {
    char note[3 + 1];
    char args[CHANNEL_ARGS + 1];
} CHANNEL;

typedef struct STEP {
    int step_index;
    CHANNEL channels[CHANNELS];
} STEP;

typedef struct PATTERN {
    STEP steps[STEPS];
} PATTERN;

void patterns_init(PATTERN patterns[]);
void patterns_draw_active(WINDOW* windows[], int win_order_code, PATTERN* patterns, int active_pattern, int step_offset, int channel_offset, int num_channels_can_fit, int max_printable_lines);

#endif