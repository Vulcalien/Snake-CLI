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
#include "highscore.h"

#ifdef __unix__
    #define PATH_SEPARATOR "/"
#elif _WIN32
    #define PATH_SEPARATOR "\\"
#endif

#define VULCALIEN_DIR  "vulcalien"
#define SCORE_FILENAME "vulcalien-snake.score"

#define READ_BUFFER_SIZE (1024)
#define LINE_BUFFER_SIZE (256)

#define TIME_BUFFER_SIZE (32)

#include <stdio.h>
#include <limits.h>
#include <time.h>

static int open_file(const char *modes, FILE **file);
static int close_file(FILE **file);

int highscore_get(u32 *score) {
    int err;

    FILE *file;
    err = open_file("r", &file);
    if(err) {
        *score = 0;
    } else {
        char *buffer      = malloc(READ_BUFFER_SIZE * sizeof(char));
        char *line_buffer = malloc(LINE_BUFFER_SIZE * sizeof(char));

        u32 line_len = 0;
        bool last_is_newline = false;

        while(true) {
            u32 read_n = fread(buffer, sizeof(char), READ_BUFFER_SIZE, file);
            if(read_n == 0) break;

            for(u32 i = 0; i < read_n; i++) {
                char c = buffer[i];

                if(c == '\n') {
                    last_is_newline = true;
                } else {
                    if(last_is_newline) {
                        last_is_newline = false;
                        line_len = 0;
                    }

                    if(line_len >= LINE_BUFFER_SIZE - 1) {
                        fputs("Error: line buffer size exceded\n", stderr);
                        err = EOF;
                        break;
                    }
                    line_buffer[line_len] = c;
                    line_len++;
                }
            }
            if(err) break;
        }
        if(!err) {
            // the buffer surely contains junk data
            // and it is not null-terminated
            line_buffer[line_len] = '\0';

            if(sscanf(line_buffer, "%u", score) == EOF) {
                fputs("Error: scanf failed to read\n", stderr);
                err = EOF;
            }
        }
        free(buffer);
        free(line_buffer);

        close_file(&file);
    }
    return err;
}

int highscore_set(u32 score) {
    int err;

    FILE *file;
    err = open_file("a", &file);
    if(!err) {
        time_t raw_time = time(NULL);
        struct tm *timeinfo = localtime(&raw_time);

        char time_buffer[TIME_BUFFER_SIZE];
        strftime(
            time_buffer,
            TIME_BUFFER_SIZE,
            "%Y-%m-%d_%H:%M:%S",
            timeinfo
        );
        fputs(time_buffer, file);
        fputs("\n", file);
        fprintf(file, "%u\n", score);

        close_file(&file);
    }
    return err;
}

static int open_file(const char *modes, FILE **file) {
    int err = 0;

    char *path = malloc(PATH_MAX * sizeof(char));

    int written;
    #ifdef __unix__
        written = snprintf(
            path, PATH_MAX,
            "%s" PATH_SEPARATOR
                "Documents" PATH_SEPARATOR
                VULCALIEN_DIR PATH_SEPARATOR
                SCORE_FILENAME,
            getenv("HOME")
        );
    #elif _WIN32
        written = snprintf(
            path, PATH_MAX,
            "%s" PATH_SEPARATOR
                VULCALIEN_DIR PATH_SEPARATOR
                SCORE_FILENAME,
            getenv("APPDATA")
        );
    #endif

    if(written >= PATH_MAX) {
        fputs("Error: PATH_MAX exceded\n", stderr);
        err = EOF;
    } else {
        *file = fopen(path, modes);
        if(*file == NULL) {
            fputs("Error: could not open score file\n", stderr);
            err = EOF;
        }
    }
    free(path);
    return err;
}

static int close_file(FILE **file) {
    int err = fclose(*file);
    *file = NULL;

    if(err) {
        fputs("Error: could not close score file\n", stderr);
    }
    return err;
}
