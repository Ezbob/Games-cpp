#pragma once

#include "SDL.h"
#include "SDL_ttf.h"
#include "Deleter.hpp"
#include "gametool/GameStateProcessor.hpp"
#include <memory>

class PauseState : public asa::GameState
{
    std::unique_ptr<SDL_Texture, asa::TextureDeleter> pausedText;
    std::unique_ptr<SDL_Texture, asa::TextureDeleter> subText;

    SDL_Rect pausedPos;
    SDL_Rect subPos;

    std::shared_ptr<SDL_Renderer> renderer;
    std::shared_ptr<TTF_Font> font;
    asa::GameStateProcessor &processor;
    std::shared_ptr<SDL_Window> m_win;

public:
    PauseState(
        std::shared_ptr<SDL_Renderer> r,
        asa::GameStateProcessor &p,
        std::shared_ptr<TTF_Font> f,
        std::shared_ptr<SDL_Window> win);

    void handleKeyState(const uint8_t *state) override;

    void handleEvent(const SDL_Event &event) override;

    bool load() override;

    void render() override;
};
