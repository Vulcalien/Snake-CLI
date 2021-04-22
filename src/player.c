#include "player.h"
#include "screen.h"
#include "private/player_types.h"

static void node_move(struct player_Node *node, struct Movement mov);
static struct Movement dir_to_movement(enum player_Direction dir);

static enum player_Direction get_trace(ui32 x, ui32 y);
static void set_trace(ui32 x, ui32 y, enum player_Direction dir);

static struct Player player;

static enum player_Direction *player_traces;

void player_init(ui32 x0, ui32 y0, ui32 len,
                 enum player_Direction dir) {
    player = (struct Player) {
        .dir = dir,
        .size = len,

        .head = {x0, y0},
        .body = malloc((len - 1) * sizeof(struct player_Node))
    };
    player_traces = malloc(SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(enum player_Direction));
    for(ui32 i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        player_traces[i] = DIRECTION_NONE;
    }

    struct Movement initial_movement = dir_to_movement(dir);
    for(ui32 i = 0; i < len; i++) {
        player.body[i] = (struct player_Node) {
            .x = x0 - initial_movement.xm * (i + 1),
            .y = y0 - initial_movement.ym * (i + 1)
        };
    }
}

void player_destroy(void) {
    free(player.body);
    free(player_traces);
}

void player_tick(void) {
    struct player_Node *head = &player.head;

    struct Movement head_mov = dir_to_movement(player.dir);
    node_move(head, head_mov);
    set_trace(head->x, head->y, player.dir);

    // check for food

    for(ui32 i = 0; i < player.size; i++) {
        struct player_Node *node = &player.body[i];
        struct player_Node old_node = *node;

        enum player_Direction dir = get_trace(node->x, node->y);
        node_move(node, dir_to_movement(dir));

        if(i == player.size - 1) {
            set_trace(old_node.x, old_node.y, DIRECTION_NONE);
        }
    }
}

static void node_move(struct player_Node *node, struct Movement mov) {
    i32 xnew = node->x + mov.xm;
    i32 ynew = node->y + mov.ym;

    if(xnew < 0) xnew = SCREEN_WIDTH - 1;
    if(xnew >= SCREEN_WIDTH) xnew = 0;

    if(ynew < 0) ynew = SCREEN_HEIGHT - 1;
    if(ynew >= SCREEN_HEIGHT) ynew = 0;

    node->x = xnew;
    node->y = ynew;
}

static struct Movement dir_to_movement(enum player_Direction dir) {
    struct Movement mov = {
        .xm = 0,
        .ym = 0
    };

    if     (player.dir == DIRECTION_UP)    mov.ym--;
    else if(player.dir == DIRECTION_LEFT)  mov.xm--;
    else if(player.dir == DIRECTION_DOWN)  mov.ym++;
    else if(player.dir == DIRECTION_RIGHT) mov.xm++;

    return mov;
}

static enum player_Direction get_trace(ui32 x, ui32 y) {
    return player_traces[x + y * SCREEN_WIDTH];
}

static void set_trace(ui32 x, ui32 y, enum player_Direction dir) {
    player_traces[x + y * SCREEN_WIDTH] = dir;
}

void player_render(void) {
    screen_setchar(player.head.x, player.head.y, '#');

    for(ui32 i = 0; i < player.size; i++) {
        struct player_Node node = player.body[i];
        screen_setchar(node.x, node.y, 'O');
    }
}
