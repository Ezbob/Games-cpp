
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
    };

    PlayingColor playingColor = PlayingColor::GREEN;

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

    void switchTurn()
    {
        if (playingColor == PlayingColor::GREEN)
        {
            playingColor = PlayingColor::RED;
        }
        else
        {
            playingColor = PlayingColor::GREEN;
        }
    }

    void findSelected()
    {
        for (int i = 0; i < n_rows; i++)
        {
            for (int j = 0; j < n_rows; ++j)
            {

                int index = i * n_rows + j;
                auto &gridCell = cells[index];

                if (SDL_PointInRect(&mouseClick, gridCell.container) && gridCell.occubant != nullptr && gridCell.occubant->color == playingColor && !SDL_RectEmpty(gridCell.occubant->position))
                {
                    selected = &gridCell;
                    return;
                }
            }
        }
    }

    void initChecker(PlayingColor checkerColor, int flatindex, int checkerX, int checkerY)
    {

        std::shared_ptr<Checker> checker;
        if (checkerColor == PlayingColor::GREEN)
        {
            greenChecks.emplace_back(SDL_Rect{
                checkerX,
                checkerY,
                checkerDim,
                checkerDim});
            checker = std::make_shared<Checker>(
                checkerColor,
                greenChecks.back());
        }
        else
        {
            redChecks.emplace_back(SDL_Rect{
                checkerX,
                checkerY,
                checkerDim,
                checkerDim});
            checker = std::make_shared<Checker>(
                checkerColor,
                redChecks.back());
        }
        checkers.emplace_back(checker);
        cells[flatindex].occubant = checker.get();
    }

    void doMoveToEmpty(GridCell &target)
    {
        Checker *source = selected->occubant;

        source->updateNextPosition(
            target.container->x + padding,
            target.container->y + padding);

        switchTurn();

        target.occubant = source;
        selected->occubant = nullptr;
        selected = nullptr;
    }

    void doOvertake(GridCell &taken, GridCell &position)
    {
        Checker *source = selected->occubant;

        source->updateNextPosition(
            position.container->x + padding,
            position.container->y + padding);

        position.occubant = source;
        selected->occubant = nullptr;

        taken.occubant->position->h = 0;
        taken.occubant->position->w = 0;

        if (taken.occubant->color == PlayingColor::RED)
        {
            nRedCheckers--;
        }
        else
        {
            nGreenCheckers--;
        }

        taken.occubant = nullptr;

        selected = nullptr;
    }

    bool tryToOvertake(GridCell &clickedGridCell, int xOffset, int yOffset)
    {
        int y2diff = selected->row + (yOffset * 2);
        int x2diff = selected->column + (xOffset * 2);
        int nextNextIndex = y2diff * n_rows + x2diff;

        if (
            x2diff < 0 ||
            x2diff >= static_cast<int>(n_rows) ||
            y2diff < 0 ||
            y2diff >= static_cast<int>(n_rows))
        {
            return false;
        }

        if (0 <= nextNextIndex && nextNextIndex < static_cast<int>(n_tiles))
        {
            if (auto &nextNextCell = cells[nextNextIndex]; nextNextCell.occubant == nullptr)
            {
                doOvertake(clickedGridCell, nextNextCell);
                return true;
            }
        }

        return false;
    }

    bool tryToMove(int xOffset, int yOffset)
    {
        int ydiff = selected->row + yOffset;
        int xdiff = selected->column + xOffset;
        int nextIndex = ydiff * n_rows + xdiff;

        if (0 <= nextIndex && nextIndex < static_cast<int>(n_tiles))
        {
            auto &gridCell = cells[nextIndex];

            if (
                xdiff < 0 ||
                xdiff >= static_cast<int>(n_rows) ||
                ydiff < 0 ||
                ydiff >= static_cast<int>(n_rows))
            {
                return false;
            }

            if (SDL_PointInRect(&mouseClick, gridCell.container))
            {
                if (gridCell.occubant == nullptr)
                {
                    doMoveToEmpty(gridCell);
                    return true;
                }
                else if (gridCell.occubant->color != selected->occubant->color)
                {
                    return tryToOvertake(gridCell, xOffset, yOffset);
                }
            }
        }

        return false;
    }

    void updateSelected()
    {
        if (tryToMove(1, 1))
            return;
        if (tryToMove(-1, 1))
            return;
        if (tryToMove(1, -1))
            return;
        if (tryToMove(-1, -1))
            return;
        selected = nullptr;
    }

