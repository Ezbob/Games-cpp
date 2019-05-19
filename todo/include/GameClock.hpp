#ifndef HEADER_GUARD_6406329bbe77dff507947b3443ad5aa2
#define HEADER_GUARD_6406329bbe77dff507947b3443ad5aa2

#include "SDL.h"

struct GameClock {
    uint64_t now = SDL_GetPerformanceCounter();
    uint64_t last = 0;
    double elapsed = 0.0;
    double lag = 0.0;

    void tick() {
        last = now;
        now = SDL_GetPerformanceCounter();
        elapsed = (now - last) * 1000 / (double) SDL_GetPerformanceFrequency();
        lag += elapsed;
    }
};

#endif
