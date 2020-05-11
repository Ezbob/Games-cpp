
#include "PauseState.hpp"
#include "SDL.h"
#include "SDL_ttf.h"
#include "Creators.hpp"
#include "ErrorCheck.hpp"

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

    auto a = asa::createUnique(TTF_RenderText_Blended(font.get(), "Game Paused", textColor));
    auto b = asa::createUnique(TTF_RenderText_Blended(font.get(), "(Press Enter to continue)", textColor));

    auto pause_ptr = asa::createUnique(SDL_CreateTextureFromSurface(renderer.get(), a.get()));
    auto sub_ptr = asa::createUnique(SDL_CreateTextureFromSurface(renderer.get(), b.get()));

    pausedText = std::move(pause_ptr);
    subText = std::move(sub_ptr);

    SDL_QueryTexture(pausedText.get(), nullptr, nullptr, &pausedPos.w, &pausedPos.h);
    SDL_QueryTexture(subText.get(), nullptr, nullptr, &subPos.w, &subPos.h);

    pausedPos.x = screen_width / 2 - 50;
    pausedPos.y = screen_height / 2 - 12;

    subPos.x = screen_width / 2 - 140;
    subPos.y = screen_height / 2 + 14;

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