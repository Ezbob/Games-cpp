#pragma once

#include <optional>
#include <memory>
#include <vector>
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL2cpp\Renderer.hpp"
#include "SDL2cpp\Texture.hpp"
#include "SDL2cpp\Font.hpp"
#include "GameTool\Tweening2DPoint.hpp"
#include "GameTool\GameState.hpp"
#include "GameTool\GameStateProcessor.hpp"

#include <iostream>

class BoardPlayState : public asa::GameState
{
private:
    asa::Renderer &renderer;
    asa::TTFFont &font;
    asa::GameStateProcessor &processor;
    const asa::GameClock &clock;

    std::shared_ptr<asa::GameState> pauseState;

    int screen_width;
    int screen_height;

    enum class PlayingColor
    {
        GREEN,
        RED
    } playingColor = PlayingColor::GREEN;

    struct Checker
    {
        const asa::GameClock &clock;
        PlayingColor color = PlayingColor::GREEN;

        double updateStep = 16.66;
        double currentDegree = 1.0;
        SDL_Rect *position = nullptr;

        asa::Tweening2DPoint positionTweener;

        Checker(PlayingColor playerColor, SDL_Rect &p, const asa::GameClock &c);

        void updateNextPosition(int x, int y);

        void tick(void);

        void move(void);
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

