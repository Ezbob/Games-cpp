
#include "WinState.hpp"
#include "SDL.h"
#include "ErrorCheck.hpp"
#include "Shortcuts.hpp"
#include <memory>

WinState::WinState(
    std::shared_ptr<SDL_Renderer> r,
    asa::GameStateProcessor &p,
    std::shared_ptr<TTF_Font> f,
    std::shared_ptr<SDL_Window> win,
    asa::MessageQueueInterface &mq) : renderer(r), processor(p), font(f), m_win(win), m_comms(mq)
{
}

bool WinState::load(void)
{
    int window_width = 0, window_height = 0;
    auto redtxt = "Red won!";
    auto greentxt = "Green won!";

    red_winner_text = std::move(asa::intoTexture(renderer, TTF_RenderText_Blended(font.get(), redtxt, SDL_Color{0xff, 0, 0, 0xff})));
    green_winner_text = std::move(asa::intoTexture(renderer, TTF_RenderText_Blended(font.get(), greentxt, SDL_Color{0, 0xff, 0, 0xff})));

    TTF_SizeText(font.get(), redtxt, &red_pos.w, &red_pos.h);
    TTF_SizeText(font.get(), greentxt, &green_pos.w, &green_pos.h);

    red_pos.x = window_width / 2 - red_pos.w / 2;
    red_pos.y = window_height / 2 - red_pos.h / 2;

    green_pos.x = window_width / 2 - green_pos.w / 2;
    green_pos.y = window_height / 2 - green_pos.h / 2;

    return isLoaded(red_winner_text && green_winner_text);
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
    SDL_SetRenderDrawColor(renderer.get(), 0x0, 0x0, 0x0, 0xff);
    SDL_RenderClear(renderer.get());

    if (has_green_won)
    {
        SDL_RenderCopy(renderer.get(), green_winner_text.get(), nullptr, &green_pos);
    }
    else
    {
        SDL_RenderCopy(renderer.get(), red_winner_text.get(), nullptr, &red_pos);
    }

    SDL_RenderPresent(renderer.get());
}
