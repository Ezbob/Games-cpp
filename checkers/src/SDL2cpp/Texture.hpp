#pragma once

#include "Surface.hpp"
#include "sdl_render.h"
#include "container_bases/SharedContainerBase.hpp"
#include "Surface.hpp"
#include "Sized2DEntity.hpp"

namespace asa
{

    class Texture : public SharedContainerBase<SDL_Texture, SDL_DestroyTexture>, public Sized2DEntity<Texture>
    {

    public:
        Texture(SDL_Renderer *renderer);

        void load(SDL_Texture *texture, int w, int h);
        void load(SDL_Texture *texture);
        void load(SDL_Surface *surface);
        void load(Texture &&texture);
        void load(Surface &surface);

        void render(const SDL_Rect &quad);
        void render(const int x = 0, const int y = 0);
        void render(const int x, const int y, const SDL_Rect &clip);
        void render(const int x, const int y, const SDL_Rect &clip, SDL_RendererFlip &flip);
        void render(const int x, const int y, SDL_RendererFlip &flip);

        void blendMode(SDL_BlendMode mode);
        SDL_BlendMode blendMode(void);

        void alphaMod(uint8_t mod);
        uint8_t alphaMod(void);

    private:
        SDL_Renderer *m_renderer = nullptr;
    };
} // namespace asa
