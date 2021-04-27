/*
 * Copyright 2021 Vulcalien
 * This program is free software. See NOTICE and LICENSE.
 */
#include "gameloop.h"
#include "screen.h"

#include "vulcalien-core/vulcalien.h"

#ifdef PERFORMANCE_THREAD
    #include "pthread.h"
#endif

#define SLEEP_MILLISECONDS  (4)
#define NANOS_PER_TICK      (NANOS_IN_SECOND / TPS)

static bool running = false;

#ifdef PERFORMANCE_THREAD
    static ui32 counter_ticks = 0;
    static ui32 counter_frames = 0;

    static pthread_t performance_thread;

    static void *tps_counter(void *arg);
#endif

void gameloop(void) {
    #ifdef PERFORMANCE_THREAD
        pthread_create(&performance_thread, NULL, tps_counter, NULL);
    #endif

    ui64 lastTime = nano_time();
    ui64 unprocessedTime = NANOS_PER_TICK;

    running = true;
    while(running) {
        ui64 now = nano_time();
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
