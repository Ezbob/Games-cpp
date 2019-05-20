#ifndef HEADER_GUARD_7438d22a99cec6e2475b167f3b35a0c3
#define HEADER_GUARD_7438d22a99cec6e2475b167f3b35a0c3

#include "SDL.hpp"

SDLTexture loadPNG(const SDLRenderer &renderer, const std::string &path, uint8_t r = 0, uint8_t g = 0xFF, uint8_t b = 0xFF);

SDLTexture loadSolidText(const SDLRenderer &renderer, const std::string &text, TTF_Font *font, SDL_Color textColor);

#endif
