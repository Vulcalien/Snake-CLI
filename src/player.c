/* Copyright 2021 Vulcalien
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
#include "player.h"

#include "vulcalien/screen.h"
#include "level.h"
#include "input.h"
#include "food.h"

#include "private/player_types.h"

static void node_move(struct player_Node *node, struct player_Movement dir);

static struct player_Movement get_trace(u32 x, u32 y);
static void set_trace(u32 x, u32 y, struct player_Movement dir);

static bool does_tile_contain_node(u32 x, u32 y);

static struct Player player;
static struct player_Movement *player_traces;

static bool has_moved = false;
static u32 should_grow = 0;

void player_init(u32 x0, u32 y0, u32 size,
                 struct player_Movement dir) {
    player = (struct Player) {
        .dir = dir,
        .size = size,

        .head = {x0, y0},
        .body = malloc(LEVEL_SIZE * sizeof(struct player_Node))
    };
    player_traces = malloc(LEVEL_SIZE * sizeof(struct player_Movement));
    for(u32 i = 0; i < LEVEL_SIZE; i++) {
        player_traces[i] = DIRECTION_NONE;
    }

    for(u32 i = 0; i < size; i++) {
        player.body[i] = (struct player_Node) {
            .x = x0 - dir.xm * (i + 1),
            .y = y0 - dir.ym * (i + 1)
        };
    }

    for(u32 i = 0; i < player.size; i++) {
        struct player_Node *node = &player.body[i];
        set_trace(node->x, node->y, player.dir);
    }
}

void player_destroy(void) {
    free(player.body);
    free(player_traces);
}

void player_tick(void) {
    struct player_Node *head = &player.head;

    // get direction from the input handler
    struct player_Movement new_dir = input_dir;
    if(new_dir.xm == 0 && new_dir.ym == 0) return;

    // change direction only if the player is
    // not trying to rotate by 180 degrees
    if(new_dir.xm != -player.dir.xm || new_dir.ym != -player.dir.ym) {
        player.dir = new_dir;
        has_moved = true;
    }
    if(!has_moved) return;

    set_trace(head->x, head->y, player.dir);
    node_move(head, player.dir);

    if(does_tile_contain_node(head->x, head->y)) {
        is_game_over = true;
    }

    if(food.x == head->x && food.y == head->y) {
        u32 food_value = food.is_special ? 3 : 1;

        should_grow = food_value;
        food_spawn();
        score += food_value;
    }

    for(u32 i = 0; i < player.size; i++) {
        struct player_Node *node = &player.body[i];
        struct player_Node old_node = *node;

        struct player_Movement dir = get_trace(node->x, node->y);
        node_move(node, dir);

        if(i == player.size - 1 && should_grow) {
            player.size++;
            should_grow--;

            player.body[player.size -1] = old_node;
            break;
        }
    }
}

static void node_move(struct player_Node *node, struct player_Movement dir) {
    i32 xnew = node->x + dir.xm;
    i32 ynew = node->y + dir.ym;

    if(xnew < 0) xnew = LEVEL_WIDTH - 1;
    if(xnew >= LEVEL_WIDTH) xnew = 0;

    if(ynew < 0) ynew = LEVEL_HEIGHT - 1;
    if(ynew >= LEVEL_HEIGHT) ynew = 0;

    node->x = xnew;
    node->y = ynew;
}

static struct player_Movement get_trace(u32 x, u32 y) {
    return player_traces[x + y * LEVEL_WIDTH];
}

static void set_trace(u32 x, u32 y, struct player_Movement dir) {
    player_traces[x + y * LEVEL_WIDTH] = dir;
}

void player_render(void) {
    for(u32 i = 0; i < player.size; i++) {
        struct player_Node node = player.body[i];
        struct player_Movement mov = get_trace(node.x, node.y);

        char c;
        if(i == player.size - 1) {
            if(mov.xm != 0) c = '-';
            else            c = '|';
        } else {
            struct player_Node next = player.body[i + 1];
            bool xdiff = (node.x - next.x) != 0;
            bool ydiff = (node.y - next.y) != 0;

            if     (mov.xm != 0 && xdiff) c = '-';
            else if(mov.ym != 0 && ydiff) c = '|';
            else                          c = '*';
        }
        screen_setchar(node.x, node.y, c, "\033[1;32m");
    }

    // render the head after the body, so when there is
    // "game over", the head is rendered
    screen_setchar(player.head.x, player.head.y, '@', "\033[1;92m");
}

bool player_is_tile_free(u32 x, u32 y) {
    if(player.head.x == x && player.head.y == y) return false;
    return !does_tile_contain_node(x, y);
}

static bool does_tile_contain_node(u32 x, u32 y) {
    for(u32 i = 0; i < player.size; i++) {
        struct player_Node node = player.body[i];
        if(node.x == x && node.y == y) return true;
    }
    return false;
}
