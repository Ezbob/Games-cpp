
#include "WinState.hpp"
#include "SDL.h"

WinState::WinState(asa::Renderer &r, asa::GameStateProcessor &p, asa::TTFFont &f, const int sw, const int sh)
    : renderer(r), processor(p), font(f), screen_width(sw), screen_height(sh)
{
}

bool WinState::load()
{
    renderer.setColor(asa::Colors::WHITE);
    renderer.clear();
    winnerText = renderer.loadBlendedText(
        "You're a winner",
        font,
        asa::asColorStruct(asa::Colors::RED));
    return isLoaded(true);
}

void WinState::handleKeyState(const uint8_t *state)
{
    if (state[SDL_SCANCODE_ESCAPE])
    {
        processor.quitGame();
    }
}

void WinState::render()
{
    winnerText->render(screen_width / 2 - 100, screen_height / 2 - 12);

    renderer.updateScreen();
}
