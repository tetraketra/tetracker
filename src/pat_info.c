#define CHANNELS 64
#define STEPS 128
#define PATTERNS 256
#define CHANNEL_ARGS 4
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