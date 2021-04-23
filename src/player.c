#include "player.h"
#include "screen.h"
#include "input.h"

#include "private/player_types.h"

static void node_move(struct player_Node *node, struct player_Movement dir);

static struct player_Movement get_trace(ui32 x, ui32 y);
static void set_trace(ui32 x, ui32 y, struct player_Movement dir);

static struct Player player;

static struct player_Movement *player_traces;
static bool is_first_movement = true;

void player_init(ui32 x0, ui32 y0, ui32 len,
                 struct player_Movement dir) {
    player = (struct Player) {
        .dir = dir,
        .size = len,

        .head = {x0, y0},
        .body = malloc((len - 1) * sizeof(struct player_Node))
    };
    player_traces = malloc(SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(struct player_Movement));
    for(ui32 i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        player_traces[i] = DIRECTION_NONE;
    }

    for(ui32 i = 0; i < len; i++) {
        player.body[i] = (struct player_Node) {
            .x = x0 - dir.xm * (i + 1),
            .y = y0 - dir.ym * (i + 1)
        };
    }
}

void player_destroy(void) {
    free(player.body);
    free(player_traces);
}

void player_tick(void) {
    player.dir = input_dir;

    struct player_Node *head = &player.head;

    if(is_first_movement) {
        for(ui32 i = 0; i < player.size; i++) {
            struct player_Node *node = &player.body[i];
            set_trace(node->x, node->y, player.dir);
        }
        is_first_movement = false;
    }
    set_trace(head->x, head->y, player.dir);
    node_move(head, player.dir);

    // check for food

    for(ui32 i = 0; i < player.size; i++) {
        struct player_Node *node = &player.body[i];
        // struct player_Node old_node = *node;

        struct player_Movement dir = get_trace(node->x, node->y);
        node_move(node, dir);

        // if(i == player.size - 1) {
        //     set_trace(old_node.x, old_node.y, DIRECTION_NONE);
        // }
    }
}

static void node_move(struct player_Node *node, struct player_Movement dir) {
    i32 xnew = node->x + dir.xm;
    i32 ynew = node->y + dir.ym;

    if(xnew < 0) xnew = SCREEN_WIDTH - 1;
    if(xnew >= SCREEN_WIDTH) xnew = 0;

    if(ynew < 0) ynew = SCREEN_HEIGHT - 1;
    if(ynew >= SCREEN_HEIGHT) ynew = 0;

    node->x = xnew;
    node->y = ynew;
}

static struct player_Movement get_trace(ui32 x, ui32 y) {
    return player_traces[x + y * SCREEN_WIDTH];
}

static void set_trace(ui32 x, ui32 y, struct player_Movement dir) {
    player_traces[x + y * SCREEN_WIDTH] = dir;
}

void player_render(void) {
    screen_setchar(player.head.x, player.head.y, '#');

    for(ui32 i = 0; i < player.size; i++) {
        struct player_Node node = player.body[i];
        screen_setchar(node.x, node.y, 'O');
    }
}
