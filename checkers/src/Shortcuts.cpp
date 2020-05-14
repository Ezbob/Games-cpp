
#include "Shortcuts.hpp"
#include "ErrorCheck.hpp"
#include <string>

asa::sdl_unique_ptr<SDL_Texture> asa::intoTexture(std::shared_ptr<SDL_Renderer> rend, SDL_Surface *surf)
{
    asa::ThrowOnNull(surf, "Could not convert surface to texture; texture not loaded.");
    auto texture = asa::make_sdl_unique(SDL_CreateTextureFromSurface(rend.get(), surf));
    delete surf;
    return texture;
}

std::string asa::getBasePath(void) {
    return std::string(std::unique_ptr<const char[]>(SDL_GetBasePath()).get());
}