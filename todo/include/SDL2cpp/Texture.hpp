#ifndef HEADER_GUARD_e73311f1c7af9dc9fb705468721e084a
#define HEADER_GUARD_e73311f1c7af9dc9fb705468721e084a

#include "Renderer.hpp"
#include "Surface.hpp"
#include "SDL.h"
#include "SDL2cpp/MemoryContainer.hpp"

class SDLRenderer;

class SDLTexture : public SharedContainerBase<SDLTexture, SDL_Texture, SDL_DestroyTexture> {

public:
    SDLTexture(SDLRenderer &renderer);
    SDLTexture(SDL_Renderer *renderer);

    void load(SDL_Texture *texture, int w, int h);
    void load(SDL_Texture *texture);
    void load(SDL_Surface *surface);
    void load(SDLSurface &surface);

    int getHeight() const {
        return m_height;
    }

    int getWidth() const {
        return m_width;
    }

    void render(const int x = 0, const int y = 0);
    void render(const int x, const int y, const SDL_Rect &clip);
    void render(const int x, const int y, const SDL_Rect &clip, SDL_RendererFlip &flip);
    void render(const int x, const int y, SDL_RendererFlip &flip);

private:
    SDL_Renderer *m_renderer;
    int m_width;
    int m_height;
};

#endif
