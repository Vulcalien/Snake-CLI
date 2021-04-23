#include "input.h"
#include "gameloop.h"

#include <stdio.h>
#include <signal.h>

// POSIX
#include <unistd.h>
#include <termios.h>
#include <pthread.h>

static void *receive_inputs(void *arg);

static void setBufferedInput(bool enable);
static void receive_signal_interrupt(int x);

struct player_Movement input_dir;

static pthread_t thread;

void input_init(void) {
    input_dir = DIRECTION_NONE;

    signal(SIGINT, receive_signal_interrupt);
    setBufferedInput(false);
}

void input_destroy(void) {
    setBufferedInput(true);
}

void input_thread_start(void) {
    pthread_create(&thread, NULL, receive_inputs, NULL);
}

void input_thread_stop(void) {
    pthread_cancel(thread);
}

void input_tick(void) {
    // TODO
}

static void *receive_inputs(void *arg) {
    while(true) {
        char c = getchar();

        switch (c) {
        case 'w':
        case 'W':
            input_dir = DIRECTION_UP;
            break;
        case 'a':
        case 'A':
            input_dir = DIRECTION_LEFT;
            break;
        case 's':
        case 'S':
            input_dir = DIRECTION_DOWN;
            break;
        case 'd':
        case 'D':
            input_dir = DIRECTION_RIGHT;
            break;
        }
    }
    return NULL;
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
