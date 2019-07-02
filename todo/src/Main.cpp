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

struct GridCell;

struct Checker {
    sdl2::Colors color = sdl2::Colors::GREEN;
    struct GridCell *grid = nullptr;
    bool isEmpty = true;
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

struct GridCell {
    SDL_Rect *dim;
    int i;
};

struct FirstState : public GameState {
    const GameClock *clock = gameStateProcessor.getClock();

    const uint8_t *key_state = nullptr;
    SDL_Event event = {0};

    std::array<SDL_Rect, 64> rects = {{0, 0, 0, 0}}; // this can be used to draw in batch
    std::array<GridCell, 64> cells;

    std::vector<Checker> checkers;

    Checker *selected = nullptr;

    int n_rows = 8;

    int checkerRectDim = 100;
    int x, y;
    sdl2::Colors playingColor = sdl2::Colors::GREEN;

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
                            if ( (checker.p.x <= x && x <= checker.p.x + checker.p.w) &&
                                (checker.p.y <= y && y <= checker.p.y + checker.p.h) &&
                                playingColor == checker.color ) {
                                selected = &checker;
                                std::cout << "selected\n";
                            }
                        }
                    } else {

                        auto &selectedGrid = selected->grid;

                        for (int i = -1; i <= 1; ++i) {
                            for (int j = -1; j <= 1; ++j) {
                                int neighbour_i = (selectedGrid->i + (n_rows * j) + i);
                                std::cout << selectedGrid->i << " + " << (n_rows *j) << " + " << i << " = " << neighbour_i << "\n";
                                if (
                                    neighbour_i >= 0 &&
                                    neighbour_i < static_cast<int>(cells.size()) // &&
                                    //std::abs(neighbour_i - selectedGrid->i) <= 2
                                ) {
                                    //std::cout << "here " << neighbour_i << "\n";
                                    auto &r = rects[neighbour_i];

                                    if ((r.x <= x && x <= r.x + r.w) &&
                                        (r.y <= y && y <= r.y + r.h)) {
                                        selected->pmove.xNext = r.x + 20;
                                        selected->pmove.yNext = r.y + 20;
                                        std::cout << "Found neighbour cell\n";
                                        selected = nullptr;
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
            auto &cell = cells[i];
            r.h = checkerRectDim;
            r.w = checkerRectDim;

            auto dx = r.w * (i % row);
            auto dy = r.h * (i / row);

            r.x = dx + 20;
            r.y = dy + 20;

            cell.dim = &rects[i];
            cell.i = i;

            i++;
        }

        for (int i = 0; i < n_rows; ++i) {
            for (int j = 0; j < n_rows; ++j) {

                auto flatindex = i * static_cast<int>(n_rows) + j;
                auto &cell = cells[flatindex];
                auto &r = rects[flatindex];


                // GREEN upper player
                if ( i % 2 == 0 && i < (n_rows / 2) ) {
                    if (j % 2 == 0) {
                        auto c = Checker(sdl2::Colors::GREEN, (r.w * (j % n_rows)) + 40, r.y + 20);
                        c.grid = &cell;
                        checkers.emplace_back(c);
                    }
                }

                if ( i % 2 != 0 && i < (n_rows / 2) - 1) {
                    if (j % 2 != 0) {
                        auto c = Checker(sdl2::Colors::GREEN, (r.w * (j % n_rows)) + 40, r.y + 20);
                        c.grid = &cell;
                        checkers.emplace_back(c);
                    }
                }

                // RED lower player
                if ( i % 2 == 0 && i > (n_rows / 2) ) {
                    if (j % 2 == 0) {
                        auto c = Checker(sdl2::Colors::RED, (r.w * (j % row)) + 40, r.y + 20);
                        c.grid = &cell;
                        checkers.emplace_back(c);
                    }
                }
                if ( i % 2 != 0 && i > (n_rows / 2) ) {
                    if (j % 2 != 0) {
                        auto c = Checker(sdl2::Colors::RED, (r.w * (j % row)) + 40, r.y + 20);
                        c.grid = &cell;
                        checkers.emplace_back(c);
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

        for (auto &c : checkers) c.draw();

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
