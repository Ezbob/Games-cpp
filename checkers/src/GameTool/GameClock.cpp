#include "GameClock.hpp"

using namespace asa;

GameClock::GameClock(int targetFps) {
    m_msPerUpdate = (1. / ((double)targetFps)) * 1000.;
}

double GameClock::msPerUpdate(void) const
{
    return m_msPerUpdate;
}

double GameClock::msPerUpdate(double ms)
{
    return (m_msPerUpdate = ms);
}

bool GameClock::shouldUpdate(void) const
{
    return m_updateLag >= m_msPerUpdate;
}

void GameClock::paybackLag(void)
{
    m_updateLag -= m_msPerUpdate;
}

void GameClock::tick(void)
{
    m_last = m_now;
    m_now = SDL_GetPerformanceCounter();
    m_frameElapsed = ((m_now - m_last) * 1000) / static_cast<double>(SDL_GetPerformanceFrequency());
    m_updateLag += m_frameElapsed;
}

auto GameClock::getDeltaTime(void) const -> double {
    return m_frameElapsed;
}