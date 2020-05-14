
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

std::string asa::getBasePath(void)
{
    return std::string(std::unique_ptr<const char[]>(SDL_GetBasePath()).get());
}

asa::TextureBundle asa::createTextureBundle(std::shared_ptr<SDL_Renderer> rend, SDL_Surface *s)
{
    auto texture = intoTexture(rend, s);
    TextureBundle r;
    ThrowOnNonZero<SDL_GetError>(
        SDL_QueryTexture(texture.get(), nullptr, nullptr, &r.position.w, &r.position.h),
        "Could not get texture dimensions");
    r.texture = std::move(texture);
    return r;
}