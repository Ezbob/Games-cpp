#ifndef HEADER_GUARD_a04ab214bef1adcd47b92de3a2c703e2
#define HEADER_GUARD_a04ab214bef1adcd47b92de3a2c703e2

#include "SDL.hpp"
#include "SDL2cpp/MemoryContainer.hpp"
#include <array>
#include <vector>


namespace sdl2 {

    class Texture;
    class Window;

    enum class Colors {
        BLACK,
        WHITE,
        RED,
        GREEN,
        YELLOW,
        BLUE,
        CYAN
    };

    class Renderer : public SharedContainerBase<SDL_Renderer, SDL_DestroyRenderer> {

    public:
        Renderer(SDL_Window *window, int index, uint32_t rendererFlags);
        Renderer() = default;

        void load(SDL_Window *window, int index, uint32_t rendererFlags);
        void load(Window &window, int index, uint32_t rendererFlags);
        SDL_Texture *createTextureFromSurface(SDL_Surface &surface);
        SDL_Texture *createTextureFromSurface(SDL_Surface *surface);

        bool clear();
        bool copyTexture(Texture &texture, SDL_Rect *src = nullptr, SDL_Rect *dest = nullptr);
        void updateScreen() const;
        bool setColor(int r, int g, int b, int a = 0xFF);
        bool setColor(Colors);

        bool drawRect(const SDL_Rect *fillRect);
        bool drawRect(const SDL_Rect &fillRect);

        template<std::size_t N>
        bool drawRects(const std::array<SDL_Rect, N> &rects) {
            return CheckError<SDL_GetError>(SDL_RenderDrawRects(m_contained.get(), rects.data(), N), "Could not fill rectangle");
        }

        bool drawRects(const std::vector<SDL_Rect> &);

        bool fillRect(const SDL_Rect *);
        bool fillRect(const SDL_Rect &);
        bool fillRects(const std::vector<SDL_Rect> &);

        template<std::size_t N>
        bool fillRects(const std::array<SDL_Rect, N> &fillRect) {
            return CheckError<SDL_GetError>(SDL_RenderFillRects(m_contained.get(), fillRect.data(), N), "Could not fill rectangle");
        }

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
