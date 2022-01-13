/* Copyright 2021 Vulcalien
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#ifndef VULC_SNAKECLI_CORE
#define VULC_SNAKECLI_CORE

#if !(defined(__unix__) || defined(_WIN32))
    #error The system is not supported
#endif

#include <stdlib.h>
#include <string.h>

#include "vulcalien/vulcalien.h"
#include "compile_options.h"
#include "level.h"

#define TPS (16)
#define NANOS_IN_SECOND (1000000000)

#define SCREEN_WIDTH  (LEVEL_WIDTH)
#define SCREEN_HEIGHT (LEVEL_HEIGHT + 1)

#define STARTING_SIZE 5
#define STARTING_DIRECTION DIRECTION_RIGHT

extern u32 tick_counter;

extern u32 current_tps;
extern u32 current_fps;

extern u32 score;

extern bool is_game_paused;
extern bool is_game_over;

extern void tick(void);
extern void render(void);

// SLEEP macro
#ifdef __unix__
    #include <time.h>
    extern void unix_sleep(struct timespec t);
    #define SLEEP(millis)\
        unix_sleep((struct timespec) {\
            .tv_sec  = millis / 1000,\
            .tv_nsec = (millis % 1000) * 1000 * 1000\
        })
#elif _WIN32
    #include <windows.h>
    #define SLEEP(millis) Sleep(millis)
#endif

extern u64 nanotime(void);

#endif // VULC_SNAKECLI_CORE
