#pragma once

#include <optional>
#include "SDL.h"
#include "SDL_ttf.h"
#include "gametool/GameState.hpp"
#include "gametool/GameStateProcessor.hpp"
#include "gametool/MessageQueueInterface.hpp"
#include <memory>

class WinState : public asa::GameState
{
private:
    std::unique_ptr<SDL_Texture> red_winner_text;
    std::unique_ptr<SDL_Texture> green_winner_text;
    std::shared_ptr<TTF_Font> font;
    std::shared_ptr<SDL_Renderer> renderer;

    asa::GameStateProcessor &processor;
    asa::MessageQueueInterface &m_comms;

    int window_width = 0, window_height = 0;
    bool has_green_won = false;

public:
    WinState(
        std::shared_ptr<SDL_Renderer>,
        std::shared_ptr<TTF_Font>,
        asa::GameStateProcessor &,
        asa::MessageQueueInterface &
    );

    bool load(void) override;

    void update(void) override;

    void handleKeyState(const uint8_t *state) override;

    void render(void) override;
};
