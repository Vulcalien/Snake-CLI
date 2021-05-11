#ifndef VULC_SNAKECLI_TERMINAL
#define VULC_SNAKECLI_TERMINAL

#include "snake.h"

extern void terminal_init(void);
extern void terminal_destroy(void);

extern ui32 terminal_width(void);
extern ui32 terminal_height(void);

#endif // VULC_SNAKECLI_TERMINAL
