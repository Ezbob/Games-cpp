
#pragma once

#include <cstdint>
#include "SDL.h"

namespace asa
{

    class SDLLibrary
    {
    private:
        bool m_is_initialized = false;

    public:
        SDLLibrary(uint32_t flags = SDL_INIT_VIDEO | SDL_INIT_TIMER);
        ~SDLLibrary();
    };

}; // namespace asa
