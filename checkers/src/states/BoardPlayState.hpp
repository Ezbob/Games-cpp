
#ifndef _HEADER_FILE_BoardPlayState_20200413202312_
#define _HEADER_FILE_BoardPlayState_20200413202312_

#include <optional>
#include <memory>
#include <vector>
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL.hpp"
#include "SDL_extra.hpp"
#include "GameTool\Tweening.hpp"
#include "GameTool\GameState.hpp"
#include "GameTool\GameStateProcessor.hpp"

class BoardPlayState : public gtool::GameState
{
private:
    sdl2::Renderer &renderer;
    sdl2::TTFFont &font;
    gtool::GameStateProcessor &processor;

    std::shared_ptr<gtool::GameState> pauseState;

    int screen_width;
    int screen_height;

    enum class PlayingColor {
        GREEN,
        RED
    } playingColor = PlayingColor::GREEN;

    struct Checker
    {
        PlayingColor color = PlayingColor::GREEN;
        double lerpDegree = 0.2;
        SDL_Rect *position = nullptr;
        gtool::Tweening2DPoint positionTweener;

        Checker(PlayingColor playerColor, SDL_Rect &p)
            : color(playerColor), position(&p), positionTweener(p.x, p.y)
        {
        }

        void updateNextPosition(int x, int y)
        {
            positionTweener.setNext(x, y);
        }

        void move()
        {
            positionTweener.lerp(lerpDegree);
            positionTweener.fillRect(position);
        }
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
    std::vector<sdl2::Texture> debugText;
#endif

    GridCell *selected = nullptr;

    int n_rows = 8;
    int checkerCellDim = 100; // h/w of the cell that can contain a checker
    int checkerDim = 60;      // h/w of the rect that is inside a cell
    int padding = 20;

    std::optional<sdl2::Texture> greenTurn;
    std::optional<sdl2::Texture> redTurn;

    void switchTurn(void);
    void findSelected(void);
    void initChecker(PlayingColor checkerColor, int flatindex, int checkerX, int checkerY);
    void doMoveToEmpty(GridCell &target);
    void doOvertake(GridCell &taken, GridCell &position);
    bool tryToOvertake(GridCell &clickedGridCell, int xOffset, int yOffset);
    bool tryToMove(int xOffset, int yOffset);
    void updateSelected(void);

public:
    BoardPlayState(sdl2::Renderer &r, gtool::GameStateProcessor &p, sdl2::TTFFont &f, int swidth, int sheight);

    void handleEvent(const SDL_Event &event) override;
    void handleKeyState(const uint8_t *state) override;
    bool load(void) override;
    void render(void) override;
    void update(void) override;
};

#endif