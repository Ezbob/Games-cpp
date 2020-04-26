#pragma once

#include "SDL_surface.h"
#include "SDL_ttf.h"
#include <memory>
#include "MemoryContainer.hpp"

namespace asa {

    class Surface : public SharedContainerBase<SDL_Surface, SDL_FreeSurface> {

    public:
        Surface(SDL_Surface *surface);
        Surface();

        void loadBMP(std::string filename);
        void loadPNG(std::string filename);

        int setKeyColor(int flags, uint32_t color);

        int getHeight() const;
        int getWidth() const;

        /**
         * Convert the internal surface pixel format to the other surface's pixelformat
         */
        void convertToFormat(const Surface &other);

        const SDL_PixelFormat *pixelFormat() const;
        void fill(int r, int g, int b);
        uint32_t rgbColor(int r, int g, int b) const;

    };

}