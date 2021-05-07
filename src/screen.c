/*
 * Copyright 2021 Vulcalien
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
#include "screen.h"

#include <stdio.h>
#include <stdarg.h>

static char *screen_buffer;
static ui32 screen_size;

void screen_init(void) {
    screen_size = SCREEN_WIDTH * SCREEN_HEIGHT + SCREEN_HEIGHT - 1 + 1;

    screen_buffer = calloc(screen_size, sizeof(char));
    screen_clear(' ');

    fputs("\033[?25l", stdout); // hide cursor
    fputs("\033[2J", stdout);   // moves screen content up (fake "clear")
}

void screen_destroy(void) {
    fputs("\033[?25h", stdout); // show cursor
    fputs("\033[m", stdout);    // reset color

    free(screen_buffer);
}

void screen_render(void) {
    fputs("\033[H", stdout); // move to top left corner

    #ifdef COLORS
        for(ui32 i = 0; i < screen_size; i++) {
            char c = screen_buffer[i];

            if     (c == '@') fputs("\033[1;92m@\033[m", stdout);
            else if(c == '-') fputs("\033[1;32m-\033[m", stdout);
            else if(c == '|') fputs("\033[1;32m|\033[m", stdout);
            else if(c == '*') fputs("\033[1;32m*\033[m", stdout);
            else if(c == '$') fputs("\033[1;31m$\033[m", stdout);
            else              fputc(c, stdout);
        }
    #else
        fputs(screen_buffer, stdout);
    #endif
}

void screen_clear(char c) {
    for(ui32 i = 0; i < screen_size; i++) {
        screen_buffer[i] = c;
    }
    for(ui32 i = 0; i < SCREEN_HEIGHT; i++) {
        screen_buffer[(SCREEN_WIDTH + 1) * (i + 1) - 1] = '\n';
    }
}

bool screen_setchar(ui32 x, ui32 y, char c) {
    if(x < 0 || x >= SCREEN_WIDTH) return false;
    if(y < 0 || y >= SCREEN_HEIGHT) return false;

    // ` is transparent
    if(c == '`') return false;

    screen_buffer[x + y * (SCREEN_WIDTH + 1)] = c;
    return true;
}

void screen_puts(ui32 x, ui32 y, const char *string) {
    ui32 xoff = 0;
    for(ui32 i = 0; true; i++) {
        char c = string[i];
        if(c == '\0') {
            break;
        } else if(c == '\n') {
            y++;
            xoff = 0;
            continue;
        }
        screen_setchar(x + xoff, y, c);
        xoff++;
    }
}

void screen_printf(ui32 x, ui32 y, const char *format, ...) {
    va_list args;
    va_start(args, format);

    char tmp[SCREEN_WIDTH + 1] = {0};
    vsnprintf(tmp, SCREEN_WIDTH, format, args);

    va_end(args);

    screen_puts(x, y, tmp);
}
