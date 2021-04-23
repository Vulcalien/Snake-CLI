#include "gameloop.h"
#include "screen.h"

// POSIX
#include <unistd.h>

#define SLEEP_MICROSECONDS  (4 * 1000)
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
        usleep(SLEEP_MICROSECONDS);
    }
}

void gameloop_stop(void) {
    running = false;
}
