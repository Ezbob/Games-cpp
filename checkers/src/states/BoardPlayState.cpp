
#include "BoardPlayState.hpp"
#include "SDL.h"
#include "gametool/Easers.hpp"
#include <cmath>

BoardPlayState::Checker::Checker(PlayingColor playerColor, SDL_Rect &p)
    : color(playerColor), position(p), next(p)
{
}

void BoardPlayState::Checker::setNextPosition(int x, int y)
{
    next.x = x;
    next.y = y;
    currentDegree = 0;
}

void BoardPlayState::Checker::update(double sekPerFrame)
{
    currentDegree += sekPerFrame;
    if (currentDegree > 1.0)
    {
        currentDegree = 1.0;
    }
    double t = asa::ease_out_quadric(currentDegree);
    if (t <= 1.0)
    {
        asa::lerp(position, next, t);
    }
}

void BoardPlayState::switchTurn()
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

void BoardPlayState::findSelected()
{
    for (int i = 0; i < n_rows; i++)
    {
        for (int j = 0; j < n_rows; ++j)
        {

            int index = i * n_rows + j;
            auto &gridCell = cells[index];

            if (SDL_PointInRect(&mouseClick, gridCell.container) && gridCell.occubant != nullptr && gridCell.occubant->color == playingColor && !SDL_RectEmpty(&gridCell.occubant->position))
            {
                selected = &gridCell;
                return;
            }
        }
    }
}

void BoardPlayState::initChecker(PlayingColor checkerColor, int flatindex, int checkerX, int checkerY)
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

void BoardPlayState::doMoveToEmpty(GridCell &target)
{
    Checker *source = selected->occubant;

    source->setNextPosition(
        target.container->x + padding,
        target.container->y + padding);

    switchTurn();

    target.occubant = source;
    selected->occubant = nullptr;
    selected = nullptr;
}

void BoardPlayState::doOvertake(GridCell &taken, GridCell &position)
{
    Checker *source = selected->occubant;

    source->setNextPosition(
        position.container->x + padding,
        position.container->y + padding);

    position.occubant = source;
    selected->occubant = nullptr;

    taken.occubant->position.h = 0;
    taken.occubant->position.w = 0;

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

bool BoardPlayState::tryToOvertake(GridCell &clickedGridCell, int xOffset, int yOffset)
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

bool BoardPlayState::tryToMove(int xOffset, int yOffset)
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

void BoardPlayState::updateSelected()
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

/* === PUBLIC INTERFACE === */

BoardPlayState::BoardPlayState(asa::Renderer &r, asa::GameStateProcessor &p, asa::TTFFont &f, const asa::GameClock &clock, int swidth, int sheight)
    : renderer(r), font(f), processor(p), clock(clock), screen_width(swidth), screen_height(sheight), sec_per_frame(clock.msPerUpdate() / 1000.)
{
    pauseState = std::make_shared<PauseState>(renderer, processor, font, screen_width, screen_height);
}

void BoardPlayState::handleEvent(const SDL_Event &event)
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

void BoardPlayState::handleKeyState(const uint8_t *state [[maybe_unused]])
{
    if (state[SDL_SCANCODE_ESCAPE])
    {
        processor.startFromNewState(pauseState);
    }
}

bool BoardPlayState::load()
{
    redTurn = renderer.loadBlendedText(
        "Red's turn",
        font,
        asa::asColorStruct(asa::Colors::RED));

    greenTurn = renderer.loadBlendedText("Green's turn",
                                         font,
                                         asa::asColorStruct(asa::Colors::GREEN));

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
            debugText.emplace_back(renderer.loadSolidText(
                "(" + std::to_string(i) + ", " + std::to_string(j) + ")",
                font,
                asa::asColorStruct(asa::Colors::BLACK)));
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

void BoardPlayState::render()
{
    renderer.setColor(asa::Colors::WHITE);
    renderer.clear();

    renderer.setColor(asa::Colors::BLACK);
    renderer.fillRects(boardBlackTiles);

    renderer.setColor(asa::Colors::BLACK);
    renderer.drawRects(boardContainers);

    if (selected)
    {
        renderer.setColor(asa::Colors::BLUE);
        renderer.fillRect(selected->container);
    }

    renderer.setColor(asa::Colors::GREEN);
    renderer.drawRects(greenChecks);

    renderer.setColor(asa::Colors::RED);
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
    }
    else
    {
        redTurn->render(screen_width / 2 - 85, screen_height - 32);
    }

    renderer.updateScreen();
}

void BoardPlayState::update()
{
    for (auto &c : checkers)
    {
        c->update(sec_per_frame);
    }
    if (nRedCheckers <= 0 || nGreenCheckers <= 0)
    {
        isPlaying(false);
    }
}
