
#include "Shortcuts.hpp"

#include "Creators.hpp"
#include "ErrorCheck.hpp"

std::unique_ptr<SDL_Texture, asa::TextureDeleter> asa::intoTexture(std::shared_ptr<SDL_Renderer> rend, SDL_Surface *surf) {
    asa::ThrowOnNull(surf, "Could not convert surface to texture; texture not loaded.");
    auto texture = asa::createUnique(SDL_CreateTextureFromSurface(rend.get(), surf));
    delete surf;
    return std::move(texture);
}
