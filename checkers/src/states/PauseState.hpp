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

    asa::sdl_shared_ptr<SDL_Renderer> renderer;
    asa::sdl_shared_ptr<TTF_Font> font;
    asa::GameStateProcessor &processor;
    asa::sdl_shared_ptr<SDL_Window> m_win;

public:
    PauseState(
        asa::GameStateProcessor &p,
        asa::sdl_shared_ptr<SDL_Renderer> r,
        asa::sdl_shared_ptr<TTF_Font> f,
        asa::sdl_shared_ptr<SDL_Window> win);

    void handleKeyState(const uint8_t *state) override;

    void handleEvent(const SDL_Event &event) override;

    bool load(void) override;

    void render(void) override;
};
