
#include "TTFLibrary.hpp"
#include "SDL_ttf.h"
#include <stdexcept>
#include "sdl2cpp/ErrorCheck.hpp"

using namespace asa;

TTFLibrary::TTFLibrary(void)
{
    ThrowOnError<NoErrorGetter>(TTF_Init(), "TTF library could not be initialized");
    m_is_initialized = true;
}

TTFLibrary::~TTFLibrary(void)
{
    if (m_is_initialized)
    {
        TTF_Quit();
    }
}