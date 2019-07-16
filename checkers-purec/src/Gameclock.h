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

#define GT_CLOCK_TICK(clock) do { \
        clock.last = clock.now; \
        clock.now = SDL_GetPerformanceCounter(); \
        clock.frameElapsed = ((clock.now - clock.last) * 1000) / (double)(SDL_GetPerformanceFrequency()); \
        clock.updateLag += clock.frameElapsed; \
    } while(0)

#define GT_CLOCK_LAG_UPDATE(clock) (clock.updateLag -= clock.msPerUpdate)
#define GT_CLOCK_SHOULD_UPDATE(clock) (g_gameclock.updateLag >= g_gameclock.msPerUpdate)

#endif
