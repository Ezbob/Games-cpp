#pragma once

#include <optional>
#include <memory>
#include <vector>
#include "SDL.h"
#include "SDL_ttf.h"
#include "sdl2cpp/Renderer.hpp"
#include "sdl2cpp/Texture.hpp"
#include "sdl2cpp/Font.hpp"
#include "gametool/GameState.hpp"
#include "gametool/GameStateProcessor.hpp"

#include <iostream>

class BoardPlayState : public asa::GameState
{
private:
    asa::Renderer &renderer;
    asa::TTFFont &font;
    asa::GameStateProcessor &processor;
    asa::GameClock const&clock;

    std::shared_ptr<asa::GameState> pauseState;

    int screen_width;
    int screen_height;

    const double sec_per_frame;

    enum class PlayingColor
    {
        GREEN,
        RED
    } playingColor = PlayingColor::GREEN;

    struct Checker
    {
        PlayingColor color = PlayingColor::GREEN;
        double currentDegree = 1.0;
        SDL_Rect &position;
        SDL_Rect next;

        Checker(PlayingColor playerColor, SDL_Rect &p);

        void setNextPosition(int x, int y);
        void update(double sekPerFrame);
    };

    struct GridCell
    {
        SDL_Rect *container = nullptr;
        Checker *occubant = nullptr;
        int column;
        int row;
    };

    SDL_Point mouseClick;
    int nRedCheckers = 0, nGreenCheckers = 0;

    const static size_t n_tiles = 64;

    // these can be used to draw in batch
    SDL_Rect boardContainers[n_tiles];
    SDL_Rect boardBlackTiles[n_tiles / 2];

    GridCell cells[n_tiles];

    std::vector<SDL_Rect> greenChecks{32};
    std::vector<SDL_Rect> redChecks{32};

    // metainfo structs that points to boardContainers
    std::vector<std::shared_ptr<Checker>> checkers;

#if _DEBUG
    std::vector<asa::Texture> debugText;
#endif

    GridCell *selected = nullptr;

    int n_rows = 8;
    int checkerCellDim = 100; // h/w of the cell that can contain a checker
    int checkerDim = 60;      // h/w of the rect that is inside a cell
    int padding = 20;

    std::optional<asa::Texture> greenTurn;
    std::optional<asa::Texture> redTurn;

    void switchTurn(void);
    void findSelected(void);
    void initChecker(PlayingColor checkerColor, int flatindex, int checkerX, int checkerY);
    void doMoveToEmpty(GridCell &target);
    void doOvertake(GridCell &taken, GridCell &position);
    bool tryToOvertake(GridCell &clickedGridCell, int xOffset, int yOffset);
    bool tryToMove(int xOffset, int yOffset);
    void updateSelected(void);

public:
    BoardPlayState(asa::Renderer &r, asa::GameStateProcessor &p, asa::TTFFont &f, const asa::GameClock &clock, int swidth, int sheight);

    void handleEvent(const SDL_Event &event) override;
    void handleKeyState(const uint8_t *state) override;
    bool load(void) override;
    void render(void) override;
    void update(void) override;
};
