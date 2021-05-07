/*
 * Copyright 2021 Vulcalien
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
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
#include "snake.h"

#include "gameloop.h"
#include "screen.h"
#include "input.h"
#include "player.h"
#include "food.h"
#include "terminal.h"
#include "highscore.h"

#include <time.h>

#include <pthread.h>

ui32 tick_counter = 0;

ui32 current_tps = 0;
ui32 current_fps = 0;

ui32 score = 0;

bool is_game_paused = false;
bool is_game_over = false;

int main(int argc, const char *argv[]) {
    // set random seed
    srand(nanotime());

    terminal_init();
    input_init();
    screen_init();
    player_init(10, 10, STARTING_SIZE, STARTING_DIRECTION);
    food_spawn();

    input_thread_start();
    gameloop();
    input_thread_stop();

    input_destroy();
    screen_destroy();
    player_destroy();
    terminal_destroy();

    pthread_exit(NULL);
    return 0;
}

void tick(void) {
    if(is_game_paused) return;

    player_tick();

    if(is_game_over) {
        // even if gameloop was stopped
        // it will render one last time
        gameloop_stop();
    }
}

void render(void) {
    screen_clear(' ');

    #ifdef DRAW_CORNERS
        screen_setchar(0,                0,                 SCREEN_BORDER);
        screen_setchar(SCREEN_WIDTH - 1, 0,                 SCREEN_BORDER);
        screen_setchar(0,                SCREEN_HEIGHT - 1, SCREEN_BORDER);
        screen_setchar(SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, SCREEN_BORDER);
    #endif

    food_render();
    player_render();

    if(is_game_paused) {
        screen_puts(17, 6,  "##````````````##");
        screen_puts(17, 7,  "##````````````##");
        screen_puts(17, 8,  "##````````````##");
        screen_puts(17, 9,  "##```PAUSED```##");
        screen_puts(17, 10, "##````````````##");
        screen_puts(17, 11, "##````````````##");
        screen_puts(17, 12, "##````````````##");
    }

    if(is_game_over) {
        screen_puts(17, 4, "================");
        screen_puts(17, 5, "==`GAME``OVER`==");
        screen_puts(17, 6, "================");

        screen_printf(17, 9, "score: %d", score);

        // highscore
        ui32 highscore;
        int err = highscore_get(&highscore);
        if(!err) {
            if(score > highscore) {
                highscore_set(score);

                screen_puts(17, 8, "new highscore!");
            }
            screen_printf(17, 10, "highscore: %d", highscore);
        } else {
            highscore_set(score);
        }
        screen_puts(1, SCREEN_HEIGHT - 2, "Made by Vulcalien");
    }

    #ifdef PERFORMANCE_THREAD
        screen_printf(1, 1, "%d tps", current_tps);
        screen_printf(1, 2, "%d fps", current_fps);
    #endif

    screen_render();
}
