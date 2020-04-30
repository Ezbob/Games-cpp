#pragma once

#include "SDL_video.h"
#include "SDL_render.h"
#include "SDL_rect.h"
#include "Color.hpp"
#include "MemoryContainer.hpp"
#include "Window.hpp"
#include "Texture.hpp"
#include "Font.hpp"
#include <array>
#include <vector>
#include <iterator>
#include <cmath>

namespace asa
{

class Renderer : public SharedContainerBase<SDL_Renderer, SDL_DestroyRenderer>
{

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
    bool setColor(asa::Colors);

    bool drawRect(const SDL_Rect *fillRect);
    bool drawRect(const SDL_Rect &fillRect);

    template <std::size_t N>
    bool drawRects(const std::array<SDL_Rect, N> &rects, std::size_t first = 0,  std::size_t last = N)
    {
        static_assert(N != 0, "zero length array parsed to drawRects");
        std::size_t distance = (last - first);
        return CheckError<SDL_GetError>(SDL_RenderDrawRects(m_contained.get(), rects.data() + first, distance < 0 ? (-distance) : distance), "Could not fill rectangle");
    }

    template <std::size_t N>
    bool drawRects(const SDL_Rect (&rects)[N], std::size_t first = 0,  std::size_t last = N)
    {
        static_assert(N != 0, "zero length array parsed to drawRects");
        std::size_t distance = (last - first);
        return CheckError<SDL_GetError>(SDL_RenderDrawRects(m_contained.get(), rects.data() + first, distance < 0 ? (-distance) : distance), "Could not fill rectangle");
    }

    bool drawRects(const std::vector<SDL_Rect> &);

    bool fillRect(const SDL_Rect *);
    bool fillRect(const SDL_Rect &);
    bool fillRects(const std::vector<SDL_Rect> &);

    template<std::size_t N>
    bool fillRects(const std::array<SDL_Rect, N> &filler, std::size_t first = 0,  std::size_t last = N) {
        static_assert(N != 0, "zero length array parsed to fillRects");
        std::size_t distance = (last - first);
        return CheckError<SDL_GetError>(SDL_RenderFillRects(m_contained.get(), filler.data() + first, distance < 0 ? (-distance) : distance), "Could not fill rectangles");
    }

    template <std::size_t N>
    bool fillRects(const SDL_Rect (&filler)[N], std::size_t first = 0,  std::size_t last = N)
    {
        static_assert(N != 0, "zero length array parsed to fillRects");
        std::size_t distance = (last - first);
        return CheckError<SDL_GetError>(SDL_RenderFillRects(m_contained.get(), filler.data() + first, distance < 0 ? (-distance) : distance), "Could not fill rectangles");
    }

    bool drawLine(int x1, int y1, int x2, int y2);
    bool drawPoint(int x, int y);
    bool setViewPort(SDL_Rect &rect);

    /**
         * Factory method for creating new textures
         */
    Texture createTexture() const;

    Texture loadPNG(const std::string &path, uint8_t r = 0, uint8_t g = 0xFF, uint8_t b = 0xFF) const;

    Texture loadSolidText(const std::string &text, TTFFont &font, SDL_Color textColor) const;

    Texture loadBlendedText(const std::string &text, TTFFont &font, SDL_Color textColor) const;

private:
    SDL_Window *m_window_parent = nullptr;
};

} // namespace asa
