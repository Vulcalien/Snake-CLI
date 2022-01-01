/* Copyright 2021-2022 Vulcalien
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 only.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "food.h"

#include "vulcalien/screen.h"
#include "level.h"
#include "player.h"

struct food_Food food;

void food_spawn(void) {
    while(true) {
        u32 x = rand() % LEVEL_WIDTH;
        u32 y = rand() % LEVEL_HEIGHT;

        if(player_is_tile_free(x, y)) {
            food = (struct food_Food) {
                .x = x,
                .y = y,

                .is_special = rand() % SPECIAL_FOOD_RARITY == 0
            };
            if(food.is_special) {
                food.special_time_left = SPECIAL_FOOD_TIME;
            }
            break;
        }
    }
}

void food_tick(void) {
    if(!food.is_special) return;
    food.special_time_left--;
    if(food.special_time_left == 0) {
        food_spawn();
    }
}

static inline void render_food(char food_char, const char *food_color) {
    #ifdef FOLLOW_HEAD
        i32 diff_x = food.x - player.head.x;
        if(diff_x >= +LEVEL_WIDTH / 2)
            diff_x -= LEVEL_WIDTH;
        if(diff_x < -LEVEL_WIDTH / 2)
            diff_x += LEVEL_WIDTH;

        i32 diff_y = food.y - player.head.y;
        if(diff_y >= +LEVEL_HEIGHT / 2)
            diff_y -= LEVEL_HEIGHT;
        if(diff_y < -LEVEL_HEIGHT / 2)
            diff_y += LEVEL_HEIGHT;

        #define FOOD_RENDER_X (LEVEL_WIDTH / 2 + diff_x)
        #define FOOD_RENDER_Y (LEVEL_HEIGHT / 2 + diff_y)
    #else
        #define FOOD_RENDER_X (food.x)
        #define FOOD_RENDER_Y (food.y)
    #endif

    screen_setchar(FOOD_RENDER_X, FOOD_RENDER_Y, food_char, food_color);

    #undef FOOD_RENDER_X
    #undef FOOD_RENDER_Y
}

void food_render(void) {
    if(food.is_special) {
        if(food.special_time_left > SPECIAL_FOOD_BLINK_TIME
           || food.special_time_left / 3 % 2 == 0) {
            char *col = tick_counter / 6 % 2 == 0 ? "\033[1;93m"
                                                  : "\033[1;94m";

            render_food('&', col);
        }
    } else {
        render_food('$', "\033[1;31m");
    }
}
