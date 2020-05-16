#pragma once

#include <optional>
#include "SDL.h"
#include "SDL_ttf.h"
#include "SmartPointers.hpp"
#include "Shortcuts.hpp"
#include "gametool/GameState.hpp"
#include "gametool/GameStateProcessor.hpp"
#include "gametool/MessageQueueInterface.hpp"
#include <memory>

class WinState : public asa::GameState
{
private:
    //asa::sdl_unique_ptr<SDL_Texture> red_winner_text;
    asa::TextureBundle green_winner_text;

    asa::TextureBundle red_winner_text;

    std::shared_ptr<SDL_Renderer> renderer;

    asa::GameStateProcessor &processor;
    std::shared_ptr<TTF_Font> font;
    std::shared_ptr<SDL_Window> m_win;

    asa::MessageQueueInterface &m_comms;

    bool has_green_won = false;

public:
    WinState(
        asa::GameStateProcessor &p,
        asa::MessageQueueInterface &mq,
        std::shared_ptr<SDL_Renderer> r,
        std::shared_ptr<TTF_Font> f,
        std::shared_ptr<SDL_Window> win
    );

    bool load(void) override;

    void update(void) override;

    void handleKeyState(const uint8_t *state) override;

    void render(void) override;

    void handleEvent(const SDL_Event &) override;
};
