
#ifndef _HEADER_FILE_WinState_20200413195715_
#define _HEADER_FILE_WinState_20200413195715_

#include <optional>
#include "SDL.hpp"
#include "SDL_ttf.h"
#include "GameTool/GameState.hpp"
#include "GameTool/GameStateProcessor.hpp"

class WinState : public gtool::GameState
{
    std::optional<sdl2::Texture> winnerText;
    sdl2::Renderer &renderer;
    gtool::GameStateProcessor &processor;
    sdl2::TTFFont &font;
    int screen_width;
    int screen_height;

public:
    WinState(sdl2::Renderer &r, gtool::GameStateProcessor &p, sdl2::TTFFont &f, const int sw, const int sh);

    bool load() override;

    void handleKeyState(const uint8_t *state) override;

    void render() override;
};

#endif