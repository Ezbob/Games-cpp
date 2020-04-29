#pragma once

#include "SDL_timer.h"
#include <cstdint>

namespace asa
{

class GameClock
{
public:
    void tick(void);

    double msPerUpdate(void) const;
    double msPerUpdate(double);

    bool shouldUpdate(void) const;

    void paybackLag(void);

    // in miliseconds
    auto getDeltaTime(void) const -> double;

private:
    uint64_t m_now = SDL_GetPerformanceCounter();
    uint64_t m_last = 0;
    double m_msPerUpdate = 16.0;
    double m_frameElapsed = 0.0;
    double m_updateLag = 0.0;
};
} // namespace asa
