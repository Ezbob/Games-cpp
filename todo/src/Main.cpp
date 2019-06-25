#include "SDL.hpp"
#include "SDL_extra.hpp"
#include "Animator.hpp"
#include "GameState.hpp"
#include "GameClock.hpp"
#include "Tweening.hpp"
#include <iostream>
#include <stack>
#include <array>
#include "Kdtree.hpp"

#if _WIN32
    #define MAIN_NAME WinMain
#else
    #define MAIN_NAME main
#endif

const int SCREEN_WIDTH = 840;
const int SCREEN_HEIGHT = 840;

sdl2::Globals globals;
sdl2::Window window;
sdl2::Renderer renderer;

GameStateProcessor gameStateProcessor { 18. };

struct FirstState : public GameState {

    sdl2::Texture background = renderer.createTexture();
    const GameClock *clock = gameStateProcessor.getClock();

    const uint8_t *key_state = nullptr;
    SDL_Event event = {0};

    std::array<SDL_Rect, 64> rects = {{0, 0, 0, 0}};

    SDL_Rect p = {40, 40, 60, 60};
    Tweening2DPoint pmove = {40., 40., 40., 40.};

    int checkerRectDim = 100;

    bool has_clicked = false;
    int x, y;

    void handleInput() override {

        while ( SDL_PollEvent(&event) != 0 ) {
            if ( event.type == SDL_QUIT ) {
                isPlaying = false;
            } else if ( event.type == SDL_KEYDOWN ) {
                auto code = event.key.keysym.sym;
                auto repeat = event.key.repeat;

                if ( !repeat ) {
                    switch ( code ) {
                        case SDLK_DOWN:
                            if (
                                p.y <= (SCREEN_HEIGHT - p.h - checkerRectDim)
                            ) {
                                pmove.yNext += checkerRectDim;
                            } else {
                                pmove.y += 20.;
                            }
                            break;
                        case SDLK_UP:
                            if (
                                p.y >= p.h
                            ) {
                                pmove.yNext -= checkerRectDim;
                            } else {
                                pmove.y -= 20.;
                            }
                            break;
                       case SDLK_RIGHT:
                            if (
                                p.x <= (SCREEN_WIDTH - p.w - checkerRectDim)
                            ) {
                                pmove.xNext += checkerRectDim;
                            } else {
                                pmove.x += 20.;
                            }
                            break;
                        case SDLK_LEFT:
                            if (
                                p.x >= p.w
                            ) {
                                pmove.xNext -= checkerRectDim;
                            } else {
                                pmove.x -= 20.;
                            }
                            break;
                        default:
                            break;
                    }
                }
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                auto mouseButtonState = SDL_GetMouseState(&x, &y);
                if ( mouseButtonState & SDL_BUTTON(SDL_BUTTON_LEFT) ) {
                    for (auto rect : rects) {
                        if ( (rect.x <= x && x <= rect.x + rect.w) &&
                                (rect.y <= y && y <= rect.y + rect.h)
                        ) {
                            pmove.xNext = rect.x + 20;
                            pmove.yNext = rect.y + 20;
                            has_clicked = true;
                        }
                    }
                }
            }
        }
        key_state = SDL_GetKeyboardState(nullptr);

        if ( key_state[SDL_SCANCODE_ESCAPE] ) {
            isPlaying = false;
        }
    }

    bool load() override {
        int i = 0, row = 8;
        for (auto &r : rects) {
            r.h = checkerRectDim;
            r.w = checkerRectDim;

            r.x = r.w * (i % row) + 20;
            r.y = r.h * (i / row) + 20;

            i++;
        }

        return true;
    }

    void update() override {

        pmove.lerp(0.06);
        pmove.fill(p);
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

bool sdlInit() {
    if ( globals.init(SDL_INIT_VIDEO | SDL_INIT_TIMER) ) {
        globals.loadExternLib(sdl2::ExternLibs::SDL_IMAGE, IMG_INIT_PNG);
        globals.loadExternLib(sdl2::ExternLibs::SDL_TTF);

        window.loadWindow(
            "Checkers game",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN
        );

        renderer = window.getRenderer(0);

        renderer.setColor(sdl2::Colors::WHITE);
    }

    return globals.is_initialized && window.isLoaded();
}

int MAIN_NAME() {

    if ( !sdlInit() ) {
        return 1;
    }

    gameStateProcessor.initStates([](auto &stack) {
        stack.emplace(new FirstState());
    });

    gameStateProcessor.processStates();

    return 0;
}
