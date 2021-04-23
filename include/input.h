#ifndef VULC_SNAKECLI_INPUT
#define VULC_SNAKECLI_INPUT

#include "snake.h"
#include "player.h"

extern struct player_Movement input_dir;

extern void input_init(void);
extern void input_destroy(void);

extern void input_thread_start(void);
extern void input_thread_stop(void);

extern void input_tick(void);

#endif // VULC_SNAKECLI_INPUT
