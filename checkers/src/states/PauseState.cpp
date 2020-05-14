
#include "PauseState.hpp"
#include "SDL.h"
#include "SDL_ttf.h"
#include "Creators.hpp"
#include "ErrorCheck.hpp"
#include "Shortcuts.hpp"

PauseState::PauseState(
    std::shared_ptr<SDL_Renderer> r,
    asa::GameStateProcessor &p,
    std::shared_ptr<TTF_Font> f,
    std::shared_ptr<SDL_Window> win)
    : renderer(r), font(f), processor(p), m_win(win) {}

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

bool PauseState::load(void)
{

    auto ptext = "Game Paused";
    auto stext = "(Press Enter to continue)";

    int screen_width = 0;
    int screen_height = 0;

    SDL_GetWindowSize(m_win.get(), &screen_width, &screen_height);

    auto pause_ptr = asa::intoTexture(renderer, TTF_RenderText_Blended(font.get(), ptext, SDL_Color{0x0, 0x0, 0x0, 0x0}));
    pausedText = std::move(pausedText);

    auto sub_ptr = asa::intoTexture(renderer, TTF_RenderText_Blended(font.get(), stext, SDL_Color{0x0, 0x0, 0x0, 0x0}));
    subText = std::move(sub_ptr);

    TTF_SizeText(font.get(), ptext, &pausedPos.w, &pausedPos.h);

    pausedPos.x = screen_width / 2 - pausedPos.w / 2;
    pausedPos.y = screen_height / 2 - pausedPos.h / 2;

    TTF_SizeText(font.get(), stext, &subPos.w, &subPos.h);

    subPos.x = screen_width / 2 - subPos.w / 2;
    subPos.y = screen_height / 2 + subPos.h / 2;

    return isLoaded(pausedText && subText);
}

void PauseState::render(void)
{
    SDL_SetRenderDrawColor(renderer.get(), 0xff, 0xff, 0xff, 0xff);
    SDL_RenderClear(renderer.get());

    SDL_RenderCopy(renderer.get(), pausedText.get(), nullptr, &pausedPos);
    SDL_RenderCopy(renderer.get(), subText.get(), nullptr, &subPos);

    SDL_RenderPresent(renderer.get());
}