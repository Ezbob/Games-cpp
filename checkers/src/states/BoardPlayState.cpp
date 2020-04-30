
#include "BoardPlayState.hpp"
#include "gametool/Easers.hpp"
#include <cmath>
#include <algorithm>

namespace
{

bool is_red(int i)
{
    return ((BOARD_N_CHECKERS / 2) < ((std::size_t)i) && ((std::size_t)i) < BOARD_N_CHECKERS);
}

bool is_green(int i)
{
    return (0 <= ((std::size_t)i) && ((std::size_t)i) < (BOARD_N_CHECKERS / 2));
}

bool is_same_color(int i, BoardPlayState::PlayingColor c)
{
    return (is_green(i) && c == BoardPlayState::PlayingColor::GREEN) || (is_red(i) && c == BoardPlayState::PlayingColor::RED);
}

bool is_in_bounds(int x, int y) {
    return (0 <= x && x < ((int)BOARD_SIDE)) && (0 <= y && y < ((int) BOARD_SIDE));
}

bool is_occupied(int i)
{
    return i >= 0;
}

int empty(void)
{
    return -1;
}

/**
     * Aka the chess distance. This asks how many turns a king piece must need in order to
     * reach some other space on a chess board.
     */
std::size_t chebyshev_distance(int ax, int ay, int bx, int by)
{
    return std::max(std::abs(ax - bx), std::abs(ay - by));
}

constexpr void vec2_add(int &outx, int &outy, const int ax, const int ay, const int bx, const int by) noexcept
{
    outx = ax + bx;
    outy = ay + by;
}


constexpr void vec2_scale(int &outx, int &outy, const int ax, const int ay, const int bscale) noexcept
{
    outx = ax * bscale;
    outy = ay * bscale;
}

}; // namespace

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

