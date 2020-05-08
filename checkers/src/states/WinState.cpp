
#include "WinState.hpp"
#include "SDL.h"
#include <iostream>

WinState::WinState(
    asa::Renderer &r,
    asa::GameStateProcessor &p,
    asa::TTFFont &f,
    asa::Window &w,
    asa::MessageQueueInterface &mq)
    : renderer(r), processor(p), font(f), m_window(w), m_comms(mq)
{
}

bool WinState::load(void)
{
    red_winner_text = renderer.loadBlendedText(
        "Red won!",
        font,
        asa::asColorStruct(asa::Colors::RED));
    green_winner_text = renderer.loadBlendedText(
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

void WinState::update(void)
{
    if (m_comms.size() > 0)
    {
        has_green_won = std::any_cast<bool>(*m_comms.poll());
    }
}

void WinState::render(void)
{
    renderer.setColor(asa::Colors::WHITE);
    renderer.clear();

    //std::cout << std::boolalpha << "Is loaded? " << (((bool) green_winner_text) && green_winner_text->isLoaded() ) << "\n"; 

    auto r = SDL_Rect {0, 0, 100, 100};
    renderer.copyTexture(*green_winner_text, nullptr, &r);
/*
    if (has_green_won)
    {
        green_winner_text->render(window_width / 2 - 100, window_height / 2 - 12);
    }
    else
    {
        red_winner_text->render(window_width / 2 - 100, window_height / 2 - 12);
    }
*/
    renderer.updateScreen();
}
