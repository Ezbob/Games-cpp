
#include "SDLLibrary.hpp"
#include <stdexcept>
#include <string>

SDLLibrary::SDLLibrary(uint32_t flags)
{
    if (SDL_init(flags) != 0)
    {
        throw std::runtime_error("SDL library could not be initialized: " + SDL_GetError());
    }
    else
    {
        m_is_initialized = true;
    }
}

SDLLibrary::~SDLLibrary()
{
    if (m_is_initialized)
    {
        SQL_Quit();
    }
}