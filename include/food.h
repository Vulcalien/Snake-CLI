#ifndef VULC_SNAKECLI_FOOD
#define VULC_SNAKECLI_FOOD

#include "snake.h"

struct food_Food {
    ui32 x;
    ui32 y;
};

extern struct food_Food food;

extern void food_spawn(void);

extern void food_render(void);

#endif // VULC_SNAKECLI_FOOD
