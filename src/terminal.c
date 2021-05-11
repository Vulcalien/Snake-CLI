/* Copyright 2021 Vulcalien
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
#include "terminal.h"

#ifdef __unix__
    #include <termios.h>
    #include <sys/ioctl.h>

    static struct termios old;

    void terminal_init(void) {
        tcgetattr(STDIN_FILENO, &old);

        struct termios new = old;

        new.c_lflag &= ~ICANON;
        new.c_lflag &= ~ECHO;

        tcsetattr(STDIN_FILENO, TCSANOW, &new);
    }

    void terminal_destroy(void) {
        tcsetattr(STDIN_FILENO, TCSANOW, &old);
    }

    ui32 terminal_width(void) {
        struct winsize ws;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);

        return ws.ws_col;
    }

    ui32 terminal_height(void) {
        struct winsize ws;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);

        return ws.ws_row;
    }
#elif _WIN32
    #include <windows.h>

    static HANDLE h_in = NULL;
    static HANDLE h_out = NULL;

    static DWORD old_out;
    static DWORD old_in;

    void terminal_init(void) {
        h_in  = GetStdHandle(STD_INPUT_HANDLE);
        h_out = GetStdHandle(STD_OUTPUT_HANDLE);

        DWORD new_in  = 0;
        DWORD new_out = 0;

        GetConsoleMode(h_in,  &new_in);
        GetConsoleMode(h_out, &new_out);

        old_in  = new_in;
        old_out = new_out;

        new_in &= ~ENABLE_LINE_INPUT;
        new_in &= ~ENABLE_ECHO_INPUT;

        new_out |= ENABLE_VIRTUAL_TERMINAL_PROCESSING; // ANSI codes

        SetConsoleMode(h_in,  new_in);
        SetConsoleMode(h_out, new_out);
    }

    void terminal_destroy(void) {
        SetConsoleMode(h_in, old_in);
        SetConsoleMode(h_out, old_out);
    }

    ui32 terminal_width(void) {
        // TODO windows
    }

    ui32 terminal_height(void) {
        // TODO windows
    }
#endif
