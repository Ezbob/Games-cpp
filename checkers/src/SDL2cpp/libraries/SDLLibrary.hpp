
#pragma once

#include <cstdint>

namespace asa
{

    class SDLLibrary
    {
    private:
        bool m_is_initialized = false;

    public:
        SDLLibrary(uint32_t flags);
        ~SDLLibrary();
    };

}; // namespace asa
