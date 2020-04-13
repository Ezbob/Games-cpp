#ifndef HEADER_GUARD_ae78126d2bc5ee4610e84cb0f11eac44
#define HEADER_GUARD_ae78126d2bc5ee4610e84cb0f11eac44

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include <memory>
#include "SDL2cpp/MemoryContainer.hpp"

namespace sdl2 {

    class Surface : public SharedContainerBase<SDL_Surface, SDL_FreeSurface> {

    public:
        Surface(SDL_Surface *surface);
        Surface();

        void loadBMP(std::string filename);
        void loadPNG(std::string filename);
        void loadSolidText(std::string text, TTF_Font &font, SDL_Color color = {0, 0, 0, 0});

        int setKeyColor(int flags, uint32_t color);

        int getHeight() const {
            return m_contained->h;
        }

        int getWidth() const {
            return m_contained->w;
        }

        /**
         * Convert the internal surface pixel format to the other surface's pixelformat
         */
        void convertToFormat(const Surface &other);

        const SDL_PixelFormat *pixelFormat() const;
        void fill(int r, int g, int b);
        uint32_t rgbColor(int r, int g, int b) const;

    };

}

#endif