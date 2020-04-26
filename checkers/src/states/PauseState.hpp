#pragma once

#include <optional>
#include "SDL.h"
#include "gametool/GameStateProcessor.hpp"
#include "sdl2cpp/Font.hpp"
#include "sdl2cpp/Renderer.hpp"
#include "sdl2cpp/Texture.hpp"

class PauseState : public asa::GameState
{
    std::optional<asa::Texture> pausedText;
    std::optional<asa::Texture> subText;

    asa::Renderer &renderer;
    asa::TTFFont &font;
    asa::GameStateProcessor &processor;
    int screen_width;
    int screen_height;

public:
    PauseState(asa::Renderer &r, asa::GameStateProcessor &p, asa::TTFFont &f, int swidth, int sheight);

    void handleKeyState(const uint8_t *state) override;

    void handleEvent(const SDL_Event &event) override;

    bool load() override;

    void render() override;
};
