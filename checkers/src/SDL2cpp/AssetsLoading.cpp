
#include "AssetsLoading.hpp"
#include "SDL_image.h"

asa::Texture asa::loadPNG(Renderer &renderer, const std::string &path, SDL_Color &keyColor)
{
    Surface loadedSurface(IMG_Load(path.c_str()));
    loadedSurface.setKeyColor(SDL_MapRGB(loadedSurface.pixelFormat(), keyColor.r, keyColor.g, keyColor.b));

    return renderer.createTextureFromSurface(loadedSurface);
}

asa::Texture asa::loadPNG(Renderer &renderer, const std::string &path)
{
    Surface loadedSurface(IMG_Load(path.c_str()));
    return renderer.createTextureFromSurface(loadedSurface);
}
