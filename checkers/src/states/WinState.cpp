
#include "WinState.hpp"
#include "SDL.h"
#include "ErrorCheck.hpp"
#include "Shortcuts.hpp"
#include <memory>

WinState::WinState(
    asa::GameStateProcessor &p,
    asa::MessageQueueInterface &mq,
    std::shared_ptr<SDL_Renderer> r,
    std::shared_ptr<TTF_Font> f,
    std::shared_ptr<SDL_Window> win)
    : renderer(r)
    , processor(p)
    , font(f)
    , m_win(win)
    , m_comms(mq)
{
}

bool WinState::load(void)
{
    int window_width = 0, window_height = 0;
    SDL_GetWindowSize(m_win.get(), &window_width, &window_height);

    red_winner_text = asa::createTextureBundle(renderer, TTF_RenderText_Blended(font.get(), "Red won!", SDL_Color{0xff, 0, 0, 0xff}));
    green_winner_text = asa::createTextureBundle(renderer, TTF_RenderText_Blended(font.get(), "Green won!", SDL_Color{0, 0xff, 0, 0xff}));

    red_winner_text.position.x = window_width / 2 - red_winner_text.position.w / 2;
    red_winner_text.position.y = window_height / 2 - red_winner_text.position.h / 2;

    green_winner_text.position.x = window_width / 2 - green_winner_text.position.w / 2;
    green_winner_text.position.y = window_height / 2 - green_winner_text.position.h / 2;

    return isLoaded(red_winner_text.texture && green_winner_text.texture);
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

void WinState::handleEvent(const SDL_Event &event)
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

void WinState::render(void)
{
    SDL_SetRenderDrawColor(renderer.get(), 0xff, 0xff, 0xff, 0xff);
    SDL_RenderClear(renderer.get());

    if (has_green_won)
    {
        SDL_RenderCopy(
            renderer.get(),
            green_winner_text.texture.get(),
            nullptr,
            &green_winner_text.position);
    }
    else
    {
        SDL_RenderCopy(
            renderer.get(),
            red_winner_text.texture.get(),
            nullptr,
            &red_winner_text.position);
    }

    SDL_RenderPresent(renderer.get());
}
