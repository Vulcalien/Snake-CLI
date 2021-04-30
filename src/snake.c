/*
 * Copyright 2021 Vulcalien
 * This program is free software. See NOTICE and LICENSE.
 */
#include "snake.h"
#include "gameloop.h"
#include "screen.h"
#include "input.h"
#include "player.h"
#include "food.h"

#include <time.h>

#include <pthread.h>

ui32 tick_counter = 0;
ui32 current_tps = 0;
ui32 current_fps = 0;

ui32 score = 0;

bool is_game_over = false;

int main(int argc, const char *argv[]) {
    // set random seed
    srand(nanotime());

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

    pthread_exit(NULL);
    return 0;
}

void tick(void) {
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

    if(is_game_over) {
        screen_puts(17, 5, "================");
        screen_puts(17, 6, "== GAME  OVER ==");
        screen_puts(17, 7, "================");

        screen_printf(17, 9, "score: %d", score);

        screen_puts(1, SCREEN_HEIGHT - 2, "Made by Vulcalien");
    }

    #ifdef PERFORMANCE_THREAD
        screen_printf(1, 1, "%d tps", current_tps);
        screen_printf(1, 2, "%d fps", current_fps);
    #endif

    screen_render();
}
