#ifndef HEADER_GUARD_6406329bbe77dff507947b3443ad5aa2
#define HEADER_GUARD_6406329bbe77dff507947b3443ad5aa2

#include "SDL2/SDL.h"
#include <cstdint>

namespace gtool {

    class GameClock {
    public:
        void tick(void);

        double msPerUpdate(void) const;
        double msPerUpdate(double);

        bool shouldUpdate(void) const;

        void paybackLag(void);
    private:
        uint64_t m_now = SDL_GetPerformanceCounter();
        uint64_t m_last = 0;
        double m_msPerUpdate = 16.0;
        double m_frameElapsed = 0.0;
        double m_updateLag = 0.0;
    };
}

#endif