void BoardPlayState::start_easing(int occupant_index, int next_x, int next_y) {
    auto &next_checker = next_checker_position[occupant_index];
    next_checker.x = next_x + PADDING;
    next_checker.y = next_y + PADDING;

    easing_progress[occupant_index] = 0.0;
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
            if (source == nullptr)
            {
                for (auto &gridCell : cells)
                {
                    bool intersects_with_mouse = SDL_PointInRect(&mouseClick, gridCell.container);
                    bool is_occupied_cell = is_occupied(gridCell.occupant);
                    bool is_playing_color = is_same_color(gridCell.occupant, playingColor);
                    if (intersects_with_mouse && is_occupied_cell && is_playing_color)
                    {
                        source = &gridCell;
                        return;
                    }
                }
            }
            else
            {
                for (auto &gridCell : cells)
                {
                    bool intersects_with_mouse = SDL_PointInRect(&mouseClick, gridCell.container);
                    bool is_not_occupied_by_same_color = !(is_occupied(gridCell.occupant) && is_same_color(gridCell.occupant, playingColor));
                    bool is_on_diagonal = (gridCell.row != source->row && gridCell.column != source->column);
                    bool is_constrained_by_super_constraint = (chebyshev_distance(gridCell.row, gridCell.column, source->row, source->column) == 1 && !source->is_super);

                    if (intersects_with_mouse && is_not_occupied_by_same_color && is_on_diagonal && is_constrained_by_super_constraint)
                    {
                        target = &gridCell;
                        return;
                    } else if (intersects_with_mouse && gridCell.occupant == source->occupant) {
                        source = nullptr;
                        return;
                    }
                }
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

    easing_progress.fill(-1.0);

    size_t currentBlackTileIndex = 0;
    size_t currentCheckerIndex = 0;

    for (size_t i = 0; i < BOARD_SIDE; ++i)
    {
        for (size_t j = 0; j < BOARD_SIDE; ++j)
        {

            auto flatindex = i * BOARD_SIDE + j;
            auto &boardContainer = boardContainers[flatindex];

            boardContainer.h = CHECKER_CELL_DIM;
            boardContainer.w = CHECKER_CELL_DIM;

            boardContainer.x = CHECKER_CELL_DIM * j + PADDING;
            boardContainer.y = CHECKER_CELL_DIM * i + PADDING;

            auto &current_cell = cells[flatindex];
            current_cell.container = &boardContainer;
            current_cell.column = j;
            current_cell.row = i;

#if _DEBUG
            // + std::to_string(i) + ", " + std::to_string(j)
            debugText.emplace_back(renderer.loadSolidText(
                "(" + std::to_string(i * BOARD_SIDE + j) + ")",
                font,
                asa::asColorStruct(asa::Colors::BLACK)));
#endif

            if (i % 2 != j % 2)
            {
                // back tiles
                auto &bbt = boardBlackTiles[currentBlackTileIndex];
                bbt.w = CHECKER_CELL_DIM;
                bbt.h = CHECKER_CELL_DIM;
                bbt.x = boardContainer.x;
                bbt.y = boardContainer.y;

                currentBlackTileIndex++;
            }
            else if (i < (BOARD_SIDE / 2 - 1) || i > (BOARD_SIDE / 2))
            {
                // all current_checker_dimensions
                auto &checker_rect = current_checker_dimensions[currentCheckerIndex];
                current_cell.occupant = currentCheckerIndex;

                checker_rect.x = boardContainer.x + PADDING;
                checker_rect.y = boardContainer.y + PADDING;
                checker_rect.w = CHECKER_DIM;
                checker_rect.h = CHECKER_DIM;

                auto &checker_next = next_checker_position[currentCheckerIndex];
                checker_next.x = checker_rect.x;
                checker_next.y = checker_rect.y;

                currentCheckerIndex++;
            }
        }
    }

    nGreenCheckers = current_checker_dimensions.size() / 2;
    nRedCheckers = current_checker_dimensions.size() / 2;

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

    if (source)
    {
        renderer.setColor(asa::Colors::BLUE);
        renderer.fillRect(source->container);
    }

    renderer.setColor(asa::Colors::GREEN);
    renderer.fillRects(current_checker_dimensions, 0, 12);

    renderer.setColor(asa::Colors::RED);
    renderer.fillRects(current_checker_dimensions, 12, 24);


#if _DEBUG
    for (std::size_t i = 0; i < BOARD_SIDE; ++i)
    {
        for (std::size_t j = 0; j < BOARD_SIDE; ++j)
        {
            auto &t = debugText[i * BOARD_SIDE + j];
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

void BoardPlayState::update(void)
{

    // tweening/easing of checker movement
    for (std::size_t i = 0; i < BOARD_N_CHECKERS; ++i)
    {
        auto &current_lerp_progress = easing_progress[i];
        current_lerp_progress += sec_per_frame;
        if (current_lerp_progress > 1.0)
        {
            current_lerp_progress = 1.0;
        }
        double t = asa::ease_out_quadric(current_lerp_progress);

        if (0.0 <= t && t <= 1.0)
        {
            asa::lerp(current_checker_dimensions[i], next_checker_position[i], t);
        }
    }

    if (source && target)
    {
        if (!is_occupied(target->occupant))
        {
            // Move to empty place
            target->occupant = source->occupant;
            source->occupant = empty();

            start_easing(target->occupant, target->container->x, target->container->y);

            switchTurn();

            if (target->row == (BOARD_SIDE - 1) || target->row == 0) {
                // first or last row
                target->is_super = true;
            }
        }
        else
        {
            // computing overtake
            int xn = 0, yn = 0;
            vec2_add(xn, yn, -source->column, -source->row, target->column, target->row);
            vec2_scale(xn, yn, xn, yn, 2);
            vec2_add(xn, yn, source->column, source->row, xn, yn);

            if ( is_in_bounds(xn, yn) ) {
                int flat_index = xn + BOARD_SIDE * yn;
                auto &next_postion = cells[flat_index];

                if ( !is_occupied(next_postion.occupant) ) {
                    // I FEEL THE CHAOS OVERTAKING ME; IT IS A GOOD PAIN
                    next_postion.occupant = source->occupant;

                    auto &target_checker = current_checker_dimensions[target->occupant];

                    target_checker.h = 0;
                    target_checker.w = 0;

                    if (is_green(target->occupant)) {
                        nGreenCheckers--;
                    } else {
                        nRedCheckers--;
                    }

                    source->occupant = empty();
                    target->occupant = empty();

                    start_easing(next_postion.occupant, next_postion.container->x, next_postion.container->y);
                }
            }
        }
        source = nullptr;
        target = nullptr;
    }

    if (nRedCheckers <= 0 || nGreenCheckers <= 0)
    {
        isPlaying(false);
    }
}
