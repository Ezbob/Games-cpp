#ifndef HEADER_GUARD_43907dcfaf9a29402dbd7293e1b778b8
#define HEADER_GUARD_43907dcfaf9a29402dbd7293e1b778b8

#include "sdl.h"

namespace sdl2 {
    enum class Colors {
        BLACK,
        WHITE,
        RED,
        GREEN,
        YELLOW,
        BLUE,
        CYAN
    };

    SDL_Color asColorStruct(sdl2::Colors color);
}

#endif
