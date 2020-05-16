
#include "BoardPlayState.hpp"
#include "gametool/Easers.hpp"
#include "Shortcuts.hpp"

#include <cmath>
#include <algorithm>

namespace
{

    bool is_green(int i)
    {
        return 0 <= i && i < ((int)BOARD_N_CHECKERS) / 2;
    }

    bool is_red(int i)
    {
        return i >= 0 && !is_green(i);
    }

    bool is_same_color(int i, BoardPlayState::PlayingColor c)
    {
        return (is_green(i) && c == BoardPlayState::PlayingColor::GREEN) || (is_red(i) && c == BoardPlayState::PlayingColor::RED);
    }

    bool is_in_bounds(int x, int y)
    {
        return (0 <= x && x < ((int)BOARD_SIDE)) && (0 <= y && y < ((int)BOARD_SIDE));
    }

    bool is_occupied(int i)
    {
        return i >= 0;
    }

    int empty(void)
    {
        return -1;
    }

    int get_flat_index(int y, int x) noexcept
    {
        return x + ((int)BOARD_SIDE) * y;
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

void BoardPlayState::start_easing(int occupant_index, int next_x, int next_y)
{
    auto &next_checker = next_checker_position[occupant_index];
    next_checker.x = next_x + PADDING;
    next_checker.y = next_y + PADDING;

    easing_progress[occupant_index] = 0.0;
}

bool BoardPlayState::should_become_super_checker(const GridCell &cell) const
{
    bool is_last_line = cell.position.y() == (BOARD_SIDE - 1);
    bool is_first_line = cell.position.y() == 0;

    bool is_already_super = super_checker_table[cell.occupant];

    bool result = (is_last_line && is_green(cell.occupant)) || (is_first_line && is_red(cell.occupant)) || is_already_super;
    return result;
}

bool BoardPlayState::no_checker_in_the_way(void) const
{
    auto direction_v = (target->position - source->position);
    auto distance = target->position.chebyshev_distance(source->position);

    direction_v /= distance;
    // integer space so we use chebyshev distance instead of euclidean to avoid double
    // casting and truncation of decimals

    auto destination_v = target->position;
    auto current_v = source->position + direction_v;

    while (is_in_bounds(current_v.x(), current_v.y()) && current_v != destination_v)
    {
        GridCell const &cell = cells[get_flat_index(current_v.y(), current_v.x())];

        if (is_occupied(cell.occupant))
        {
            return false;
        }

        current_v += direction_v;
    }

    return true;
}

/* === PUBLIC INTERFACE === */

BoardPlayState::BoardPlayState(
    asa::GameStateProcessor &p,
    asa::MessageQueueInterface &comms,
    asa::GameClock &clock,
    asa::sdl_shared_ptr<SDL_Renderer> r,
    asa::sdl_shared_ptr<TTF_Font> f,
    std::shared_ptr<SDL_Window> win)
    : renderer(r),
      processor(p),
      font(f),
      m_win(win),
      sec_per_frame(clock.msPerUpdate() / 1000.0),
      m_comms(comms)
{
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
                for (GridCell &gridCell : cells)
                {
                    if (SDL_PointInRect(&mouseClick, gridCell.container))
                    {
                        bool is_occupied_cell = is_occupied(gridCell.occupant);
                        bool is_playing_color = is_same_color(gridCell.occupant, playingColor);

                        if (is_occupied_cell && is_playing_color)
                        {
                            source = &gridCell;
                            return;
                        }
                    }
                }
            }
            else
            {
                int max_distance = super_checker_table[source->occupant] ? ((int)BOARD_SIDE) : 1;

                for (GridCell &gridCell : cells)
                {

                    bool intersects_with_mouse = SDL_PointInRect(&mouseClick, gridCell.container);
                    bool is_not_occupied_by_same_color = !(is_occupied(gridCell.occupant) && is_same_color(gridCell.occupant, playingColor));

                    if (
                        intersects_with_mouse &&
                        is_not_occupied_by_same_color)
                    {
                        auto v = (gridCell.position - source->position).normalized();
                        auto distance = source->position.chebyshev_distance(gridCell.position);

                        if (std::abs(v.x()) == std::abs(v.y()) && distance <= max_distance)
                        {
                            target = &gridCell;
                            return;
                        }
                    }
                    else if (intersects_with_mouse && gridCell.occupant == source->occupant)
                    {
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
        processor.startFromState(0);
    }
}

bool BoardPlayState::load(void)
{
    auto asset_path = asa::getBasePath() + "/assets";

    int screen_width, screen_height;
    SDL_GetWindowSize(m_win.get(), &screen_width, &screen_height);

    red_turn_text = asa::createTextureBundle(
        renderer,
        TTF_RenderText_Blended(font.get(), "Red's turn", SDL_Color{0xff, 0x0, 0x0, 0xff}));

    green_turn_text = asa::createTextureBundle(
        renderer,
        TTF_RenderText_Blended(font.get(), "Green's turn", SDL_Color{0x0, 0xff, 0x0, 0xff}));

    green_turn_text.position.x = screen_width / 2 - 85;
    green_turn_text.position.y = screen_height - 32;
    red_turn_text.position.x = green_turn_text.position.x;
    red_turn_text.position.y = green_turn_text.position.y;

#define INCLUDE_PNG_FILE__(p) std::move(asa::intoTexture(renderer, IMG_Load(p)))
    white_tile = INCLUDE_PNG_FILE__((asset_path + "/white_tile.png").c_str());
    black_tile = INCLUDE_PNG_FILE__((asset_path + "/black_tile.png").c_str());

    green_checker_texture = INCLUDE_PNG_FILE__((asset_path + "/checker_green3.png").c_str());
    red_checker_texture = INCLUDE_PNG_FILE__((asset_path + "/checker_red3.png").c_str());
    checker_shadow_texture = INCLUDE_PNG_FILE__((asset_path + "/checker_shadow.png").c_str());
    green_super_texture = INCLUDE_PNG_FILE__((asset_path + "/checker_green_king.png").c_str());
    red_super_texture = INCLUDE_PNG_FILE__((asset_path + "/checker_red_king.png").c_str());
#undef INCLUDE_PNG_FILE__

    bool is_loaded = green_checker_texture &&
                     red_checker_texture &&
                     checker_shadow_texture &&
                     white_tile &&
                     black_tile &&
                     green_super_texture &&
                     red_super_texture &&
                     red_turn_text.texture &&
                     green_turn_text.texture;

    easing_progress.fill(-1.0);
    super_checker_table.fill(false);

    size_t currentBlackTileIndex = 0;
    size_t currentCheckerIndex = 0;

    for (size_t y = 0; y < BOARD_SIDE; ++y)
    {
        for (size_t x = 0; x < BOARD_SIDE; ++x)
        {

            auto flatindex = y * BOARD_SIDE + x;
            auto &boardContainer = boardContainers[flatindex];

            boardContainer.h = CHECKER_CELL_DIM;
            boardContainer.w = CHECKER_CELL_DIM;

            boardContainer.x = CHECKER_CELL_DIM * x + PADDING;
            boardContainer.y = CHECKER_CELL_DIM * y + PADDING;

            auto &current_cell = cells[flatindex];
            current_cell.container = &boardContainer;

            current_cell.position.x(x);
            current_cell.position.y(y);

            /*
#if _DEBUG
            // + std::to_string(i) + ", " + std::to_string(j)
            debugText.emplace_back(renderer.loadSolidText(
                "(" + std::to_string(current_cell.position.y()) + ", " + std::to_string(current_cell.position.x()) + ")",
                font,
                asa::asColorStruct(asa::Colors::BLACK)));
#endif
*/

            if (x % 2 != y % 2)
            {
                // back tiles
                auto &bbt = boardBlackTiles[currentBlackTileIndex];
                bbt.w = CHECKER_CELL_DIM;
                bbt.h = CHECKER_CELL_DIM;
                bbt.x = boardContainer.x;
                bbt.y = boardContainer.y;

                currentBlackTileIndex++;
            }
            else if (y < (BOARD_SIDE / 2 - 1) || y > (BOARD_SIDE / 2)) // split at the median
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

    green_checkers = current_checker_dimensions.size() / 2;
    red_checkers = current_checker_dimensions.size() / 2;

    return isLoaded(is_loaded);
}

void BoardPlayState::render(void)
{
    SDL_SetRenderDrawColor(renderer.get(), 0xff, 0xff, 0xff, 0xff);
    SDL_RenderClear(renderer.get());

    for (auto &r : boardContainers)
    {
        SDL_RenderCopy(renderer.get(), white_tile.get(), nullptr, &r);
    }

    for (auto &r : boardBlackTiles)
    {
        SDL_RenderCopy(renderer.get(), black_tile.get(), nullptr, &r);
    }

    SDL_SetRenderDrawBlendMode(renderer.get(), SDL_BLENDMODE_ADD);

    if (source)
    {
        SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0xff, 0xff);
        SDL_RenderFillRect(renderer.get(), source->container);
    }

    SDL_SetRenderDrawBlendMode(renderer.get(), SDL_BLENDMODE_NONE);

    for (std::size_t i = 0; i < BOARD_N_CHECKERS / 2; ++i)
    {
        if (super_checker_table[i])
        {
            SDL_RenderCopy(renderer.get(), green_super_texture.get(), nullptr, &current_checker_dimensions[i]);
        }
        else
        {
            SDL_RenderCopy(renderer.get(), green_checker_texture.get(), nullptr, &current_checker_dimensions[i]);
        }
    }

    for (std::size_t i = (BOARD_N_CHECKERS / 2); i < BOARD_N_CHECKERS; ++i)
    {
        if (super_checker_table[i])
        {
            SDL_RenderCopy(renderer.get(), red_super_texture.get(), nullptr, &current_checker_dimensions[i]);
        }
        else
        {
            SDL_RenderCopy(renderer.get(), red_checker_texture.get(), nullptr, &current_checker_dimensions[i]);
        }
    }

    /*
#if _DEBUG
    for (std::size_t y = 0; y < BOARD_SIDE; ++y)
    {
        for (std::size_t x = 0; x < BOARD_SIDE; ++x)
        {
            auto &t = debugText[y * BOARD_SIDE + x];
            t.render(y * 100 + 22, x * 100 + 20);
        }
    }
#endif
*/

    if (playingColor == PlayingColor::GREEN)
    {
        SDL_RenderCopy(renderer.get(), green_turn_text.texture.get(), nullptr, &green_turn_text.position);
    }
    else
    {
        SDL_RenderCopy(renderer.get(), red_turn_text.texture.get(), nullptr, &red_turn_text.position);
    }

    SDL_RenderPresent(renderer.get());
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
        if (no_checker_in_the_way())
        {
            if (!is_occupied(target->occupant))
            {
                // Move to empty place
                target->occupant = source->occupant;
                source->occupant = empty();

                start_easing(target->occupant, target->container->x, target->container->y);

                switchTurn();

                if (should_become_super_checker(*target))
                {
                    // first or last y
                    super_checker_table[target->occupant] = true;
                }
            }
            else
            {
                // computing overtake

                // first check whether some other checker is on our path
                auto direction_v = (target->position - source->position);
                auto distance = target->position.chebyshev_distance(source->position);

                direction_v /= distance;
                // integer space so we use chebyshev distance instead of euclidean to avoid double
                // casting and truncation of decimals

                auto destination_v = target->position + direction_v;

                // we're turned around ?? :D our basis is [[0, 1], [1, 0]] not [[1,0], [0, 1]]

                if (is_in_bounds(destination_v.y(), destination_v.x()))
                {
                    int flat_index = get_flat_index(destination_v.y(), destination_v.x());
                    auto &next_postion = cells[flat_index];

                    if (!is_occupied(next_postion.occupant))
                    {
                        // I FEEL THE WARP OVERTAKING ME; IT IS A GOOD PAIN
                        next_postion.occupant = source->occupant;

                        auto &target_checker = current_checker_dimensions[target->occupant];

                        target_checker.h = 0;
                        target_checker.w = 0;

                        if (is_green(target->occupant))
                        {
                            green_checkers--;
                        }
                        else
                        {
                            red_checkers--;
                        }

                        source->occupant = empty();
                        target->occupant = empty();

                        start_easing(next_postion.occupant, next_postion.container->x, next_postion.container->y);

                        if (should_become_super_checker(next_postion))
                        {
                            // first or last y
                            super_checker_table[next_postion.occupant] = true;
                        }
                    }
                }
            }
        }
        source = nullptr;
        target = nullptr;
    }

    if (red_checkers <= 0 || green_checkers <= 0)
    {
        m_comms.offer(std::make_shared<std::any>(green_checkers > 0));
        isPlaying(false);
    }
}
