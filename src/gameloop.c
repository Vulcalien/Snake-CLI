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
#include "gameloop.h"

#ifdef PERFORMANCE_THREAD
    #include <pthread.h>
#endif

#define SLEEP_MILLISECONDS  (4)
#define NANOS_PER_TICK      (NANOS_IN_SECOND / TPS)

static bool running = false;

#ifdef PERFORMANCE_THREAD
    static u32 counter_ticks = 0;
    static u32 counter_frames = 0;

    static pthread_t performance_thread;

    static void *tps_counter(void *arg);
#endif

void gameloop(void) {
    #ifdef PERFORMANCE_THREAD
        pthread_create(&performance_thread, NULL, tps_counter, NULL);
    #endif

    u64 lastTime = nanotime();
    u64 unprocessedTime = NANOS_PER_TICK;

    running = true;
    while(running) {
        u64 now = nanotime();
        i64 passedTime = now - lastTime;
        lastTime = now;

        if(passedTime < 0) passedTime = 0;
        if(passedTime > NANOS_IN_SECOND) passedTime = NANOS_IN_SECOND;

        unprocessedTime += passedTime;

        bool ticked = false;
        while(unprocessedTime >= NANOS_PER_TICK) {
            unprocessedTime -= NANOS_PER_TICK;

            tick();
            tick_counter++;
            ticked = true;

            #ifdef PERFORMANCE_THREAD
                counter_ticks++;
            #endif
        }
        if(ticked) {
            render();
            #ifdef PERFORMANCE_THREAD
                counter_frames++;
            #endif
        }
        #ifdef SLEEP_BETWEEN_TICKS
            SLEEP(SLEEP_MILLISECONDS);
        #endif
    }
}

void gameloop_stop(void) {
    running = false;
    #ifdef PERFORMANCE_THREAD
        pthread_cancel(performance_thread);
    #endif
}

#ifdef PERFORMANCE_THREAD
    static void *tps_counter(void *arg) {
        while(true) {
            current_tps = counter_ticks;
            current_fps = counter_frames;

            counter_ticks = 0;
            counter_frames = 0;
            SLEEP(1000);
        }
        return NULL;
    }
#endif
