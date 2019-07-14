#ifndef HEADER_GUARD_a17e097f8bd12c4c14b2c994eb545006
#define HEADER_GUARD_a17e097f8bd12c4c14b2c994eb545006

#include "SDL.h"
#include <stack>
#include <memory>
#include <functional>
#include "GameTool/GameClock.hpp"

namespace gtool {

    struct GameState {
        bool isPlaying = true;

        void pumpEvents() {
            static SDL_Event inputEvent;
            while ( SDL_PollEvent(&inputEvent) != 0 ) {
                handleEvent(inputEvent);
            }
            handleKeyState(SDL_GetKeyboardState(nullptr));
        }

        virtual ~GameState() {};
        virtual void handleKeyState(const uint8_t *keyState [[maybe_unused]]) {};
        virtual void handleEvent(const SDL_Event &event [[maybe_unused]]) {
            if (event.type == SDL_QUIT) {
                isPlaying = false;
            }
        }
        virtual bool load() = 0;
        virtual void update() = 0;
        virtual void render() = 0;
    };

    using GameStateStack = std::stack<std::shared_ptr<GameState>>;

    class GameStateProcessor {
        GameStateStack gameStates;
        gtool::GameClock clock;

        bool isPlaying = true;

    public:
        GameStateProcessor(double msPerFrame = 16.) {
            clock.msPerUpdate = msPerFrame;
        }

        void initStates(std::function<void (GameStateStack &)> initFunction) {
            initFunction(gameStates);
        }

        void processStates();

        gtool::GameClock const &getClock() {
            return clock;
        }

        void quitGame() {
            isPlaying = false;
        }
    };
}

#endif
