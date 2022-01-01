/* Copyright 2021-2022 Vulcalien
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 only.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#ifndef VULC_SNAKECLI_PLAYER
#define VULC_SNAKECLI_PLAYER

#include "snake.h"

#define DIRECTION_UP    ((struct player_Movement) {0 , -1})
#define DIRECTION_LEFT  ((struct player_Movement) {-1,  0})
#define DIRECTION_DOWN  ((struct player_Movement) {0 , +1})
#define DIRECTION_RIGHT ((struct player_Movement) {+1,  0})

#define DIRECTION_NONE  ((struct player_Movement) {0 ,  0})

struct player_Movement {
    i32 xm;
    i32 ym;
};

struct player_Node {
    u32 x;
    u32 y;
};

struct Player {
    struct player_Movement dir;

    u32 size;

    struct player_Node head;
    struct player_Node *body;
};

extern struct Player player;

extern void player_init(u32 x0, u32 y0, u32 size,
                        struct player_Movement dir);
extern void player_destroy(void);

extern void player_tick(void);
extern void player_render(void);

extern bool player_is_tile_free(u32 x, u32 y);

#endif // VULC_SNAKECLI_PLAYER
