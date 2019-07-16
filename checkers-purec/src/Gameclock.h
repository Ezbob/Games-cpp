#ifndef HEADER_GUARD_2c42e4f3f6631169b8f6ba1cb44edd44
#define HEADER_GUARD_2c42e4f3f6631169b8f6ba1cb44edd44

#include "sdl2/SDL.h"
#include <stdint.h>

struct GameClock {
    double msPerUpdate;
    uint64_t now;
    uint64_t last;
    double frameElapsed;
    double updateLag;
};

void GT_gameclock_init(struct GameClock *, double);
void GT_gameclock_tick(struct GameClock *);

#endif
