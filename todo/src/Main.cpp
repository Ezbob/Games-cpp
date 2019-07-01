#include "SDL.hpp"
#include "SDL_extra.hpp"
#include "Animator.hpp"
#include "GameState.hpp"
#include "GameClock.hpp"
#include "Tweening.hpp"
#include <iostream>
#include <stack>
#include <vector>
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

struct Checker {
    sdl2::Colors color = sdl2::Colors::GREEN;
    int playerId = 0;
    SDL_Rect p;
    Tweening2DPoint pmove;

    Checker(sdl2::Colors playerColor, int x, int y, int w = 60, int h = 60)
        : color(playerColor)
        , p{x, y, w, h}
        , pmove{
            static_cast<double>(x), 
            static_cast<double>(y), 
            static_cast<double>(x),
            static_cast<double>(y)
          } 
        {}
    
    Checker() {}
    
    void move() {
        pmove.lerp(0.06);
        pmove.fill(p);
    }

    void draw() {
        renderer.setColor(color);
        renderer.fillRect(p);
    }
};


struct FirstState : public GameState {
    const GameClock *clock = gameStateProcessor.getClock();

    const uint8_t *key_state = nullptr;
    SDL_Event event = {0};

    std::array<SDL_Rect, 64> rects = {{0, 0, 0, 0}};

    std::vector<Checker> checkers;

    //Checker c {40, 40};

    int checkerRectDim = 100;
    int x, y;
    bool isPlayer1Playing = true;

    void handleInput() override {

        while ( SDL_PollEvent(&event) != 0 ) {
            if ( event.type == SDL_QUIT ) {
                isPlaying = false;
            } 
            /*
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                auto mouseButtonState = SDL_GetMouseState(&x, &y);
                if ( mouseButtonState & SDL_BUTTON(SDL_BUTTON_LEFT) ) {
                    for (auto rect : rects) {
                        if ( (rect.x <= x && x <= rect.x + rect.w) &&
                                (rect.y <= y && y <= rect.y + rect.h)
                        ) {
                            c.pmove.xNext = rect.x + 20;
                            c.pmove.yNext = rect.y + 20;
                        }
                    }
                }
            }
            */
        }
        key_state = SDL_GetKeyboardState(nullptr);

        if ( key_state[SDL_SCANCODE_ESCAPE] ) {
            isPlaying = false;
        }
    }

    bool load() override {
        int i = 0, row = 8;
        for ( auto &r : rects ) {
            r.h = checkerRectDim;
            r.w = checkerRectDim;

            auto dx = r.w * (i % row);
            auto dy = r.h * (i / row);

            r.x = dx + 20;
            r.y = dy + 20;

            if ( i % (row * 2) == 0 && dy < ((SCREEN_HEIGHT / 2) - r.h) ) {
                for (auto j = 0; j < row; ++j)
                    if (j % 2 == 0)
                        checkers.emplace_back(Checker(sdl2::Colors::GREEN, (r.w * (j % row)) + 40, r.y + 20));
            } else if ( i % (row * 2 - 1) == 0 && dy < ((SCREEN_HEIGHT / 2) - r.h * 2) ) {
                for (auto j = 0; j < row - 1; ++j)
                    if (j % 2 == 0)
                        checkers.emplace_back(Checker(sdl2::Colors::GREEN, (r.w * (j % row)) + 140, r.y + 20));
            }

            if ( i % (row * 2) == 0 && dy > ((SCREEN_HEIGHT / 2)) ) {
                for (auto j = 0; j < row; ++j)
                    if (j % 2 == 0)
                        checkers.emplace_back(Checker(sdl2::Colors::RED, (r.w * (j % row)) + 40, r.y + 20));
            } else if ( i % (row * 2 - 1) == 0 && dy > ((SCREEN_HEIGHT / 2)) ) {
                for (auto j = 0; j < row - 1; ++j)
                    if (j % 2 == 0)
                        checkers.emplace_back(Checker(sdl2::Colors::RED, (r.w * (j % row)) + 140, r.y + 20));
            }

            i++;
        }

        return true;
    }

    void update() override {
        for (auto c : checkers) c.move();
    }

    void render() override {
        renderer.setColor(sdl2::Colors::WHITE);
        renderer.fillRect({0, 0, SCREEN_WIDTH, SCREEN_HEIGHT});

        renderer.setColor(sdl2::Colors::BLACK);
        renderer.drawRects(rects);

        for (auto c : checkers) c.draw();

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

        renderer = window.getRenderer();

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
