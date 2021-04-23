#ifndef VULC_SNAKECLI_PLAYER
#define VULC_SNAKECLI_PLAYER

#include "snake.h"

#define DIRECTION_UP    ((struct player_Movement) {0 , -1})
#define DIRECTION_LEFT  ((struct player_Movement) {-1,  0})
#define DIRECTION_DOWN  ((struct player_Movement) {0 , +1})
#define DIRECTION_RIGHT ((struct player_Movement) {+1,  0})

#define DIRECTION_NONE  ((struct player_Movement) {0 ,  0})

struct player_Movement {
    i32 xm;
    i32 ym;
};

extern void player_init(ui32 x0, ui32 y0, ui32 size,
                        struct player_Movement dir);
extern void player_destroy(void);

extern void player_tick(void);
extern void player_render(void);

extern bool player_is_tile_free(ui32 x, ui32 y);

#endif // VULC_SNAKECLI_PLAYER
