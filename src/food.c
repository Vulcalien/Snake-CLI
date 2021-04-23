#include "food.h"
#include "screen.h"
#include "player.h"

struct food_Food food;

void food_spawn(void) {
    while(true) {
        ui32 x = rand() % SCREEN_WIDTH;
        ui32 y = rand() % SCREEN_HEIGHT;

        if(player_is_tile_free(x, y)) {
            food.x = x;
            food.y = y;

            return;
        }
    }
}

void food_render(void) {
    screen_setchar(food.x, food.y, '$');
}
