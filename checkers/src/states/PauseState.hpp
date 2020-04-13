
#ifndef _HEADER_FILE_PauseState_20200413193438_
#define _HEADER_FILE_PauseState_20200413193438_

#include <optional>
#include "SDL.h"
#include "SDL.hpp"
#include "SDL_extra.hpp"
#include "GameTool/GameStateProcessor.hpp"

class PauseState : public gtool::GameState
{
    std::optional<sdl2::Texture> pausedText;
    std::optional<sdl2::Texture> subText;

    sdl2::Renderer &renderer;
    sdl2::TTFFont &font;
    gtool::GameStateProcessor &processor;
    int screen_width;
    int screen_height;

public:
    PauseState(sdl2::Renderer &r, gtool::GameStateProcessor &p, sdl2::TTFFont &f, int swidth, int sheight);

    void handleKeyState(const uint8_t *state) override;

    void handleEvent(const SDL_Event &event) override;

    bool load() override;

    void render() override;
};

#endif