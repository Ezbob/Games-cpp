#include "SDL.hpp"
#include "SDL_extra.hpp"
#include "Animator.hpp"
#include "GameState.hpp"
#include "GameClock.hpp"
#include "Actor.hpp"
#include "Tweening.hpp"
#include <iostream>
#include <stack>
#include <array>


const int SCREEN_WIDTH = 840;
const int SCREEN_HEIGHT = 840;

sdl2::Globals globals;
sdl2::Window window;
sdl2::Renderer renderer;


const double MS_PER_UPDATE = 16.0;
    // how much time the update step has been given (in ms)
    // this parameter has to be minimized, but if it is too small
    // then the game update (physics, AI, etc) will never catch up.
    // Also > 0 value

GameClock clock;

struct FirstState : public GameState {

    sdl2::Texture background = renderer.createTexture();

    const uint8_t *key_state = nullptr;
    SDL_Event event = {0};

    std::array<SDL_Rect, 64> rects = {{0, 0, 0, 0}};

    SDL_Rect p = {40, 40, 60, 60};
    Tweening2DPoint<double> pmove = Tweening2DPoint<double>({40., 40., 40., 40.});

    bool is_down = false;

    void handleInput() override {
        while ( SDL_PollEvent(&event) != 0 ) {
            if ( event.type == SDL_QUIT ) {
                globals.isPlaying = false;
            } else if ( event.type == SDL_KEYDOWN ) {
                SDL_Keycode code = event.key.keysym.sym;
                auto repeat = event.key.repeat;

                if ( !repeat ) {
                    switch ( code ) {
                        case SDLK_DOWN:
                            if (
                                p.y <= (SCREEN_HEIGHT - 200)
                            ) {
                                pmove.yNext() += 100.;
                            } else {
                                pmove.y() += 20.;
                            }
                            break;
                        case SDLK_UP:
                            if (
                                p.y >= p.h
                            ) {
                                pmove.yNext() -= 100.;
                            } else {
                                pmove.y() -= 20.;
                            }
                            break;
                       case SDLK_RIGHT:
                            if (
                                p.x <= (SCREEN_WIDTH - 200)
                            ) {
                                pmove.xNext() += 100.;
                            } else {
                                pmove.x() += 20.;
                            }
                            break;
                        case SDLK_LEFT:
                            if (
                                p.x >= p.w
                            ) {
                                pmove.xNext() -= 100.;
                            } else {
                                pmove.x() -= 20.;
                            }
                            break;
                        default:
                            break;
                    }
                }
            }
        }
        key_state = SDL_GetKeyboardState(nullptr);

        if ( key_state[SDL_SCANCODE_ESCAPE] ) {
            globals.isPlaying = false;
        }
    }

    bool load() override {
        int i = 0, row = 8;
        for (auto &r : rects) {
            r.h = 100;
            r.w = 100;

            r.x = r.w * (i % row) + 20;
            r.y = r.h * (i / row) + 20;

            i++;
        }

        return true;
    }

    void update() override {

        pmove.lerp(0.16);
        pmove.fillRect(p);
    }

    void render() override {

        renderer.setColor(sdl2::Colors::WHITE);
        renderer.fillRect({0, 0, SCREEN_WIDTH, SCREEN_HEIGHT});

        renderer.setColor(sdl2::Colors::BLACK);
        renderer.drawRects(rects);

        renderer.setColor(sdl2::Colors::GREEN);
        renderer.fillRect(p);

        renderer.updateScreen();
    }
};

bool init() {
    bool result = true;

    if ( globals.init(SDL_INIT_VIDEO | SDL_INIT_TIMER) ) {
        globals.loadExternLib(sdl2::ExternLibs::SDL_IMAGE, IMG_INIT_PNG);
        globals.loadExternLib(sdl2::ExternLibs::SDL_TTF);

        window.loadWindow(
            "SDL Tutorial",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN
        );

        renderer = window.getRenderer(SDL_RENDERER_ACCELERATED);
        renderer.setColor(sdl2::Colors::BLACK);
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

                while ( clock.lag >= MS_PER_UPDATE ) {
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
