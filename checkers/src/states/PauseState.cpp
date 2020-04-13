
#include "PauseState.hpp"

PauseState::PauseState(sdl2::Renderer &r, gtool::GameStateProcessor &p, sdl2::TTFFont &f, int swidth, int sheight)
    : renderer(r), font(f), processor(p), screen_width(swidth), screen_height(sheight) {}

void PauseState::handleKeyState(const uint8_t *state)
{
    if (state[SDL_SCANCODE_RETURN])
    {
        isPlaying(false);
    }
}

void PauseState::handleEvent(const SDL_Event &event)
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

bool PauseState::load()
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

void PauseState::render()
{
    renderer.setColor(sdl2::Colors::WHITE);
    renderer.clear();

    pausedText->render(screen_width / 2 - 50, screen_height / 2 - 12);
    subText->render(screen_width / 2 - 140, screen_height / 2 + 14);

    renderer.updateScreen();
}