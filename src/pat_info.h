#ifndef PAT_INFO_H
#define PAT_INFO_H

#define CHANNELS 64
#define STEPS 129
#define STEPS_PLAYABLE (STEPS - 1) // dumb hack
#define PATTERNS 256
#define CHANNEL_ARGS 4 // can change!
#define CHANNEL_WIDTH (3 + 1 + CHANNEL_ARGS + 1) // +1 for whitespace

typedef struct {
    char symbols[CHANNEL_WIDTH];
} CHANNEL;

typedef struct {
    CHANNEL channels[CHANNELS];
} STEP;

typedef struct {
    STEP steps[STEPS];
} PATTERN;

#endif