#include "input.h"
#include "gameloop.h"

#include <stdio.h>
#include <signal.h>

// POSIX
#include <unistd.h>
#include <termios.h>

static void setBufferedInput(bool enable);
static void receive_signal_interrupt(int x);

void input_init(void) {
    signal(SIGINT, receive_signal_interrupt);
    setBufferedInput(false);
}

void input_destroy(void) {
    setBufferedInput(true);
}

void input_tick(void) {

}

static void receive_signal_interrupt(int x) {
    gameloop_stop();
}

/* copied from https://github.com/mevdschee/2048.c
 *
 * May not be an exact copy of the original code
 *
 * original copyright (MIT License):
 * Copyright (c) 2014 Maurits van der Schee
 */
static void setBufferedInput(bool enable) {
    static bool enabled = true;
    static struct termios old;

    if (enable && !enabled) {
        // restore the former settings
        tcsetattr(STDIN_FILENO, TCSANOW, &old);
        // set the new state
        enabled = true;
    } else if (!enable && enabled) {
        struct termios new;

        // get the terminal settings for standard input
        tcgetattr(STDIN_FILENO, &new);
        // we want to keep the old setting to restore them at the end
        old = new;
        // disable canonical mode (buffered i/o) and local echo
        new.c_lflag &=(~ICANON & ~ECHO);
        // set the new settings immediately
        tcsetattr(STDIN_FILENO, TCSANOW, &new);
        // set the new state
        enabled = false;
    }
}
