#pragma once

#include <optional>
#include <memory>
#include <vector>
#include <array>
#include "SDL.h"
#include "SDL_ttf.h"
#include "sdl2cpp/Renderer.hpp"
#include "sdl2cpp/Texture.hpp"
#include "sdl2cpp/Font.hpp"
#include "gametool/GameState.hpp"
#include "gametool/GameStateProcessor.hpp"

#include <iostream>

constexpr std::size_t BOARD_SIDE = 8;
constexpr std::size_t BOARD_SIZE = BOARD_SIDE * BOARD_SIDE;
constexpr std::size_t BOARD_N_CHECKERS = (BOARD_SIZE / 2) - BOARD_SIDE;

constexpr std::size_t CHECKER_CELL_DIM = 100; // h/w of the cell that can contain a checker
constexpr std::size_t CHECKER_DIM = 60;       // h/w of the rect that is inside a cell
constexpr std::size_t PADDING = 20;

class BoardPlayState : public asa::GameState
{
public:
    enum class PlayingColor
    {
        GREEN,
        RED
    };

    BoardPlayState(asa::Renderer &r, asa::GameStateProcessor &p, asa::TTFFont &f, const asa::GameClock &clock, int swidth, int sheight);

    void handleEvent(const SDL_Event &event) override;
    void handleKeyState(const uint8_t *state) override;
    bool load(void) override;
    void render(void) override;
    void update(void) override;

private:
    asa::Renderer &renderer;
    asa::TTFFont &font;
    asa::GameStateProcessor &processor;
    asa::GameClock const &clock;

    std::shared_ptr<asa::GameState> pauseState;

    int screen_width, screen_height;

    const double sec_per_frame;

    std::array<double, BOARD_N_CHECKERS> easing_progress;
    std::array<SDL_Point, BOARD_N_CHECKERS> next_checker_position;
    std::array<SDL_Rect, BOARD_N_CHECKERS> current_checker_dimensions;

    struct GridCell
    {
        SDL_Rect *container;
        bool is_super = false;
        int occupant = -1;
        int column;
        int row;
    };

    // these can be used to draw in batch
    std::array<SDL_Rect, BOARD_SIZE> boardContainers;
    std::array<SDL_Rect, BOARD_SIZE / 2> boardBlackTiles;
    std::array<GridCell, BOARD_SIZE> cells;

    PlayingColor playingColor = PlayingColor::GREEN;

    SDL_Point mouseClick;
    int nRedCheckers = 0, nGreenCheckers = 0;


#if _DEBUG
    std::vector<asa::Texture> debugText;
#endif


    GridCell *source = nullptr;
    GridCell *target = nullptr;

    std::optional<asa::Texture> greenTurn;
    std::optional<asa::Texture> redTurn;

    void switchTurn(void);

    void start_easing(int occupant_index, int next_x, int next_y);
};
