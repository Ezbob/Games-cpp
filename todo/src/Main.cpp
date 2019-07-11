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

#if defined(_WIN32)
    #define MAIN_NAME WinMain
#else
    #define MAIN_NAME main
#endif

const int SCREEN_WIDTH = 840;
const int SCREEN_HEIGHT = 860;

sdl2::Globals globals;
sdl2::Window window;
sdl2::Renderer renderer;
sdl2::TTFFont font;

GameStateProcessor gameStateProcessor { 18. };

class FirstState : public GameState {
    const GameClock *clock = gameStateProcessor.getClock();

    struct Checker {
        sdl2::Colors color = sdl2::Colors::GREEN;
        double lerpDegree = 0.06;
        SDL_Rect *position = nullptr;
        Tweening2DPoint positionTweener;

        Checker(sdl2::Colors playerColor, SDL_Rect &p)
            : color(playerColor)
            , position(&p)
            , positionTweener{
                static_cast<double>(p.x),
                static_cast<double>(p.y),
                static_cast<double>(p.x),
                static_cast<double>(p.y)
            }
            {}

        void updateNextPosition(int x, int y) {
            positionTweener.xNext = x;
            positionTweener.yNext = y;
        }

        void move() {
            positionTweener.lerp(lerpDegree);
            positionTweener.fillRect(position);
        }
    };

    struct GridCell {
        SDL_Rect *container = nullptr;
        Checker *occubant = nullptr;
        int column;
        int row;
    };

    const uint8_t *key_state = nullptr;
    SDL_Event event = {0};
    int x, y;

    const static size_t n_tiles = 64;

    // these can be used to draw in batch
    SDL_Rect boardContainers[n_tiles];
    GridCell cells[n_tiles];

    std::vector<SDL_Rect> greenChecks{32};
    std::vector<SDL_Rect> redChecks{32};

    // metainfo structs that points to boardContainers
    std::vector<std::shared_ptr<Checker>> checkers;
    std::vector<sdl2::Texture> text;

    bool is_findingSelected = false;
    GridCell *selected = nullptr;

    int n_rows = 8;
    int checkerCellDim = 100; // h/w of the cell that can contain a checker
    int checkerDim = 60; // h/w of the rect that is inside a cell

    sdl2::Colors playingColor = sdl2::Colors::GREEN;
    sdl2::Texture greenTurn = renderer.createTexture();
    sdl2::Texture redTurn = renderer.createTexture();
    sdl2::Texture *currentText = nullptr;

    bool contains(const SDL_Rect &r, const int x, const int y) const {
        return (r.x <= x && x <= r.x + r.w) && (r.y <= y && y <= r.y + r.h);
    }

    bool contains(const SDL_Rect *r, const int x, const int y) const {
        return (r->x <= x && x <= r->x + r->w) && (r->y <= y && y <= r->y + r->h);
    }

    void switchTurn() {
        if (playingColor == sdl2::Colors::GREEN) {
            playingColor = sdl2::Colors::RED;
            currentText = &redTurn;
        } else {
            playingColor = sdl2::Colors::GREEN;
            currentText = &greenTurn;
        }
    }

    void findSelected() {
        for (int i = 0; i < n_rows; i++) {
            for (int j = 0; j < n_rows; ++j) {

                int index = i * n_rows + j;
                auto &gridCell = cells[index];

                if ( contains(gridCell.container, x, y)
                    && gridCell.occubant != nullptr
                    && gridCell.occubant->color == playingColor
                    && gridCell.occubant->position->w != 0
                    && gridCell.occubant->position->h != 0 ) {
                    selected = &gridCell;
                    return;
                }
            }
        }
    }

    void initChecker(sdl2::Colors checkerColor, int flatindex, int checkerX, int checkerY) {
        auto checkerRect = SDL_Rect {
            checkerX,
            checkerY,
            checkerDim,
            checkerDim
        };

        if (checkerColor == sdl2::Colors::GREEN) {
            greenChecks.emplace_back(checkerRect);
            checkers.emplace_back(std::make_shared<Checker>(
                checkerColor,
                greenChecks[greenChecks.size() - 1]
            ));
            cells[flatindex].occubant = checkers[checkers.size() - 1].get();
        } else {
            redChecks.emplace_back(checkerRect);
            checkers.emplace_back(std::make_shared<Checker>(
                checkerColor,
                redChecks[redChecks.size() - 1]
            ));
            cells[flatindex].occubant = checkers[checkers.size() - 1].get();
        }
    }

    void doMoveToEmpty(GridCell &target) {
        Checker *source = selected->occubant;

        source->updateNextPosition(
            target.container->x + 20,
            target.container->y + 20
        );

        switchTurn();

        target.occubant = source;
        selected->occubant = nullptr;
        selected = nullptr;
    }

    void doOvertake(GridCell &taken, GridCell &position) {
        Checker *source = selected->occubant;

        source->updateNextPosition(
            position.container->x + 20,
            position.container->y + 20
        );

        position.occubant = source;
        selected->occubant = nullptr;

        taken.occubant->position->h = 0;
        taken.occubant->position->w = 0;
        taken.occubant = nullptr;

        selected = nullptr;
    }

    bool tryToOvertake(GridCell &clickedGridCell, int xOffset, int yOffset) {
        int y2diff = selected->row + (yOffset * 2);
        int x2diff = selected->column + (xOffset * 2);
        int nextNextIndex = y2diff * n_rows + x2diff;

        if (
            x2diff < 0 ||
            x2diff >= static_cast<int>(n_rows) ||
            y2diff < 0 ||
            y2diff >= static_cast<int>(n_rows)
        ) {
            return false;
        }

        if ( 0 <= nextNextIndex && nextNextIndex < static_cast<int>(n_tiles) ) {
            if (auto &nextNextCell = cells[nextNextIndex]; nextNextCell.occubant == nullptr) {
                doOvertake(clickedGridCell, nextNextCell);
                return true;
            }
        }

        return false;
    }

