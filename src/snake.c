/* Copyright 2021 Vulcalien
 * All Rights Reserved
 */

#include "snake.h"
#include "gameloop.h"
#include "screen.h"
#include "input.h"
#include "player.h"

ui32 tickCounter = 0;
ui32 currentFPS = 0;

#include <termios.h>
#include <unistd.h>

int main(int argc, const char *argv[]) {
    #ifdef MALLOC_AND_FREE_TEST
        for(int i = 0; i < 1000000000; i++) {
            screen_init();
            player_init(10, 10, 2, DIRECTION_LEFT);

            screen_destroy();
            player_destroy();
        }
        return 0;
    #endif // MALLOC_AND_FREE_TEST

    input_init();
    screen_init();
    player_init(10, 10, 2, DIRECTION_LEFT);

    gameloop();

    input_destroy();
    screen_destroy();
    player_destroy();
    return 0;
}

void tick(void) {
    player_tick();
    input_tick();
}

void render(void) {
    screen_clear(' ');
    player_render();

    #ifdef DEBUG_MODE
    screen_printf(1, 1, "%d fps", currentFPS);
    #endif

    screen_render();
}
