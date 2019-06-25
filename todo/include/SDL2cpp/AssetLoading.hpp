#ifndef HEADER_GUARD_7438d22a99cec6e2475b167f3b35a0c3
#define HEADER_GUARD_7438d22a99cec6e2475b167f3b35a0c3

#include "SDL2cpp/Texture.hpp"
#include "SDL2cpp/Renderer.hpp"

namespace sdl2 {

    class Texture;

    Texture loadPNG(const Renderer &renderer, const std::string &path, uint8_t r = 0, uint8_t g = 0xFF, uint8_t b = 0xFF);

    Texture loadSolidText(const Renderer &renderer, const std::string &text, TTF_Font *font, SDL_Color textColor);
}

#endif
