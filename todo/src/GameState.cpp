#include "GameState.hpp"
#include <iostream>

#if _STATS
    #define _STATS_MS_DIFF(starttime) static_cast<double>(((SDL_GetPerformanceCounter() - starttime) * 1000) / static_cast<double>(SDL_GetPerformanceFrequency()))
#endif

void GameStateProcessor::processStates() {
    #if _STATS
    double rtime = 0.0;
    double utime = 0.0;
    double itime = 0.0;
    #endif

    while ( !gameStates.empty() && isPlaying ) {
        auto state = gameStates.top();
        if (  state->load() ) {
            while ( state->isPlaying && isPlaying ) {
                #if _STATS
                auto istart = SDL_GetPerformanceCounter();
                #endif

                state->pumpEvents();

                #if _STATS
                itime = _STATS_MS_DIFF(istart);
                auto ustart = SDL_GetPerformanceCounter();
                #endif

                while ( clock.updateLag >= clock.msPerUpdate ) {   
                    state->update();
                    clock.updateLag -= clock.msPerUpdate;
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
                std::cout <<
                    "I " << itime << "\n" <<
                    "U " << utime << "\n" <<
                    "R " << rtime << "\n" <<
                    "F " << clock.frameElapsed << "\n";
                #endif
            }
        }
        gameStates.pop();
    }
}