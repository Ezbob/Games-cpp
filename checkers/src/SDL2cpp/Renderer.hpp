#pragma once

#include "SDL_video.h"
#include "SDL_render.h"
#include "SDL_rect.h"
#include "Color.hpp"
#include "ErrorCheck.hpp"
#include "container_bases/SharedContainerBase.hpp"
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
        Renderer(Window &window, int index = -1, uint32_t rendererFlags = SDL_RENDERER_ACCELERATED);

        bool clear(void);
        bool copyTexture(Texture &texture, SDL_Rect *src = nullptr, SDL_Rect *dest = nullptr);
        void updateScreen() const;

        bool setViewPort(SDL_Rect &rect);

        Texture createTextureFromSurface(Surface const&);

        bool setColor(int r, int g, int b, int a = 0xFF);
        bool setColor(asa::Colors);

        /* single primitive */

        bool drawRect(const SDL_Rect &);
        bool fillRect(const SDL_Rect &);

        bool drawLine(int x1, int y1, int x2, int y2);
        bool drawPoint(int x, int y);

        bool drawRects(const std::vector<SDL_Rect> &);
        bool fillRects(const std::vector<SDL_Rect> &);

        /* sets of primitives */

        template <std::size_t N>
        bool drawRects(const std::array<SDL_Rect, N> &rects, std::size_t first = 0, std::size_t last = N)
        {
            static_assert(N != 0, "zero length array parsed to drawRects");
            std::size_t distance = (last - first);
            return CheckError<SDL_GetError>(SDL_RenderDrawRects(m_contained.get(), rects.data() + first, distance < 0 ? (-distance) : distance), "Could not fill rectangle");
        }

        template <std::size_t N>
        bool drawRects(const SDL_Rect (&rects)[N], std::size_t first = 0, std::size_t last = N)
        {
            static_assert(N != 0, "zero length array parsed to drawRects");
            std::size_t distance = (last - first);
            return CheckError<SDL_GetError>(SDL_RenderDrawRects(m_contained.get(), rects.data() + first, distance < 0 ? (-distance) : distance), "Could not fill rectangle");
        }

        template <std::size_t N>
        bool fillRects(const std::array<SDL_Rect, N> &filler, std::size_t first = 0, std::size_t last = N)
        {
            static_assert(N != 0, "zero length array parsed to fillRects");
            std::size_t distance = (last - first);
            return CheckError<SDL_GetError>(SDL_RenderFillRects(m_contained.get(), filler.data() + first, distance < 0 ? (-distance) : distance), "Could not fill rectangles");
        }

        template <std::size_t N>
        bool fillRects(const SDL_Rect (&filler)[N], std::size_t first = 0, std::size_t last = N)
        {
            static_assert(N != 0, "zero length array parsed to fillRects");
            std::size_t distance = (last - first);
            auto return_code = SDL_RenderFillRects(m_contained.get(), filler.data() + first, distance < 0 ? (-distance) : distance);
            return CheckError<SDL_GetError>(return_code, "Could not fill rectangles");
        }

        template <std::size_t N>
        bool drawPoints(const SDL_Point (&points)[N])
        {
            static_assert(N != 0, "zero length array parsed to drawRects");
            auto return_code = SDL_RenderDrawPoints(m_contained.get(), points, static_cast<int>(N));
            return CheckError<SDL_GetError>(return_code, "Could not draw points");
        }

        template <std::size_t N>
        bool drawPoints(const std::array<SDL_Point, N> &points)
        {
            static_assert(N != 0, "zero length array parsed to drawRects");
            auto return_code = SDL_RenderDrawPoints(m_contained.get(), points.data(), static_cast<int>(N));
            return CheckError<SDL_GetError>(return_code, "Could not draw points");
        }

        bool drawCircle(int centerX, int centerY, int radius);

        bool fillCircle(int centerX, int centerY, int radius);

        SDL_BlendMode drawBlendMode(void);

        void drawBlendMode(SDL_BlendMode);

    private:
        Window &m_window;
    };

} // namespace asa
