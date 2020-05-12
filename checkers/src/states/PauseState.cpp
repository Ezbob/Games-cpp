
#include "PauseState.hpp"
#include "SDL.h"
#include "SDL_ttf.h"
#include "Creators.hpp"
#include "ErrorCheck.hpp"
#include "Shortcuts.hpp"

PauseState::PauseState(
    std::shared_ptr<SDL_Renderer> r,
    asa::GameStateProcessor &p,
    std::shared_ptr<TTF_Font> f, int swidth, int sheight)
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

bool PauseState::load(void)
{
    textColor = SDL_Color{
        0x0,
        0x0,
        0x0,
        0x0};

    pausedText = std::move(asa::intoTexture(renderer, TTF_RenderText_Blended(font.get(), "Game Paused", textColor)));
    subText = std::move(asa::intoTexture(renderer, TTF_RenderText_Blended(font.get(), "(Press Enter to continue)", textColor)));

    SDL_QueryTexture(pausedText.get(), nullptr, nullptr, &pausedPos.w, &pausedPos.h);
    SDL_QueryTexture(subText.get(), nullptr, nullptr, &subPos.w, &subPos.h);

    int font_height = TTF_FontHeight(font.get());

    pausedPos.x = screen_width / 2 - pausedPos.w / 2;
    pausedPos.y = screen_height / 2 - font_height / 2;

    subPos.x = screen_width / 2 - subPos.w / 2;
    subPos.y = screen_height / 2 + font_height / 2;

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