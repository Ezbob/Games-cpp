#pragma once

#include "SDL.h"
#include "SDL_ttf.h"
#include "SmartPointers.hpp"
#include "Shortcuts.hpp"
#include "gametool/GameStateProcessor.hpp"
#include <memory>

class PauseState : public asa::GameState
{
    asa::TextureBundle pausedText;
    asa::TextureBundle subText;

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
