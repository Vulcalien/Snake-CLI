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
#include "input.h"

#include "gameloop.h"

#include <stdio.h>
#include <signal.h>

#include <pthread.h>

static void *receive_inputs(void *arg);

static void receive_signal_interrupt(int x);

struct player_Movement input_dir;

static pthread_t thread;

void input_init(void) {
    input_dir = DIRECTION_NONE;

    signal(SIGINT, receive_signal_interrupt);
}

void input_destroy(void) {
}

void input_thread_start(void) {
    pthread_create(&thread, NULL, receive_inputs, NULL);
}

void input_thread_stop(void) {
    pthread_cancel(thread);
}

static void *receive_inputs(void *arg) {
    u32 escape_found = 0;
    while(true) {
        char c = getchar();

        // if c is pause or enter
        if(c == ' ' || c == '\n') {
            is_game_paused = !is_game_paused;
            continue;
        }

        // do not process any input if paused
        if(is_game_paused) continue;

        if(c == '\033') {
            escape_found = 1;
            continue;
        }

        // "\033" was found, now check if [ is also there
        if(escape_found == 1) {
            if(c == '[') {
                escape_found = 2;
                continue;
            } else {
                escape_found = 0;
            }
        }

        if(escape_found == 2) {
            if(c == 'A') input_dir = DIRECTION_UP;
            if(c == 'B') input_dir = DIRECTION_DOWN;
            if(c == 'C') input_dir = DIRECTION_RIGHT;
            if(c == 'D') input_dir = DIRECTION_LEFT;

            escape_found = 0;
        } else {
            if(c == 'w' || c == 'W') input_dir = DIRECTION_UP;
            if(c == 'a' || c == 'A') input_dir = DIRECTION_LEFT;
            if(c == 's' || c == 'S') input_dir = DIRECTION_DOWN;
            if(c == 'd' || c == 'D') input_dir = DIRECTION_RIGHT;
        }
    }
    return NULL;
}

static void receive_signal_interrupt(int x) {
    gameloop_stop();
}
