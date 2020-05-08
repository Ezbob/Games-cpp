#include <iostream>
#include "SDL_image.h"
#include "ErrorCheck.hpp"
#include "Surface.hpp"

using namespace asa;

Surface::Surface(SDL_Surface *surface)
{
    m_contained = std::shared_ptr<SDL_Surface>(surface, SDL_FreeSurface);
    height(m_contained->h);
    width(m_contained->w);
}

/*
void Surface::loadBMP(std::string filename)
{
    m_contained = std::shared_ptr<SDL_Surface>(SDL_LoadBMP(filename.c_str()), Surface::freeingFunction);
    if (m_contained == nullptr)
    {
        std::cerr << "Error: Surface could not be initialize: " << SDL_GetError() << std::endl;
        return;
    }

    height(m_contained->h);
    width(m_contained->w);
}

void Surface::loadPNG(std::string filename)
{
    m_contained = std::shared_ptr<SDL_Surface>(IMG_Load(filename.c_str()), Surface::freeingFunction);
    if (m_contained == nullptr)
    {
        std::cerr << "Error: Surface could not be initialize: " << SDL_GetError() << std::endl;
        return;
    }

    height(m_contained->h);
    width(m_contained->w);
}
*/

void Surface::convertToFormat(const Surface &other)
{

    std::shared_ptr<SDL_Surface> optimizedSurface = nullptr;
    optimizedSurface = std::shared_ptr<SDL_Surface>(SDL_ConvertSurface(m_contained.get(), other.pixelFormat(), 0), SDL_FreeSurface);
    if (optimizedSurface == nullptr)
    {
        std::cerr << "Error: Surface replace could not be initialized; convertion failed" << std::endl;
    }
    else
    {
        m_contained.swap(optimizedSurface);
    }
}

const SDL_PixelFormat *Surface::pixelFormat() const
{
    return m_contained->format;
}

void Surface::fill(int r, int g, int b)
{
    SDL_FillRect(m_contained.get(), nullptr, rgbColor(r, g, b));
}

uint32_t Surface::rgbColor(int r, int g, int b) const
{
    const SDL_PixelFormat *mapper = pixelFormat();
    return SDL_MapRGB(mapper, r, g, b);
}

int Surface::setKeyColor(uint32_t color, int flags)
{
    return CheckError<SDL_GetError>(SDL_SetColorKey(m_contained.get(), flags, color), "Could not set key color");
}
