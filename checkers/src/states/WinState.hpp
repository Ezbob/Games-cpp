#pragma once

#include <optional>
#include "SDL2cpp/Renderer.hpp"
#include "SDL2cpp/Texture.hpp"
#include "SDL2cpp/Font.hpp"
#include "SDL_ttf.h"
#include "GameTool/GameState.hpp"
#include "GameTool/GameStateProcessor.hpp"

class WinState : public asa::GameState
{
    std::optional<asa::Texture> winnerText;
    asa::Renderer &renderer;
    asa::GameStateProcessor &processor;
    asa::TTFFont &font;
    int screen_width;
    int screen_height;

public:
    WinState(asa::Renderer &r, asa::GameStateProcessor &p, asa::TTFFont &f, const int sw, const int sh);

    bool load() override;

    void handleKeyState(const uint8_t *state) override;

    void render() override;
};
