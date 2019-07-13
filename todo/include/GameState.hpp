#ifndef HEADER_GUARD_a17e097f8bd12c4c14b2c994eb545006
#define HEADER_GUARD_a17e097f8bd12c4c14b2c994eb545006

#include <stack>
#include <array>
#include <memory>
#include <functional>
#include "GameClock.hpp"

#if _STATS
    #define _STATS_MS_DIFF(starttime) static_cast<double>(((SDL_GetPerformanceCounter() - starttime) * 1000) / static_cast<double>(SDL_GetPerformanceFrequency()))
#endif

struct GameState {
    virtual ~GameState() {};
    virtual void handleInput() = 0;
    virtual bool load() = 0;
    virtual void update() = 0;
    virtual void render() = 0;

    bool isPlaying = true;
};

using GameStateStack = std::stack<std::shared_ptr<GameState>>;

class GameStateProcessor {
    GameStateStack gameStates;
    GameClock clock;

    bool isPlaying = true;
public:
    GameStateProcessor(double msPerFrame = 16.) {
        clock.msPerUpdate = msPerFrame;
    }

    void initStates(std::function<void (GameStateStack &)> initFunction) {
        initFunction(gameStates);
    }

    void processStates() {
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

                    state->handleInput();

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

    GameClock const *getClock() {
        return &clock;
    }

    void quitGame() {
        isPlaying = false;
    }
};

#endif
