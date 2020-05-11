
#include "WinState.hpp"
#include "SDL.h"
#include "ErrorCheck.hpp"

WinState::WinState(
        std::shared_ptr<SDL_Renderer> renderer,
        std::shared_ptr<TTF_Font> font,
        asa::GameStateProcessor & p,
        asa::MessageQueueInterface & mq
    ) : renderer(r), processor(p), font(f), m_comms(mq)
{
}

bool WinState::load(void)
{

    SDL_CreateTexture()
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

    if (has_green_won)
    {
        
        green_winner_text->render(window_width / 2 - 100, window_height / 2 - 12);
    }
    else
    {
        red_winner_text->render(window_width / 2 - 100, window_height / 2 - 12);
    }

    renderer.updateScreen();
}
