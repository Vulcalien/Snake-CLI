#include "highscore.h"

#define VULCALIEN_DIR  "vulcalien/"
#define SCORE_FILENAME "vulcalien-snake.score"

#define TIME_BUFFER_SIZE 32

#include <stdio.h>
#include <limits.h>
#include <time.h>

static int open_file(const char *modes, FILE **file);
static int close_file(FILE **file);

static int add_to_path(char *dest, char *src, ui32 *remaining_space);

int highscore_get(ui32 *score) {
    int err;

    FILE *file;
    err = open_file("r", &file);
    if(err) {
        *score = 0;
    } else {
        // TODO read score
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
            "%Y-%m-%d_%H.%M.%S",
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

        err = add_to_path(path, getenv("HOME"),  &space);
        err = add_to_path(path, "/Documents/",   &space);
        err = add_to_path(path, VULCALIEN_DIR,   &space);
        err = add_to_path(path, SCORE_FILENAME,  &space);
    #elif _WIN32
        char *path = "%APPDATA%/" VULCALIEN_DIR SCORE_FILENAME;
    #endif

    if(!err) {
        *file = fopen(path, modes);
        if(*file == NULL) {
            fputs("Error: could not read score file\n", stderr);
            err = -1;
        }
    }
    free(path); // TODO only necessary for unix (at the moment)
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
        err = -1;
        fputs("Error: PATH_MAX exceded\n", stderr);
    } else {
        *remaining_space -= src_len;
        strcat(dest, src);
    }
    return err;
}
