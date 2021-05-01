/*
 * Copyright 2021 Vulcalien
 * This program is free software. See NOTICE and LICENSE.
 */
#include "input.h"
#include "gameloop.h"

#include <stdio.h>
#include <signal.h>

#include <pthread.h>

#ifdef __unix__
    #include <unistd.h>
    #include <termios.h>
#elif _WIN32
    #include <windows.h>
#endif

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

static void *receive_inputs(void *arg) {
    ui32 escape_found = 0;
    while(true) {
        char c = getchar();
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

// TODO find a better name for what this does and
// consider moving this to its own file (terminal.c ???)
#ifdef __unix__
    /*
    * copied from https://github.com/mevdschee/2048.c
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

            new.c_lflag &= ~ICANON; // disable canonical mode (buffered i/o)
            new.c_lflag &= ~ECHO;   // disable local echo

            // set the new settings immediately
            tcsetattr(STDIN_FILENO, TCSANOW, &new);
            // set the new state
            enabled = false;
        }
    }
#elif _WIN32
    // this code i'm writing, looks very stupid
    static void setBufferedInput(bool enable) {
        static bool enabled = true;
        static DWORD oldOut;
        static DWORD oldIn;

        static HANDLE hOut = NULL;
        static HANDLE hIn = NULL;
        if(hOut == NULL) {
            hOut = GetStdHandle(STD_OUTPUT_HANDLE);
            hIn  = GetStdHandle(STD_INPUT_HANDLE);
        }

        if(enable && !enabled) {
            // i'm not sure if this works
            SetConsoleMode(hOut, oldOut);
            SetConsoleMode(hIn,  oldIn);
            enabled = true;
        } else if(!enable && enabled) {
            DWORD newOut = 0;
            DWORD newIn = 0;
            GetConsoleMode(hOut, &newOut);
            GetConsoleMode(hIn,  &newIn);

            oldOut = newOut;

            // enable ANSI escape codes
            newOut |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, newOut);

            newIn &= ~ENABLE_ECHO_INPUT;
            newIn &= ~ENABLE_LINE_INPUT;
            SetConsoleMode(hIn, newIn);

            enabled = false;
        }
    }
#endif
