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
    #include <unistd.h>
    #define SLEEP(millis) usleep(millis * 1000)
#elif _WIN32
    #include <windows.h>
    #define SLEEP(millis) Sleep(millis)
#endif

extern u64 nanotime(void);

#endif // VULC_SNAKECLI_CORE
