#ifndef VULC_SNAKECLI_CORE
#define VULC_SNAKECLI_CORE

#include <stdlib.h>
#include <string.h>

#include "vulcalien/vulcalien_core.h"

// constants
#define TPS 24

// variables
extern ui32 tickCounter;
extern ui32 currentFPS;

extern bool debugMode;

// functions
extern void tick(void);
extern void render(void);

#endif // VULC_SNAKECLI_CORE