public:

    BoardPlayState(sdl2::Renderer &r, gtool::GameStateProcessor &p, sdl2::TTFFont &f, int swidth, int sheight)
        : renderer(r), font(f), processor(p), screen_width(swidth), screen_height(sheight) {
            pauseState = std::make_shared<PauseState>(renderer, processor, font, screen_width, screen_height);
        }


    void handleEvent(const SDL_Event &event) override
    {
        switch (event.type)
        {
        case SDL_MOUSEBUTTONDOWN:
        {
            auto mouseButtonState = SDL_GetMouseState(&mouseClick.x, &mouseClick.y);
            if ((mouseButtonState & SDL_BUTTON(SDL_BUTTON_LEFT)))
            {
                if (selected == nullptr)
                {
                    findSelected();
                }
                else
                {
                    updateSelected();
                }
            }
        }
        break;
        case SDL_QUIT:
            processor.quitGame();
            break;
        default:
            break;
        }
    }

    void handleKeyState(const uint8_t *state [[maybe_unused]]) override
    {
        if (state[SDL_SCANCODE_ESCAPE])
        {
            processor.startFromNewState(pauseState);
        }
    }

    bool load() override
    {
        redTurn = sdl2::loadBlendedText(renderer,
                                        "Red's turn",
                                        (TTF_Font *)font,
                                        sdl2::asColorStruct(sdl2::Colors::RED));

        greenTurn = sdl2::loadBlendedText(renderer,
                                          "Green's turn",
                                          (TTF_Font *)font,
                                          sdl2::asColorStruct(sdl2::Colors::GREEN));

        size_t currentBlackTileIndex = 0;
        size_t red = 0, green = 0;

        for (int i = 0; i < n_rows; ++i)
        {
            for (int j = 0; j < n_rows; ++j)
            {

                auto flatindex = i * static_cast<int>(n_rows) + j;
                auto &boardContainer = boardContainers[flatindex];

                boardContainer.h = checkerCellDim;
                boardContainer.w = checkerCellDim;

                boardContainer.x = checkerCellDim * j + padding;
                boardContainer.y = checkerCellDim * i + padding;

                cells[flatindex].container = &boardContainer;
                cells[flatindex].column = j;
                cells[flatindex].row = i;

#if _DEBUG
                debugText.emplace_back(sdl2::loadSolidText(renderer,
                                                           "(" + std::to_string(i) + ", " + std::to_string(j) + ")",
                                                           (TTF_Font *)font,
                                                           sdl2::asColorStruct(sdl2::Colors::BLACK)));
#endif
                if (i % 2 != j % 2)
                {
                    auto &bbt = boardBlackTiles[currentBlackTileIndex++];
                    bbt.w = checkerCellDim;
                    bbt.h = checkerCellDim;
                    bbt.x = checkerCellDim * j + padding;
                    bbt.y = checkerCellDim * i + padding;
                }
                else if (i < (n_rows / 2 - 1))
                {
                    green++;
                    initChecker(
                        PlayingColor::GREEN,
                        flatindex,
                        boardContainer.x + padding,
                        boardContainer.y + padding);
                }
                else if (i > (n_rows / 2))
                {
                    red++;
                    initChecker(
                        PlayingColor::RED,
                        flatindex,
                        boardContainer.x + padding,
                        boardContainer.y + padding);
                }
            }
        }

        nGreenCheckers = green;
        nRedCheckers = red;

        return isLoaded(true);
    }

    void update() override
    {
        for (auto &c : checkers)
        {
            c->move();
        }
        if (nRedCheckers <= 0 || nGreenCheckers <= 0)
        {
            isPlaying(false);
        }
    }

    void render() override
    {
        renderer.setColor(sdl2::Colors::WHITE);
        renderer.clear();

        renderer.setColor(sdl2::Colors::BLACK);
        renderer.fillRects(boardBlackTiles);

        renderer.setColor(sdl2::Colors::BLACK);
        renderer.drawRects(boardContainers);

        if (selected)
        {
            renderer.setColor(sdl2::Colors::BLUE);
            renderer.fillRect(selected->container);
        }

        renderer.setColor(sdl2::Colors::GREEN);
        renderer.drawRects(greenChecks);

        renderer.setColor(sdl2::Colors::RED);
        renderer.drawRects(redChecks);

#if _DEBUG
        for (auto i = 0; i < n_rows; ++i)
        {
            for (auto j = 0; j < n_rows; ++j)
            {
                auto &t = debugText[i * n_rows + j];
                t.render(j * 100 + 22, i * 100 + 20);
            }
        }
#endif

        if (playingColor == PlayingColor::GREEN)
        {
            greenTurn->render(screen_width / 2 - 85, screen_height - 32);
        } else {
            redTurn->render(screen_width / 2 - 85, screen_height - 32);
        }

        renderer.updateScreen();
    }
};

#endif