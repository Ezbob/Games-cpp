
#include "PauseState.hpp"
#include "SDL.h"
#include "SDL_ttf.h"
#include "Creators.hpp"
#include "ErrorCheck.hpp"


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
    int screen_width = 0;
    int screen_height = 0;

    SDL_GetWindowSize(m_win.get(), &screen_width, &screen_height);

    pausedText = asa::createTextureBundle(renderer, TTF_RenderText_Blended(font.get(), "Game Paused", SDL_Color{0x0, 0x0, 0x0, 0xff}));
    subText = asa::createTextureBundle(renderer, TTF_RenderText_Blended(font.get(), "(Press Enter to continue)", SDL_Color{0x0, 0x0, 0x0, 0xff}));

    pausedText.position.x = screen_width / 2 - pausedText.position.w / 2;
    pausedText.position.y = screen_height / 2 - pausedText.position.h / 2;

    subText.position.x = screen_width / 2 - subText.position.w / 2;
    subText.position.y = screen_height / 2 + subText.position.h / 2;

    return isLoaded(pausedText.texture && subText.texture);
}

void PauseState::render(void)
{
    SDL_SetRenderDrawColor(renderer.get(), 0xff, 0xff, 0xff, 0xff);
    SDL_RenderClear(renderer.get());

    SDL_RenderCopy(renderer.get(), pausedText.texture.get(), nullptr, &pausedText.position);
    SDL_RenderCopy(renderer.get(), subText.texture.get(), nullptr, &subText.position);

    SDL_RenderPresent(renderer.get());
}