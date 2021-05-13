/* Copyright 2021 Vulcalien
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
#ifndef VULC_SNAKECLI_CORE
#define VULC_SNAKECLI_CORE

#if !(defined(__unix__) || defined(_WIN32))
    #error The system is not supported
#endif

#include <stdlib.h>
#include <string.h>

#include "compile_options.h"

// --- Data Types ---
#include <stdbool.h>
#include <stdint.h>

typedef int8_t   i8;    // signed 8 bit integer
typedef uint8_t  ui8;   // unsigned 8 bit integer

typedef int16_t  i16;   // signed 16 bit integer
typedef uint16_t ui16;  // unsigned 16 bit integer

typedef int32_t  i32;   // signed 32 bit integer
typedef uint32_t ui32;  // unsigned 32 bit integer

typedef int64_t  i64;   // signed 64 bit integer
typedef uint64_t ui64;  // unsigned 64 bit integer
// --- End of Data Types ---

#define TPS (16)
#define NANOS_IN_SECOND (1000000000)

#define STARTING_SIZE 5
#define STARTING_DIRECTION DIRECTION_RIGHT

extern ui32 tick_counter;

extern ui32 current_tps;
extern ui32 current_fps;

extern ui32 score;

extern bool is_game_paused;
extern bool is_game_over;

extern void tick(void);
extern void render(void);

#include <stdbool.h>
#include <stdint.h>

// SLEEP macro
#ifdef __unix__
    #include <unistd.h>
    #define SLEEP(millis) usleep(millis * 1000)
#elif _WIN32
    #include <windows.h>
    #define SLEEP(millis) Sleep(millis)
#endif

extern ui64 nanotime(void);

#endif // VULC_SNAKECLI_CORE
