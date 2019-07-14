
#include "SDL.h"
#include "sdl2cpp/Color.hpp"

SDL_Color sdl2::asColorStruct(sdl2::Colors color) {
    uint8_t r = 0x0, g = 0x0, b = 0x0, a = 0xff;
    switch (color) {
        case sdl2::Colors::WHITE:
            r = 0xff;
            g = 0xff;
            b = 0xff;
            break;

        case sdl2::Colors::RED:
            r = 0xff;
            break;

        case sdl2::Colors::BLUE:
            b = 0xff;
            break;

        case sdl2::Colors::GREEN:
            g = 0xff;
            break;

        case sdl2::Colors::YELLOW:
            r = 0xff;
            g = 0xff;
            break;

        case sdl2::Colors::CYAN:
            g = 0xff;
            b = 0xff;
            break;

        case sdl2::Colors::BLACK:
            break;
    }
    return SDL_Color { r, g, b, a };
}

