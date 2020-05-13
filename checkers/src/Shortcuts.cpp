
#include "Shortcuts.hpp"

#include "Creators.hpp"
#include "ErrorCheck.hpp"

std::unique_ptr<SDL_Texture, asa::TextureDeleter> asa::intoTexture(std::shared_ptr<SDL_Renderer> rend, SDL_Surface *surf)
{
    asa::ThrowOnNull(surf, "Could not convert surface to texture; texture not loaded.");
    auto texture = asa::make_sdl_unique(SDL_CreateTextureFromSurface(rend.get(), surf));
    delete surf;
    return texture;
}
