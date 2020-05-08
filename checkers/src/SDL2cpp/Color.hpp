#pragma once

#include "SDL_pixels.h"

namespace asa
{
    enum class Colors
    {
        BLACK,
        WHITE,
        RED,
        GREEN,
        YELLOW,
        BLUE,
        CYAN
    };

    SDL_Color asColorStruct(asa::Colors color);
} // namespace asa
