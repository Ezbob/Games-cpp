
#include "TTFLibrary.hpp"
#include "SDL_ttf.h"
#include <stdexcept>

TTFLibrary::TTFLibrary(void)
{
    if (TTF_Init() != 0)
    {
        throw std::runtime_error("TTF library could not be initialized");
    }
    else
    {
        m_is_initialized = true;
    }
}

TTFLibrary::~TTFLibrary(void)
{
    if (m_is_initialized)
    {
        TTF_Quit();
    }
}