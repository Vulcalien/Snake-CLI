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

static int add_to_path(char *dest, char *src, ui32 *remaining_space);
static int add_separator(char *dest, ui32 *remaining_space);

int highscore_get(ui32 *score) {
    int err;

    FILE *file;
    err = open_file("r", &file);
    if(err) {
        *score = 0;
    } else {
        char *buffer      = malloc(READ_BUFFER_SIZE * sizeof(char));
        char *line_buffer = malloc(LINE_BUFFER_SIZE * sizeof(char));

        ui32 line_len = 0;
        bool last_is_newline = false;

        while(true) {
            ui32 read_n = fread(buffer, sizeof(char), READ_BUFFER_SIZE, file);
            if(read_n == 0) break;

            for(ui32 i = 0; i < read_n; i++) {
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

int highscore_set(ui32 score) {
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

    #ifdef __unix__
        char *path = calloc(PATH_MAX + 1, sizeof(char));
        ui32 space = PATH_MAX;

        err = add_to_path(path, getenv("HOME"), &space);
        err = add_separator(path, &space);
        err = add_to_path(path, "Documents",    &space);
        err = add_separator(path, &space);
        err = add_to_path(path, VULCALIEN_DIR,  &space);
        err = add_separator(path, &space);
        err = add_to_path(path, SCORE_FILENAME, &space);
    #elif _WIN32
        char *path = calloc(PATH_MAX + 1, sizeof(char));
        ui32 space = PATH_MAX;

        err = add_to_path(path, getenv("APPDATA"), &space);
        err = add_separator(path, &space);
        err = add_to_path(path, VULCALIEN_DIR,     &space);
        err = add_separator(path, &space);
        err = add_to_path(path, SCORE_FILENAME,    &space);
    #endif

    if(!err) {
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

static int add_to_path(char *dest, char *src, ui32 *remaining_space) {
    int err = 0;

    ui32 src_len = strlen(src);
    if(src_len > *remaining_space) {
        fputs("Error: PATH_MAX exceded\n", stderr);
        err = EOF;
    } else {
        *remaining_space -= src_len;
        strcat(dest, src);
    }
    return err;
}

static int add_separator(char *dest, ui32 *remaining_space) {
    return add_to_path(dest, PATH_SEPARATOR, remaining_space);
}
