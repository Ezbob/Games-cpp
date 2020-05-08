
#pragma once

#include <string>
#include "Renderer.hpp"
#include "SDL_pixels.h"

namespace asa {

    Texture loadPNG(Renderer &renderer, const std::string &path, SDL_Color &keyColor);
    Texture loadPNG(Renderer &renderer, const std::string &path);

}; // namespace asa
