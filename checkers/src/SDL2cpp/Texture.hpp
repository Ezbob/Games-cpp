#pragma once

#include "Surface.hpp"
#include "sdl_render.h"
#include "MemoryContainer.hpp"
#include "Surface.hpp"

namespace asa {

    class Texture : public SharedContainerBase<SDL_Texture, SDL_DestroyTexture> {

    public:
        Texture(SDL_Renderer *renderer);

        void load(SDL_Texture *texture, int w, int h);
        void load(SDL_Texture *texture);
        void load(SDL_Surface *surface);
        void load(Texture &&texture);
        void load(Surface &surface);

        int getHeight() const;
        int getWidth() const;

        void render(const int x = 0, const int y = 0);
        void render(const int x, const int y, const SDL_Rect &clip);
        void render(const int x, const int y, const SDL_Rect &clip, SDL_RendererFlip &flip);
        void render(const int x, const int y, SDL_RendererFlip &flip);

    private:
        SDL_Renderer *m_renderer = nullptr;
        int m_width;
        int m_height;
    };
}
