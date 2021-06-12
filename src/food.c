/* Copyright 2021 Vulcalien
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 2 only.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
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

void food_render(void) {
    if(food.is_special) {
        if(food.special_time_left > SPECIAL_FOOD_BLINK_TIME
           || food.special_time_left / 3 % 2 == 0) {
            char *col = tick_counter / 6 % 2 == 0 ? "\033[1;93m"
                                                  : "\033[1;94m";

            screen_setchar(scr, food.x, food.y, '&', col);
        }
    } else {
        screen_setchar(scr, food.x, food.y, '$', "\033[1;31m");
    }
}
