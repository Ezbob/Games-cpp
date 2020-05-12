
#pragma once

#include <memory>
#include "SDL.h"
#include "Deleter.hpp"

namespace asa {

    std::unique_ptr<SDL_Texture, asa::TextureDeleter> intoTexture(std::shared_ptr<SDL_Renderer> , SDL_Surface *);

}; // namespace asa
