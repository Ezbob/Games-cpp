#pragma once

#include "Texture.hpp"
#include "Renderer.hpp"
#include "SDL_pixels.h"
#include "SDL_ttf.h"
#include <string>
#include <cstdint>

namespace asa
{

Texture loadPNG(const Renderer &renderer, const std::string &path, uint8_t r = 0, uint8_t g = 0xFF, uint8_t b = 0xFF);

Texture loadSolidText(const Renderer &renderer, const std::string &text, TTF_Font *font, SDL_Color textColor);

Texture loadBlendedText(const Renderer &renderer, const std::string &text, TTF_Font *font, SDL_Color textColor);

} // namespace asa
