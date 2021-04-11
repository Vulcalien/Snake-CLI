#include "snake.h"
#include "gameloop.h"
#include "screen.h"

#include <time.h>

// platform-dependent
#include <unistd.h>

#define NANOS_IN_SECOND     1000000000
#define SLEEP_MICROSECONDS  4 * 1000

static void loop_tick(void);
static void loop_render(void);

static ui64 nano_time(void);

static bool running = false;

void gameloop(void) {
    ui32 frames = 0;

    ui64 timePerTick = NANOS_IN_SECOND / TPS;
    ui64 lastTime = nano_time();
    ui64 unprocessedTime = timePerTick;

    running = true;
    while(running) {
        ui64 now = nano_time();
        ui64 passedTime = now - lastTime;
        lastTime = now;

        if(passedTime > NANOS_IN_SECOND) passedTime = NANOS_IN_SECOND;

        unprocessedTime += passedTime;

        bool ticked = false;
        while(unprocessedTime >= timePerTick) {
            unprocessedTime -= timePerTick;

            loop_tick();
            tickCounter++;
            ticked = true;

            if(tickCounter % TPS == 0) {
                currentFPS = frames;
                frames = 0;
            }
        }
        if(ticked) {
            loop_render();
            frames++;
        }

        // this is not cross-platform
        usleep(SLEEP_MICROSECONDS);
    }
}

void gameloop_stop(void) {
    running = false;
}

static void loop_tick(void) {
    tick();
}

static void loop_render(void) {
    render();
}

static ui64 nano_time(void) {
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    return time.tv_sec * NANOS_IN_SECOND + time.tv_nsec;
}
