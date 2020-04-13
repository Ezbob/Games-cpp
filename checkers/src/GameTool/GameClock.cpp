#include "GameTool/GameClock.hpp"

using namespace gtool;


double GameClock::msPerUpdate(void) const {
    return m_msPerUpdate;
}

double GameClock::msPerUpdate(double ms) {
    return (m_msPerUpdate = ms);
}

bool GameClock::shouldUpdate(void) const {
    return m_updateLag >= m_msPerUpdate;
}

void GameClock::paybackLag(void) {
    m_updateLag -= m_msPerUpdate;
}

void GameClock::tick() {
    m_last = m_now;
    m_now = SDL_GetPerformanceCounter();
    m_frameElapsed = ((m_now - m_last) * 1000) / static_cast<double>(SDL_GetPerformanceFrequency());
    m_updateLag += m_frameElapsed;
}
