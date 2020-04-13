#ifndef HEADER_GUARD_e73311f1c7af9dc9fb705468721e084a
#define HEADER_GUARD_e73311f1c7af9dc9fb705468721e084a

#include "Renderer.hpp"
#include "Surface.hpp"
#include "SDL2/SDL.h"
#include "SDL2cpp/MemoryContainer.hpp"


namespace sdl2 {

    class Renderer;

    class Texture : public SharedContainerBase<SDL_Texture, SDL_DestroyTexture> {

    public:
        Texture(Renderer &renderer);
        Texture(SDL_Renderer *renderer);

        void load(SDL_Texture *texture, int w, int h);
        void load(SDL_Texture *texture);
        void load(SDL_Surface *surface);
        void load(Texture &&texture);
        void load(Surface &surface);

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
}

#endif