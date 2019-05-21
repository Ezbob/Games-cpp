#ifndef HEADER_GUARD_a04ab214bef1adcd47b92de3a2c703e2
#define HEADER_GUARD_a04ab214bef1adcd47b92de3a2c703e2

#include "SDL.hpp"
#include "SDL2cpp/MemoryContainer.hpp"


namespace sdl2 {

    class Texture;
    class Window;

    class Renderer : public SharedContainerBase<Renderer, SDL_Renderer, SDL_DestroyRenderer> {

    public:
        Renderer(SDL_Window *window, int index, uint32_t rendererFlags);
        Renderer();

        void load(SDL_Window *window, int index, uint32_t rendererFlags);
        void load(Window &window, int index, uint32_t rendererFlags);
        SDL_Texture *createTextureFromSurface(SDL_Surface &surface);
        SDL_Texture *createTextureFromSurface(SDL_Surface *surface);

        bool clear();
        bool copyTexture(Texture &texture, SDL_Rect *src = nullptr, SDL_Rect *dest = nullptr);
        void updateScreen() const;
        bool setColor(int r, int g, int b, int a = 0xFF);
        bool drawRect(const SDL_Rect *fillRect);
        bool drawRect(const SDL_Rect &fillRect);
        bool fillRect(const SDL_Rect *fillRect);
        bool fillRect(const SDL_Rect &fillRect);
        bool drawLine(int x1, int y1, int x2, int y2);
        bool drawPoint(int x, int y);
        bool setViewPort(SDL_Rect &rect);

        /**
         * Factory method for creating new textures
         */
        Texture createTexture() const;

    private:
        SDL_Window *m_window_parent = nullptr;
    };

}

#endif
