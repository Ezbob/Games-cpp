
#include "SDL.h"
#include "Color.hpp"

SDL_Color asa::asColorStruct(asa::Colors color)
{
    uint8_t r = 0x0, g = 0x0, b = 0x0, a = 0xff;
    switch (color)
    {
    case asa::Colors::WHITE:
        r = 0xff;
        g = 0xff;
        b = 0xff;
        break;

    case asa::Colors::RED:
        r = 0xff;
        break;

    case asa::Colors::BLUE:
        b = 0xff;
        break;

    case asa::Colors::GREEN:
        g = 0xff;
        break;

    case asa::Colors::YELLOW:
        r = 0xff;
        g = 0xff;
        break;

    case asa::Colors::CYAN:
        g = 0xff;
        b = 0xff;
        break;

    case asa::Colors::BLACK:
        break;
    }
    return SDL_Color{r, g, b, a};
}
