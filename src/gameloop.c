/*
 * Copyright 2021 Vulcalien
 * This program is free software. See NOTICE and LICENSE.
 */
#include "gameloop.h"
#include "screen.h"

// platform-dependent
#ifdef __unix__
    #include <unistd.h>
#elif _WIN32
    #include <windows.h>
#endif

#define SLEEP_MILLISECONDS  (4)
#define SLEEP_MICROSECONDS  (SLEEP_MILLISECONDS * 1000)
#define NANOS_PER_TICK      (NANOS_IN_SECOND / TPS)

static bool running = false;

void gameloop(void) {
    ui32 frames = 0;

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
            tickCounter++;
            ticked = true;

            if(tickCounter % TPS == 0) {
                currentFPS = frames;
                frames = 0;
            }
        }
        if(ticked) {
            render();
            frames++;
        }

        // platform-dependent
        #ifdef __unix__
            usleep(SLEEP_MICROSECONDS);
        #elif _WIN32
            Sleep(SLEEP_MILLISECONDS);
        #endif
    }
}

void gameloop_stop(void) {
    running = false;
}
