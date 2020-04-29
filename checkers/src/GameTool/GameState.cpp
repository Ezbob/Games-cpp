#include "GameState.hpp"
#include <iostream>
#include "SDL.h"

using namespace asa;

void GameState::pumpEvents()
{
    static SDL_Event inputEvent;
    while (SDL_PollEvent(&inputEvent) != 0)
    {
        handleEvent(inputEvent);
    }
    handleKeyState(SDL_GetKeyboardState(nullptr));
}

void GameState::handleEvent(const SDL_Event &event)
{
    if (event.type == SDL_QUIT)
    {
        isPlaying(false);
    }
}

void GameState::handleKeyState(const uint8_t *keyState [[maybe_unused]])
{
}

bool GameState::load()
{
    return this->isLoaded(true);
}

void GameState::update() {}
void GameState::render() {}

bool GameState::isLoaded() const
{
    return m_isLoaded;
}

bool GameState::isPlaying() const
{
    return m_isPlaying;
}

bool GameState::isPlaying(bool is_playing)
{
    return (m_isPlaying = is_playing);
}

bool GameState::isLoaded(bool is_loaded)
{
    return (m_isLoaded = is_loaded);
}