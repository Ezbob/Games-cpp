#pragma once

#include <optional>
#include "SDL.h"
#include "SDL_ttf.h"
#include "Deleter.hpp"
#include "gametool/GameState.hpp"
#include "gametool/GameStateProcessor.hpp"
#include "gametool/MessageQueueInterface.hpp"
#include <memory>

class WinState : public asa::GameState
{
private:
    std::unique_ptr<SDL_Texture, asa::TextureDeleter> red_winner_text;
    std::unique_ptr<SDL_Texture, asa::TextureDeleter> green_winner_text;
    
    std::shared_ptr<SDL_Renderer> renderer;

    SDL_Rect red_pos;
    SDL_Rect green_pos;

    asa::GameStateProcessor &processor;
    std::shared_ptr<TTF_Font> font;
    std::shared_ptr<SDL_Window> m_win;

    asa::MessageQueueInterface &m_comms;

    bool has_green_won = false;

public:
    WinState(
        std::shared_ptr<SDL_Renderer> r,
        asa::GameStateProcessor &p,
        std::shared_ptr<TTF_Font> f,
        std::shared_ptr<SDL_Window> win,
        asa::MessageQueueInterface &mq);

    bool load(void) override;

    void update(void) override;

    void handleKeyState(const uint8_t *state) override;

    void render(void) override;
};
