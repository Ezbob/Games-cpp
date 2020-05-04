
#include "GameStateProcessor.hpp"
#include <iostream>
#include "SDL_timer.h"

using namespace asa;

GameStateProcessor::GameStateProcessor(unsigned int targetFps)
    : comm_queue(std::make_unique<SingleThreadedMessageQueue>())
{
    double msPerFrame = (1. / ((double)targetFps)) * 1000.;
    clock.msPerUpdate(msPerFrame);
}

GameStateProcessor::GameStateProcessor(unsigned int fps, std::unique_ptr<MessageQueueInterface> q) 
    : comm_queue(std::move(q))
{
    double msPerFrame = (1. / ((double)fps)) * 1000.;
    clock.msPerUpdate(msPerFrame);
}

void GameStateProcessor::initStates(std::function<void(GameStateStack &, MessageQueueInterface&)> initFunction)
{
    initFunction(gameStates, *comm_queue.get());
}

asa::GameClock const &GameStateProcessor::getClock() const
{
    return clock;
}

void GameStateProcessor::quitGame()
{
    m_isPlaying = false;
}

#if _STATS
#define _STATS_MS_DIFF(starttime) static_cast<double>(((SDL_GetPerformanceCounter() - starttime) * 1000) / static_cast<double>(SDL_GetPerformanceFrequency()))
#endif

void GameStateProcessor::processStates()
{
#if _STATS
    double rtime = 0.0;
    double utime = 0.0;
    double itime = 0.0;
#endif

#if !_STATS && _DEBUG
    std::cout << "Initial gamestate stack size: " << gameStates.size() << "\n";
#endif

    while (!gameStates.empty() && m_isPlaying)
    {
    gameloop_start:
        auto state = gameStates.top();

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

        gameStates.pop();
    }
}

void GameStateProcessor::startFromNewState(const std::shared_ptr<GameState> state)
{
    state->isPlaying(true);
    gameStates.emplace(state);
    m_shouldReload = true;
}