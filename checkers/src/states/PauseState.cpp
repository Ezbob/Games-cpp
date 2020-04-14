
#include "PauseState.hpp"
#include "SDL2cpp/AssetLoading.hpp"
#include "SDL.h"

PauseState::PauseState(asa::Renderer &r, asa::GameStateProcessor &p, asa::TTFFont &f, int swidth, int sheight)
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
    pausedText = asa::loadBlendedText(renderer,
                                      "Game Paused",
                                      (TTF_Font *)font,
                                      asColorStruct(asa::Colors::BLACK));

    subText = asa::loadBlendedText(renderer,
                                   "(Press Enter to continue)",
                                   (TTF_Font *)font,
                                   asColorStruct(asa::Colors::BLACK));

    return isLoaded(pausedText && subText);
}

void PauseState::render()
{
    renderer.setColor(asa::Colors::WHITE);
    renderer.clear();

    pausedText->render(screen_width / 2 - 50, screen_height / 2 - 12);
    subText->render(screen_width / 2 - 140, screen_height / 2 + 14);

    renderer.updateScreen();
}