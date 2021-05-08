#ifndef VULC_SNAKECLI_FOOD
#define VULC_SNAKECLI_FOOD

#include "snake.h"

#define SPECIAL_FOOD_TIME       (TPS * 8)
#define SPECIAL_FOOD_BLINK_TIME (TPS * 3)
#define SPECIAL_FOOD_RARITY     (8)

struct food_Food {
    ui32 x;
    ui32 y;

    bool is_special;
    ui32 special_time_left;
};

extern struct food_Food food;

extern void food_spawn(void);

extern void food_tick(void);
extern void food_render(void);

#endif // VULC_SNAKECLI_FOOD
