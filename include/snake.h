#ifndef VULC_SNAKECLI_CORE
#define VULC_SNAKECLI_CORE

#if !(defined(__unix__) || defined(_WIN32))
    #error The system is not supported
#endif

#include <stdlib.h>
#include <string.h>

#include "compile_options.h"
#include "vulcalien-core/vulcalien.h"

#define TPS (16)
#define NANOS_IN_SECOND (1000000000)

#define STARTING_SIZE 5
#define STARTING_DIRECTION DIRECTION_RIGHT

extern ui32 tick_counter;

extern ui32 current_tps;
extern ui32 current_fps;

extern ui32 score;

extern bool is_game_paused;
extern bool is_game_over;

extern void tick(void);
extern void render(void);

#endif // VULC_SNAKECLI_CORE
