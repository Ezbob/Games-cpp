#include "SDL.hpp"
#include "SDL_extra.hpp"
#include "Animator.hpp"
#include "GameState.hpp"
#include "GameClock.hpp"
#include <iostream>
#include <stack>
#include <cmath>

const int SCREEN_WIDTH = 840;
const int SCREEN_HEIGHT = 480;

SDLGlobals globals;
SDLWindow window;
SDLRenderer renderer;

SDL_Event event;

std::stack<std::shared_ptr<GameState>> gameStack;

template<typename T>
constexpr void lerp(T &out, double f = 0) {
    double t = 1.0 - f;
    out.x[0] = out.x[0] * t + out.x[1] * f;
    out.y[0] = out.y[0] * t + out.y[1] * f;
}

const double MS_PER_UPDATE = 15.0;
    // how much time the update step has been given (in ms)
    // this parameter has to be minimized, but if it is too small
    // then the game update (physics, AI, etc) will never catch up.
    // Also > 0 value

GameClock clock;

bool init() {
    bool result = true;

    if ( globals.init(SDL_INIT_VIDEO | SDL_INIT_TIMER) ) {
        globals.loadExternLib(SDLExternLibs::SDL_IMAGE, IMG_INIT_PNG);
        globals.loadExternLib(SDLExternLibs::SDL_TTF);
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

struct FirstState : public GameState {
    void handleInput() override {
        while ( SDL_PollEvent(&event) != 0 ) {
            if ( event.type == SDL_QUIT ) {
                globals.is_playing = false;
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


int WinMain() {

    if ( !init() ) {
        return 1;
    }

    gameStack.emplace(new FirstState());

    while ( !gameStack.empty() ) {
        auto state = gameStack.top();
        gameStack.pop();

        if ( state->load() ) {
            while ( globals.is_playing ) {
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
    }

    return 0;
}
