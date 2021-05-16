#ifndef VULC_SNAKECLI_PLAYER_TYPES
#define VULC_SNAKECLI_PLAYER_TYPES

#include "player.h"

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

#endif // VULC_SNAKECLI_PLAYER_TYPES
