#ifndef VULC_SNAKECLI_PLAYER
#define VULC_SNAKECLI_PLAYER

#include "snake.h"

enum player_Direction {
    DIRECTION_UP    = 0,
    DIRECTION_LEFT  = 1,
    DIRECTION_DOWN  = 2,
    DIRECTION_RIGHT = 3,

    DIRECTION_NONE  = 127
};

extern void player_init(ui32 x0, ui32 y0, ui32 len,
                        enum player_Direction dir);
extern void player_destroy(void);

extern void player_tick(void);
extern void player_render(void);

#endif // VULC_SNAKECLI_PLAYER
