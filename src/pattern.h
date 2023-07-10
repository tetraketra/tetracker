#ifndef PATTERN_H
#define PATTERN_H

#define CHANNEL_ARGS 4
#define CHANNELS 16
#define STEPS 128
#define PATTERNS 256

typedef struct CHANNEL {
    char note[3];
    char args[CHANNEL_ARGS];
} CHANNEL;

typedef struct STEP {
    CHANNEL channels[CHANNELS];
} STEP;

typedef struct PATTERN {
    int step_index;
    STEP steps[STEPS];
} PATTERN;

void patterns_init(PATTERN patterns[]);

#endif