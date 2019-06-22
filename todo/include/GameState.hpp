#ifndef HEADER_GUARD_a17e097f8bd12c4c14b2c994eb545006
#define HEADER_GUARD_a17e097f8bd12c4c14b2c994eb545006

#include <stack>
#include <array>
#include <memory>
#include <functional>
#include "GameClock.hpp"

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

public:
    static GameStateProcessor create(std::function<void (GameStateStack &)> initFunction) {
        GameStateProcessor gp;
        initFunction(gp.gameStates);
        return gp;
    }

    void initStates(std::function<void (GameStateStack &)> initFunction) {
        initFunction(gameStates);
    }

    void processStates() {
        while ( !gameStates.empty() ) {

            if ( auto state = gameStates.top(); state->load() ) {
                while ( state->isPlaying ) {
                    state->handleInput();

                    while ( clock.updateLag >= clock.msPerUpdate ) {
                        state->update();
                        clock.updateLag -= clock.msPerUpdate;
                    }

                    state->render();

                    clock.tick();
                }
            }

            gameStates.pop();
        }
    }

    GameClock const *getClock() {
        return &clock;
    }
};

#endif
