
#include "SDLLibrary.hpp"
#include "SDL.h"
#include "sdl2cpp/ErrorCheck.hpp"

using namespace asa;

SDLLibrary::SDLLibrary(uint32_t flags)
{
    ThrowOnError<SDL_GetError>(SDL_Init(flags), "Could not initialize SDL2");
    m_is_initialized = true;
}

SDLLibrary::~SDLLibrary()
{
    if (m_is_initialized)
    {
        SDL_Quit();
    }
}