    bool tryToMove(int xOffset, int yOffset) {
        int ydiff = selected->row + yOffset;
        int xdiff = selected->column + xOffset;
        int nextIndex = ydiff * n_rows + xdiff;

        if ( 0 <= nextIndex && nextIndex < static_cast<int>(n_tiles) ) {
            auto &gridCell = cells[nextIndex];

            if (
                xdiff < 0 ||
                xdiff >= static_cast<int>(n_rows) ||
                ydiff < 0 ||
                ydiff >= static_cast<int>(n_rows)
            ) {
                return false;
            }

            if ( contains(gridCell.container, x, y) ) {
                if ( gridCell.occubant == nullptr ) {
                    doMoveToEmpty(gridCell);
                    return true;
                } else if ( gridCell.occubant->color != selected->occubant->color ) {
                    return tryToOvertake(gridCell, xOffset, yOffset);
                }
            }
        }

        return false;
    }

    void updateSelected() {
        if ( tryToMove( 1,  1) ) return;
        if ( tryToMove(-1,  1) ) return;
        if ( tryToMove( 1, -1) ) return;
        if ( tryToMove(-1, -1) ) return;
        selected = nullptr;
    }

public:
    void handleInput() override {

        while ( SDL_PollEvent(&event) != 0 ) {
            if ( event.type == SDL_QUIT ) {
                isPlaying = false;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                auto mouseButtonState = SDL_GetMouseState(&x, &y);
                if ((mouseButtonState & SDL_BUTTON(SDL_BUTTON_LEFT))) {
                    if ( selected == nullptr ) {
                        findSelected();
                    } else {
                        updateSelected();
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

        font.loadTTF("assets/consola.ttf", 24);

        redTurn = sdl2::loadSolidText(renderer,
            "Red's turn",
            (TTF_Font *) font,
            SDL_Color {
                0xFF, 0x00, 0x00, 0xff
            });

        greenTurn = sdl2::loadSolidText(renderer,
            "Green's turn",
            (TTF_Font *) font,
            SDL_Color {
                0x00, 0xFF, 0x00, 0xff
            });

        currentText = &greenTurn;

        for (int i = 0; i < n_rows; ++i) {
            for (int j = 0; j < n_rows; ++j) {

                auto flatindex = i * static_cast<int>(n_rows) + j;
                auto &boardContainer = boardContainers[flatindex];

                boardContainer.h = checkerCellDim;
                boardContainer.w = checkerCellDim;

                boardContainer.x = boardContainer.w * (flatindex % n_rows) + 20;
                boardContainer.y = boardContainer.h * (flatindex / n_rows) + 20;

                cells[flatindex].container = &boardContainer;
                cells[flatindex].column = j;
                cells[flatindex].row = i;

                text.emplace_back(sdl2::loadSolidText(renderer,
                    "(" + std::to_string(i) + ", " + std::to_string(j) + ")",
                    (TTF_Font *) font,
                    SDL_Color {
                        0x00, 0x00, 0x00, 0xff
                    }
                ));

                // GREEN upper player
                if ( i % 2 == 0 && i < (n_rows / 2) ) {
                    if (j % 2 == 0) {
                        initChecker(
                            sdl2::Colors::GREEN,
                            flatindex,
                            (boardContainer.w * (j % n_rows)) + 40,
                            boardContainer.y + 20
                        );
                        continue;
                    }
                } else if ( i % 2 != 0 && i < (n_rows / 2) - 1) {
                    if (j % 2 != 0) {
                        initChecker(
                            sdl2::Colors::GREEN,
                            flatindex,
                            (boardContainer.w * (j % n_rows)) + 40,
                            boardContainer.y + 20
                        );
                        continue;
                    }
                }

                // RED lower player
                if ( i % 2 == 0 && i > (n_rows / 2) ) {
                    if (j % 2 == 0) {
                        initChecker(
                            sdl2::Colors::RED,
                            flatindex,
                            (boardContainer.w * (j % n_rows)) + 40,
                            boardContainer.y + 20
                        );
                        continue;
                    }
                } else if ( i % 2 != 0 && i > (n_rows / 2) ) {
                    if (j % 2 != 0) {
                        initChecker(
                            sdl2::Colors::RED,
                            flatindex,
                            (boardContainer.w * (j % n_rows)) + 40,
                            boardContainer.y + 20
                        );
                        continue;
                    }
                }
            }
        }

        return font.isLoaded();
    }

    void update() override {
        for (auto &c : checkers)
            c->move();
    }

    void render() override {
        renderer.setColor(sdl2::Colors::WHITE);
        renderer.fillRect({0, 0, SCREEN_WIDTH, SCREEN_HEIGHT});

        renderer.setColor(sdl2::Colors::BLACK);
        renderer.drawRects(boardContainers);

        renderer.setColor(sdl2::Colors::GREEN);
        renderer.drawRects(greenChecks);

        renderer.setColor(sdl2::Colors::RED);
        renderer.drawRects(redChecks);

        for (auto i = 0; i < n_rows; ++i) {
            for (auto j = 0; j < n_rows; ++j) {
                auto &t = text[i * n_rows + j];
                t.render(j * 100 + 22, i * 100 + 20);
            }
        }

        if (currentText != nullptr) {
            currentText->render(SCREEN_WIDTH / 2 - 85, SCREEN_HEIGHT - 32);
        }

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
