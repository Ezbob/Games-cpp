#include "SDL.hpp"
#include "SDL_extra.hpp"
#include "Animator.hpp"
#include "GameState.hpp"
#include "GameClock.hpp"
#include "Tweening.hpp"
#include <iostream>
#include <stack>

const int SCREEN_WIDTH = 840;
const int SCREEN_HEIGHT = 480;

sdl2cpp::SDLGlobals globals;
sdl2cpp::SDLWindow window;
sdl2cpp::SDLRenderer renderer;

SDL_Event event;

const double MS_PER_UPDATE = 15.0;
    // how much time the update step has been given (in ms)
    // this parameter has to be minimized, but if it is too small
    // then the game update (physics, AI, etc) will never catch up.
    // Also > 0 value

GameClock clock;

struct FirstState : public GameState {

    void handleInput() override {
        while ( SDL_PollEvent(&event) != 0 ) {
            if ( event.type == SDL_QUIT ) {
                globals.isPlaying = false;
            }
        }
        const uint8_t *currentKeyState = SDL_GetKeyboardState(nullptr);
    }

    bool load() override {
        return true;
    }

    void update() override {}

    void render() override {}

    ~FirstState() = default;
};

bool init() {
    bool result = true;

    if ( globals.init(SDL_INIT_VIDEO | SDL_INIT_TIMER) ) {
        globals.loadExternLib(sdl2cpp::SDLExternLibs::SDL_IMAGE, IMG_INIT_PNG);
        globals.loadExternLib(sdl2cpp::SDLExternLibs::SDL_TTF);
        window.loadWindow("SDL Tutorial",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN);

        renderer = window.getRenderer(-1, SDL_RENDERER_ACCELERATED);
        renderer.setColor(0xFF, 0xFF, 0xFF, 0xFF);
        //SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest"); // basically the anti-aliasing
    }

    result = globals.is_initialized && window.isLoaded();

    return result;
}

bool initStates(std::stack<std::shared_ptr<GameState>> &stack) {
    stack.emplace(new FirstState());

    return true;
}

int WinMain() {

    std::stack<std::shared_ptr<GameState>> gameStack;

    if ( !init() ) {
        return 1;
    }

    if ( !initStates(gameStack) ) {
        return 1;
    }

    while ( globals.isPlaying && !gameStack.empty() ) {
        auto state = gameStack.top();

        if ( state->load() ) {
            while ( globals.isPlaying && state->isPlaying ) {
                state->handleInput();

                while ( clock.lag >= MS_PER_UPDATE) {
                    state->update();
                    clock.lag -= MS_PER_UPDATE;
                }

                state->prerender();
                state->render();

                clock.tick();
            }
        }

        gameStack.pop();
    }

    return 0;
}
