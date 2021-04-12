#ifndef VULC_SNAKECLI_SCREEN
#define VULC_SNAKECLI_SCREEN

#include "snake.h"

#include <stdio.h>

#define SCREEN_WIDTH  (50)
#define SCREEN_HEIGHT (20)

extern void screen_init(void);
extern void screen_render(void);

extern void screen_clear(char c);
extern bool screen_setchar(ui32 x, ui32 y, char c);
extern void screen_printf(ui32 x, ui32 y, const char *format, ...);

#endif // VULC_SNAKECLI_SCREEN
