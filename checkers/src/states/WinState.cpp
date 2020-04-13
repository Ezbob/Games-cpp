
#include "WinState.hpp"

WinState::WinState(sdl2::Renderer &r, gtool::GameStateProcessor &p, sdl2::TTFFont &f, const int sw, const int sh)
    : renderer(r), processor(p), font(f), screen_width(sw), screen_height(sh)
{
}

bool WinState::load()
{
    renderer.setColor(sdl2::Colors::WHITE);
    renderer.clear();
    winnerText = sdl2::loadBlendedText(renderer,
                                       "You're a winner",
                                       (TTF_Font *)font,
                                       sdl2::asColorStruct(sdl2::Colors::RED));
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
