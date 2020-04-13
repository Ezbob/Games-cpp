#include "SDL2cpp/AssetLoading.hpp"

namespace sdl2 {
    Texture loadPNG(const Renderer &renderer, const std::string &path, uint8_t r, uint8_t g, uint8_t b) {
        Texture texture = renderer.createTexture();

        Surface loadedSurface;
        loadedSurface.loadPNG(path);
        if ( loadedSurface.isLoaded() ) {
            loadedSurface.setKeyColor(SDL_TRUE, SDL_MapRGB( loadedSurface.pixelFormat(), r, g, b ));
            texture.load(loadedSurface);
        }

        return texture;
    }

    Texture loadSolidText(const Renderer &renderer, const std::string &text, TTF_Font *font, SDL_Color textColor) {
        Texture texture = renderer.createTexture();

        Surface surface;

        surface.load(TTF_RenderText_Solid(font, text.c_str(), textColor));

        if (surface.isLoaded()) {
            texture.load(surface);
        }

        return texture;
    }

    Texture loadBlendedText(const Renderer &renderer, const std::string &text, TTF_Font *font, SDL_Color textColor) {
        Texture texture = renderer.createTexture();

        Surface surface;

        surface.load(TTF_RenderText_Blended(font, text.c_str(), textColor));

        if (surface.isLoaded()) {
            texture.load(surface);
        }

        return texture;
    }
}


