
#include "WinState.hpp"
#include "SDL.h"

WinState::WinState(
    asa::Renderer &r,
    asa::GameStateProcessor &p,
    asa::TTFFont &f,
    asa::Window &w,
    asa::MessageQueueInterface &mq)
    : renderer(r)
    , processor(p)
    , font(f)
    , m_window(w)
    , m_comms(mq)
{
}

bool WinState::load(void)
{
    renderer.setColor(asa::Colors::WHITE);
    renderer.clear();
    red_winner_text = renderer.loadBlendedText(
        "Red won!",
        font,
        asa::asColorStruct(asa::Colors::RED));
    red_winner_text = renderer.loadBlendedText(
        "Green won!",
        font,
        asa::asColorStruct(asa::Colors::GREEN));

    window_height = m_window.height();
    window_width = m_window.width();

    return isLoaded(true);
}

void WinState::handleKeyState(const uint8_t *state)
{
    if (state[SDL_SCANCODE_ESCAPE])
    {
        processor.quitGame();
    }
}

void WinState::render(void)
{
    if (has_green_won) {
        green_winner_text->render(window_width / 2 - 100, window_height / 2 - 12);
    } else {
        red_winner_text->render(window_width / 2 - 100, window_height / 2 - 12);
    }

    renderer.updateScreen();
}
