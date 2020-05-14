
#pragma once

#include <memory>
#include "SDL.h"
#include "SmartPointers.hpp"

namespace asa {

    asa::sdl_unique_ptr<SDL_Texture> intoTexture(std::shared_ptr<SDL_Renderer>, SDL_Surface *);

    std::string getBasePath(void);

}; // namespace asa
