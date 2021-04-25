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

ui32 tickCounter = 0;
ui32 currentFPS = 0;

ui32 score = 0;

bool is_game_over = false;

int main(int argc, const char *argv[]) {
    #ifdef MALLOC_AND_FREE_TEST
        for(int i = 0; i < 1000000000; i++) {
            screen_init();
            player_init(10, 10, 2, DIRECTION_LEFT);

            screen_destroy();
            player_destroy();
        }
        return 0;
    #endif // MALLOC_AND_FREE_TEST

    // set random seed
    srand(nano_time());

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

    food_render();
    player_render();

    if(is_game_over) {
        screen_puts(17, 5, "================");
        screen_puts(17, 6, "== GAME  OVER ==");
        screen_puts(17, 7, "================");

        screen_printf(17, 9, "score: %d", score);

        screen_puts(1, SCREEN_HEIGHT - 2, "Made by Vulcalien");
    }

    #ifdef DEBUG_MODE
    screen_printf(1, 1, "%d fps", currentFPS);
    #endif

    screen_render();
}

ui64 nano_time(void) {
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    return time.tv_sec * NANOS_IN_SECOND + time.tv_nsec;
}
