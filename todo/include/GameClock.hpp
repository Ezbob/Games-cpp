#ifndef HEADER_GUARD_6406329bbe77dff507947b3443ad5aa2
#define HEADER_GUARD_6406329bbe77dff507947b3443ad5aa2

#include "SDL2/SDL.h"

struct GameClock {
    double msPerUpdate = 16.0;
    uint64_t now = SDL_GetPerformanceCounter();
    uint64_t last = 0;
    double frameElapsed = 0.0;
    double updateLag = 0.0;

    void tick() {
        last = now;
        now = SDL_GetPerformanceCounter();
        frameElapsed = ((now - last) * 1000) / static_cast<double>(SDL_GetPerformanceFrequency());
        updateLag += frameElapsed;
    }
};

#endif
