#pragma once

#include <optional>
#include "sdl2cpp/Renderer.hpp"
#include "sdl2cpp/Texture.hpp"
#include "sdl2cpp/Font.hpp"
#include "SDL_ttf.h"
#include "gametool/GameState.hpp"
#include "gametool/GameStateProcessor.hpp"
#include "gametool/MessageQueueInterface.hpp"

class WinState : public asa::GameState
{
private:
    std::optional<asa::Texture> red_winner_text;
    std::optional<asa::Texture> green_winner_text;
    asa::Renderer &renderer;
    asa::GameStateProcessor &processor;
    asa::TTFFont &font;
    asa::Window &m_window;
    asa::MessageQueueInterface &m_comms;

    int window_width = 0, window_height = 0;
    bool has_green_won = false;

public:
    WinState(asa::Renderer &,
            asa::GameStateProcessor &,
            asa::TTFFont &,
            asa::Window &,
            asa::MessageQueueInterface &);

    bool load(void) override;

    void update(void) override;

    void handleKeyState(const uint8_t *state) override;

    void render(void) override;
};
