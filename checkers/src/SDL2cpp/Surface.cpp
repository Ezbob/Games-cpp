#include <iostream>
#include "SDL2/SDL_image.h"
#include "SDL2cpp/ErrorCheck.hpp"
#include "SDL2cpp/Surface.hpp"

using namespace sdl2;

Surface::Surface(SDL_Surface *surface) {
    m_contained = std::shared_ptr<SDL_Surface>(surface, SDL_FreeSurface);
}

Surface::Surface() {}

void Surface::loadBMP(std::string filename) {
    m_contained = std::shared_ptr<SDL_Surface>(SDL_LoadBMP(filename.c_str()), Surface::freeingFunction);
    if (m_contained == nullptr) {
        std::cerr << "Error: Surface could not be initialize: " << SDL_GetError() << std::endl;
        return;
    }
}

void Surface::loadPNG(std::string filename) {
    m_contained = std::shared_ptr<SDL_Surface>(IMG_Load(filename.c_str()), Surface::freeingFunction);
    if ( !m_contained ) {
        std::cerr << "Error: Surface could not be initialize: " << SDL_GetError() << std::endl;
        return;
    }
}

void Surface::loadSolidText(std::string text, TTF_Font &font, SDL_Color color) {
    m_contained = std::shared_ptr<SDL_Surface>(TTF_RenderText_Solid(&font, text.c_str(), color));
    if (m_contained == nullptr) {
        std::cerr << "Error: Surface could not be initialize: " << TTF_GetError() << std::endl;
        return;
    }
}

void Surface::convertToFormat(const Surface &other) {

    std::shared_ptr<SDL_Surface> optimizedSurface = nullptr;
    optimizedSurface = std::shared_ptr<SDL_Surface>(SDL_ConvertSurface(m_contained.get(), other.pixelFormat(), 0), SDL_FreeSurface);
    if (optimizedSurface == nullptr) {
        std::cerr << "Error: Surface replace could not be initialized; convertion failed" << std::endl;
    } else {
        m_contained.swap(optimizedSurface);
        //m_contained = optimizedSurface;
    }
}

const SDL_PixelFormat *Surface::pixelFormat() const {
    return m_contained->format;
}

void Surface::fill(int r, int g, int b) {
    SDL_FillRect(m_contained.get(), nullptr, rgbColor(r, g, b));
}

uint32_t Surface::rgbColor(int r, int g, int b) const {
    const SDL_PixelFormat *mapper = pixelFormat();
    return SDL_MapRGB(mapper, r, g, b);
}

int Surface::setKeyColor(int flags, uint32_t color) {
    return CheckError<SDL_GetError>(SDL_SetColorKey(m_contained.get(), flags, color), "Could not set key color");
}
