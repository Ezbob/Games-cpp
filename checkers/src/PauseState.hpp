
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
    PauseState(sdl2::Renderer &r, gtool::GameStateProcessor &p, sdl2::TTFFont &f, int swidth, int sheight)
        : renderer(r), font(f), processor(p), screen_width(swidth), screen_height(sheight) {}

    void handleKeyState(const uint8_t *state) override
    {
        if (state[SDL_SCANCODE_RETURN])
        {
            isPlaying(false);
        }
    }

    void handleEvent(const SDL_Event &event) override
    {
        switch (event.type)
        {
        case SDL_QUIT:
            processor.quitGame();
            break;
        default:
            break;
        }
    }

    bool load() override
    {
        pausedText = sdl2::loadBlendedText(renderer,
                                           "Game Paused",
                                           (TTF_Font *)font,
                                           asColorStruct(sdl2::Colors::BLACK));

        subText = sdl2::loadBlendedText(renderer,
                                        "(Press Enter to continue)",
                                        (TTF_Font *)font,
                                        asColorStruct(sdl2::Colors::BLACK));

        return isLoaded(pausedText && subText);
    }

    void render() override
    {
        renderer.setColor(sdl2::Colors::WHITE);
        renderer.clear();

        pausedText->render(screen_width / 2 - 50, screen_height / 2 - 12);
        subText->render(screen_width / 2 - 140, screen_height / 2 + 14);

        renderer.updateScreen();
    }
};

#endif