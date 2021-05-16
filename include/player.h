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

extern void player_init(u32 x0, u32 y0, u32 size,
                        struct player_Movement dir);
extern void player_destroy(void);

extern void player_tick(void);
extern void player_render(void);

extern bool player_is_tile_free(u32 x, u32 y);

#endif // VULC_SNAKECLI_PLAYER
