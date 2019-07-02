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
    int atIndex = 0;
    SDL_Rect *position = nullptr;
    Tweening2DPoint positionTweener;

    Checker(sdl2::Colors playerColor, SDL_Rect *p, int x, int y)
        : color(playerColor)
        , position(p)
        , positionTweener{
            static_cast<double>(x),
            static_cast<double>(y),
            static_cast<double>(x),
            static_cast<double>(y)
          }
        {}

    Checker() {}

    void updateNextPosition(int x, int y) {
        positionTweener.xNext = x;
        positionTweener.yNext = y;
    }

    void move() {
        positionTweener.lerp(0.06);
        positionTweener.fillRect(position);
    }
};


struct FirstState : public GameState {
    const GameClock *clock = gameStateProcessor.getClock();

    const uint8_t *key_state = nullptr;
    SDL_Event event = {0};
    int x, y;

    // these can be used to draw in batch
    std::array<SDL_Rect, 64> rects = {{0, 0, 0, 0}};
    std::vector<SDL_Rect> greenChecks{32};
    std::vector<SDL_Rect> redChecks{32};

    // metainfo structs that points to rects
    std::vector<Checker> checkers;

    Checker *selected = nullptr;

    int n_rows = 8;
    int checkerCellDim = 100; // h/w of the cell that can contain a checker
    int checkerDim = 60; // h/w of the rect that is inside a cell

    sdl2::Colors playingColor = sdl2::Colors::GREEN;

    bool contains(const SDL_Rect &r, const int x, const int y) const {
        return (r.x <= x && x <= r.x + r.w) && (r.y <= y && y <= r.y + r.h);
    }

    void handleInput() override {

        while ( SDL_PollEvent(&event) != 0 ) {
            if ( event.type == SDL_QUIT ) {
                isPlaying = false;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                auto mouseButtonState = SDL_GetMouseState(&x, &y);
                if ( mouseButtonState & SDL_BUTTON(SDL_BUTTON_LEFT) ) {

                    if (selected == nullptr) {
                        for (auto &checker : checkers) {
                            auto &r = rects[checker.atIndex];
                            if ( contains(r, x, y) && playingColor == checker.color ) {
                                selected = &checker;
                            }
                        }
                    } else {
                        int selectedGridIndex = selected->atIndex;
                        auto escape = false;

                        // looking through the neighbourhood and switching out the pointer to
                        // the cell only if the mouse was within a neighbour cell
                        for (int i = -1; i <= 1 && !escape; ++i) {
                            for (int j = -1; j <= 1 && !escape; ++j) {
                                int nextIndex = (selectedGridIndex + (n_rows * j) + i);

                                if ( 0 <= nextIndex && nextIndex < static_cast<int>(rects.size()) && nextIndex != selectedGridIndex ) {
                                    auto &r = rects[nextIndex];

                                    if ( contains(r, x, y) ) {
                                        selected->updateNextPosition(r.x + 20, r.y + 20);

                                        selected->atIndex = nextIndex;
                                        if (playingColor == sdl2::Colors::GREEN) {
                                            playingColor = sdl2::Colors::RED;
                                        } else {
                                            playingColor = sdl2::Colors::GREEN;
                                        }
                                        selected = nullptr;
                                        escape = true;
                                        break;
                                    }
                                }
                            }
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
        int i = 0, row = n_rows;
        for ( auto &r : rects ) {
            r.h = checkerCellDim;
            r.w = checkerCellDim;

            auto dx = r.w * (i % row);
            auto dy = r.h * (i / row);

            r.x = dx + 20;
            r.y = dy + 20;
            i++;
        }

        for (int i = 0; i < n_rows; ++i) {
            for (int j = 0; j < n_rows; ++j) {

                auto flatindex = i * static_cast<int>(n_rows) + j;
                auto &r = rects[flatindex];

                // GREEN upper player
                if ( i % 2 == 0 && i < (n_rows / 2) ) {
                    if (j % 2 == 0) {
                        greenChecks.emplace_back(SDL_Rect{(r.w * (j % n_rows)) + 40, r.y + 20, checkerDim, checkerDim});
                        auto c = Checker(sdl2::Colors::GREEN, &greenChecks[greenChecks.size() - 1], (r.w * (j % n_rows)) + 40, r.y + 20);
                        c.atIndex = flatindex;
                        checkers.emplace_back(c);
                        continue;
                    }
                } else if ( i % 2 != 0 && i < (n_rows / 2) - 1) {
                    if (j % 2 != 0) {
                        greenChecks.emplace_back(SDL_Rect{(r.w * (j % n_rows)) + 40, r.y + 20, checkerDim, checkerDim});
                        auto c = Checker(sdl2::Colors::GREEN, &greenChecks[greenChecks.size() - 1], (r.w * (j % n_rows)) + 40, r.y + 20);
                        c.atIndex = flatindex;
                        checkers.emplace_back(c);
                        continue;
                    }
                }

                // RED lower player
                if ( i % 2 == 0 && i > (n_rows / 2) ) {
                    if (j % 2 == 0) {
                        redChecks.emplace_back(SDL_Rect{(r.w * (j % n_rows)) + 40, r.y + 20, checkerDim, checkerDim});
                        auto c = Checker(sdl2::Colors::RED, &redChecks[redChecks.size() - 1], (r.w * (j % n_rows)) + 40, r.y + 20);
                        c.atIndex = flatindex;
                        checkers.emplace_back(c);
                        continue;
                    }
                } else if ( i % 2 != 0 && i > (n_rows / 2) ) {
                    if (j % 2 != 0) {
                        redChecks.emplace_back(SDL_Rect{(r.w * (j % n_rows)) + 40, r.y + 20, checkerDim, checkerDim});
                        auto c = Checker(sdl2::Colors::RED, &redChecks[redChecks.size() - 1], (r.w * (j % n_rows)) + 40, r.y + 20);
                        c.atIndex = flatindex;
                        checkers.emplace_back(c);
                        continue;
                    }
                }
            }
        }

        return true;
    }

    void update() override {
        for (auto &c : checkers) c.move();
    }

    void render() override {
        renderer.setColor(sdl2::Colors::WHITE);
        renderer.fillRect({0, 0, SCREEN_WIDTH, SCREEN_HEIGHT});

        renderer.setColor(sdl2::Colors::BLACK);
        renderer.drawRects(rects);

        renderer.setColor(sdl2::Colors::GREEN);
        renderer.drawRects(greenChecks);

        renderer.setColor(sdl2::Colors::RED);
        renderer.drawRects(redChecks);

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
