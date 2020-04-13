
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
    WinState(sdl2::Renderer &r, gtool::GameStateProcessor &p, sdl2::TTFFont& f, const int sw, const int sh)
        : renderer(r), processor(p), font(f), screen_width(sw), screen_height(sh) {
    }

    bool load() override
    {
        renderer.setColor(sdl2::Colors::WHITE);
        renderer.clear();
        winnerText = sdl2::loadBlendedText(renderer,
                                           "You're a winner",
                                           (TTF_Font *)font,
                                           sdl2::asColorStruct(sdl2::Colors::RED));
        return isLoaded(true);
    }

    void handleKeyState(const uint8_t *state) override
    {
        if (state[SDL_SCANCODE_ESCAPE])
        {
            processor.quitGame();
        }
    }

    void render() override
    {
        winnerText->render(screen_width / 2 - 100, screen_height / 2 - 12);

        renderer.updateScreen();
    }
};

#endif