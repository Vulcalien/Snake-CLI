/* Copyright 2021 Vulcalien
 * All Rights Reserved
 */

#include "snake.h"
#include "gameloop.h"
#include "screen.h"
#include "player.h"

ui32 tickCounter = 0;
ui32 currentFPS = 0;

bool debugMode = false;

int main(int argc, const char *argv[]) {
    screen_init();
    player_init(10, 10, 2, DIRECTION_LEFT);
    gameloop();
    return 0;
}

void tick(void) {
    player_tick();
}

void render(void) {
    screen_clear('.');
    player_render();

    screen_render();
}
