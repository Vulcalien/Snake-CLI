#ifndef VULC_SNAKECLI_CORE
#define VULC_SNAKECLI_CORE

#include <stdlib.h>
#include <string.h>

#include "compile_options.h"
#include "vulcalien/vulcalien_core.h"

// constants
#define TPS (16)
#define NANOS_IN_SECOND (1000000000)

#define STARTING_SIZE 5
#define STARTING_DIRECTION DIRECTION_RIGHT

// variables
extern ui32 tickCounter;
extern ui32 currentFPS;

extern ui32 score;

extern bool is_game_over;

// functions
extern void tick(void);
extern void render(void);

extern ui64 nano_time(void);

#endif // VULC_SNAKECLI_CORE
