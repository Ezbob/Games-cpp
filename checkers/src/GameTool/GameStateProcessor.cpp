
#include "GameStateProcessor.hpp"
#include <iostream>
#include "SDL_timer.h"
#include <stdexcept>

using namespace asa;

void GameStateProcessor::addState(std::shared_ptr<GameState> state)
{
    m_gameStates.push_back(state);
    m_currentIndex = (m_gameStates.size() - 1);
}

void GameStateProcessor::quitGame(void)
{
    m_isPlaying = false;
}

std::shared_ptr<GameState> GameStateProcessor::currentState(void) const
{
    if (m_currentIndex >= m_gameStates.size())
    {
        throw std::out_of_range("GameState index out of range");
    }
    return m_gameStates[m_currentIndex];
}

void GameStateProcessor::removeCurrentState(void)
{
    m_gameStates.pop_back();
    m_currentIndex = (m_gameStates.size() - 1);
}

void GameStateProcessor::startFromState(const uint32_t index)
{
    m_currentIndex = index;
    m_shouldReload = true;
}

uint32_t GameStateProcessor::lastIndex(void) const {
    return m_gameStates.size() - 1;
}

#if _STATS
#define _STATS_MS_DIFF(starttime) static_cast<double>(((SDL_GetPerformanceCounter() - starttime) * 1000) / static_cast<double>(SDL_GetPerformanceFrequency()))
#endif

void GameStateProcessor::processStates(GameClock &clock)
{
#if _STATS
    double rtime = 0.0;
    double utime = 0.0;
    double itime = 0.0;
#endif

#if !_STATS && _DEBUG
    std::cout << "Initial gamestate stack size: " << m_gameStates.size() << "\n";
#endif

    while (!m_gameStates.empty() && m_isPlaying)
    {
    gameloop_start:
        auto state = currentState();

        if (!state->isLoaded())
        {
            state->load();
#if !_STATS && _DEBUG
            std::cout << "State not loaded. Load successful: " << (state->isLoaded() ? "true" : "false") << "\n";
#endif
        }

        if (state->isLoaded())
        {
            while (state->isPlaying() && m_isPlaying)
            {
#if _STATS
                auto istart = SDL_GetPerformanceCounter();
#endif

                state->pumpEvents();

#if _STATS
                itime = _STATS_MS_DIFF(istart);
                auto ustart = SDL_GetPerformanceCounter();
#endif

                while (clock.shouldUpdate())
                {
                    state->update();

                    clock.paybackLag();
                }

#if _STATS
                utime = _STATS_MS_DIFF(ustart);
                auto rstart = SDL_GetPerformanceCounter();
#endif

                state->render();

#if _STATS
                rtime = _STATS_MS_DIFF(rstart);
#endif

                clock.tick();

#if _STATS
                std::cout << "I " << itime << "\n"
                          << "U " << utime << "\n"
                          << "R " << rtime << "\n"
                          << "F " << clock.frameElapsed << "\n";
#endif

                if (m_shouldReload)
                {
                    m_shouldReload = false;
                    goto gameloop_start;
                }
            }
        }

        removeCurrentState();
    }
}

void GameStateProcessor::startFromNewState(const std::shared_ptr<GameState> state)
{
    state->isPlaying(true);
    m_gameStates.emplace_back(state);
    m_shouldReload = true;
}