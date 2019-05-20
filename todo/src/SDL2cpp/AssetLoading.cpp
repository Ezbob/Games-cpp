#include "SDL2cpp/AssertLoading.hpp"

SDLTexture loadPNG(const SDLRenderer &renderer, const std::string &path, uint8_t r = 0, uint8_t g = 0xFF, uint8_t b = 0xFF) {
    SDLTexture texture = renderer.createTexture();

    SDLSurface loadedSurface;
    loadedSurface.loadPNG(path);
    if ( loadedSurface.isLoaded() ) {
        loadedSurface.setKeyColor(SDL_TRUE, SDL_MapRGB( loadedSurface.pixelFormat(), r, g, b ));
        texture.load(loadedSurface);
    }

    return texture;
}

SDLTexture loadSolidText(const SDLRenderer &renderer, const std::string &text, TTF_Font *font, SDL_Color textColor) {
    SDLTexture texture = renderer.createTexture();

    SDLSurface surface;

    surface.load(TTF_RenderText_Solid(font, text.c_str(), textColor));

    if (surface.isLoaded()) {
        texture.load(surface);
    }

    return texture;
}
