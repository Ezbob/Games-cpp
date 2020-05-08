
#pragma once

#include "SDL_image.h"
#include <cstdint>

namespace asa
{

    class ImageLibrary
    {
    private:
        bool m_is_initialized = false;

    public:
        ImageLibrary(std::uint32_t);
        ~ImageLibrary(void);
    };

}; // namespace asa
