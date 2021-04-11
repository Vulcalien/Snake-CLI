#include "snake.h"
#include "screen.h"

#include <stdarg.h>

static char *screen_buffer;
static ui32 screen_size;

void screen_init(void) {
    screen_size = SCREEN_WIDTH * SCREEN_HEIGHT + SCREEN_HEIGHT - 1 + 1;

    screen_buffer = calloc(screen_size, sizeof(char));
    screen_clear(' ');
}

void screen_render(void) {
    // clear screen
    fputs("\033[H\033[J", stdout);

    fputs(screen_buffer, stdout);
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

    screen_buffer[x + y * (SCREEN_WIDTH + 1)] = c;
    return true;
}

void screen_printf(ui32 x, ui32 y, const char *format, ...) {
    va_list args;
    va_start(args, format);

    char tmp[SCREEN_WIDTH + 1] = {0};
    vsnprintf(tmp, SCREEN_WIDTH, format, args);

    va_end(args);

    ui32 xoff = 0;
    for(ui32 i = 0; true; i++) {
        char c = tmp[i];
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
