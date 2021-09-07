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
#include "snake.h"

#include "vulcalien/screen.h"
#include "gameloop.h"
#include "input.h"
#include "player.h"
#include "food.h"
#include "highscore.h"

#include <time.h>

#include <pthread.h>

u32 tick_counter = 0;

u32 current_tps = 0;
u32 current_fps = 0;

u32 score = 0;

bool is_game_paused = false;
bool is_game_over = false;

int main(int argc, const char *argv[]) {
    // set random seed
    srand(nanotime());

    screen_create();
    screen_setsize(SCREEN_WIDTH, SCREEN_HEIGHT);

    screen_terminal_prepare();
    screen_ignored_char('`');

    input_init();
    player_init(10, 10, STARTING_SIZE, STARTING_DIRECTION);
    food_spawn();

    input_thread_start();
    gameloop();
    input_thread_stop();

    input_destroy();
    player_destroy();

    screen_destroy();
    screen_terminal_reset();

    pthread_exit(NULL);
    return 0;
}

void tick(void) {
    if(is_game_paused) return;

    player_tick();
    food_tick();

    if(is_game_over) {
        // even if gameloop was stopped
        // it will render one last time
        gameloop_stop();
    }
}

void render(void) {
    screen_clear(' ', NULL);

    #ifdef DRAW_CORNERS
        screen_setchar(0,               0,                ' ', "\033[100m");
        screen_setchar(LEVEL_WIDTH - 1, 0,                ' ', "\033[100m");
        screen_setchar(0,               LEVEL_HEIGHT - 1, ' ', "\033[100m");
        screen_setchar(LEVEL_WIDTH - 1, LEVEL_HEIGHT - 1, ' ', "\033[100m");
    #endif

    food_render();
    player_render();

    screen_printf(0, SCREEN_HEIGHT - 1, NULL, "score: %d", score);

    if(is_game_paused) {
        screen_puts(17, 6,  "##````````````##", NULL);
        screen_puts(17, 7,  "##````````````##", NULL);
        screen_puts(17, 8,  "##````````````##", NULL);
        screen_puts(17, 9,  "##```PAUSED```##", NULL);
        screen_puts(17, 10, "##````````````##", NULL);
        screen_puts(17, 11, "##````````````##", NULL);
        screen_puts(17, 12, "##````````````##", NULL);
    }

    if(is_game_over) {
        screen_puts(17, 4, "================", NULL);
        screen_puts(17, 5, "==`GAME``OVER`==", NULL);
        screen_puts(17, 6, "================", NULL);

        screen_printf(17, 9, NULL, "score: %d", score);

        // highscore
        u32 highscore;
        int err = highscore_get(&highscore);
        if(!err) {
            if(score > highscore) {
                highscore_set(score);

                screen_puts(17, 8, "new highscore!", NULL);
            }
            screen_printf(17, 10, NULL, "highscore: %d", highscore);
        } else {
            highscore_set(score);
        }
        screen_puts(SCREEN_WIDTH - 17, SCREEN_HEIGHT - 1, "Made by Vulcalien", NULL);
    }

    #ifdef PERFORMANCE_THREAD
        screen_printf(1, 1, NULL, "%d tps", current_tps);
        screen_printf(1, 2, NULL, "%d fps", current_fps);
    #endif

    screen_render();
}

// unix_sleep function
#ifdef __unix__
    #include <time.h>
    void unix_sleep(struct timespec t) {
        nanosleep(&t, NULL);
    }
#endif

// nanotime function
#ifdef __unix__
    #include <time.h>

    u64 nanotime(void) {
        struct timespec time;
        clock_gettime(CLOCK_MONOTONIC, &time);
        return time.tv_sec * (1000 * 1000 * 1000) + time.tv_nsec;
    }
#elif _WIN32
    #include <windows.h>

    static u64 frequency = 0;

    u64 nanotime(void) {
        if(frequency == 0) {
            LARGE_INTEGER freq;
            QueryPerformanceFrequency(&freq);
            frequency = freq.QuadPart;

            if(frequency == 0) return 0;
        }
        LARGE_INTEGER counter;
        QueryPerformanceCounter(&counter);
        return counter.QuadPart * (1000 * 1000 * 1000) / frequency;
    }
#endif
