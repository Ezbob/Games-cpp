#include "Renderer.hpp"
#include "Texture.hpp"
#include "SDL.h"
#include <memory>

using namespace asa;

Renderer::Renderer(SDL_Window *window, int index, uint32_t rendererFlags)
{
    m_contained = std::shared_ptr<SDL_Renderer>(SDL_CreateRenderer(window, index, rendererFlags), SDL_DestroyRenderer);
    m_window_parent = window;

    CheckNullError<SDL_Renderer, SDL_GetError>(m_contained.get(), "Could not initialize renderer");
}

void Renderer::load(SDL_Window *window, int index, uint32_t rendererFlags)
{
    m_contained = std::shared_ptr<SDL_Renderer>(SDL_CreateRenderer(window, index, rendererFlags), SDL_DestroyRenderer);
    m_window_parent = window;
    CheckNullError<SDL_Renderer, SDL_GetError>(m_contained.get(), "Could not initialize renderer");
}

void Renderer::load(Window &window, int index, uint32_t rendererFlags)
{
    m_contained = std::shared_ptr<SDL_Renderer>(SDL_CreateRenderer((SDL_Window *)window, index, rendererFlags), SDL_DestroyRenderer);
    m_window_parent = (SDL_Window *)window;
    CheckNullError<SDL_Renderer, SDL_GetError>(m_contained.get(), "Could not initialize renderer");
}

SDL_Texture *Renderer::createTextureFromSurface(SDL_Surface &surface)
{
    return SDL_CreateTextureFromSurface(m_contained.get(), &surface);
}

SDL_Texture *Renderer::createTextureFromSurface(SDL_Surface *surface)
{
    return SDL_CreateTextureFromSurface(m_contained.get(), surface);
}

bool Renderer::clear(void)
{
    return CheckError<SDL_GetError>(SDL_RenderClear(m_contained.get()), "Could not clear renderer");
}

bool Renderer::copyTexture(Texture &texture, SDL_Rect *src, SDL_Rect *dest)
{
    return CheckError<SDL_GetError>(SDL_RenderCopy(m_contained.get(), (SDL_Texture *)texture, src, dest), "Could not copy texture to renderer");
}

void Renderer::updateScreen() const
{
    SDL_RenderPresent(m_contained.get());
}

bool Renderer::setColor(int r, int g, int b, int a)
{
    return CheckError<SDL_GetError>(SDL_SetRenderDrawColor(m_contained.get(), r, g, b, a), "Could not set renderer color");
}

bool Renderer::setColor(asa::Colors color)
{
    uint8_t r = 0x0, g = 0x0, b = 0x0;

    switch (color)
    {
    case Colors::WHITE:
        r = 0xff;
        g = 0xff;
        b = 0xff;
        break;

    case Colors::RED:
        r = 0xff;
        break;

    case Colors::BLUE:
        b = 0xff;
        break;

    case Colors::GREEN:
        g = 0xff;
        break;

    case Colors::YELLOW:
        r = 0xff;
        g = 0xff;
        break;

    case Colors::CYAN:
        g = 0xff;
        b = 0xff;
        break;

    case Colors::BLACK:
        break;
    }

    return CheckError<SDL_GetError>(SDL_SetRenderDrawColor(m_contained.get(), r, g, b, 0xff), "Could not set renderer color");
}

bool Renderer::drawRect(const SDL_Rect &fillRect)
{
    return CheckError<SDL_GetError>(SDL_RenderDrawRect(m_contained.get(), &fillRect), "Could not draw rectangle");
}

bool Renderer::fillRect(const SDL_Rect &fillRect)
{
    return CheckError<SDL_GetError>(SDL_RenderFillRect(m_contained.get(), &fillRect), "Could not fill rectangle");
}

bool Renderer::drawLine(int x1, int y1, int x2, int y2)
{
    return CheckError<SDL_GetError>(SDL_RenderDrawLine(m_contained.get(), x1, y1, x2, y2), "Could not draw line");
}

bool Renderer::drawPoint(int x, int y)
{
    return CheckError<SDL_GetError>(SDL_RenderDrawPoint(m_contained.get(), x, y), "Could not draw point");
}

bool Renderer::setViewPort(SDL_Rect &rect)
{
    return CheckError<SDL_GetError>(SDL_RenderSetViewport(m_contained.get(), &rect), "Could not set view port");
}

bool Renderer::drawRects(const std::vector<SDL_Rect> &rects)
{
    return CheckError<SDL_GetError>(SDL_RenderFillRects(m_contained.get(), rects.data(), rects.size()), "Could not fill rectangle");
}

bool Renderer::fillRects(const std::vector<SDL_Rect> &fillRect)
{
    return CheckError<SDL_GetError>(SDL_RenderFillRects(m_contained.get(), fillRect.data(), fillRect.size()), "Could not fill rectangle");
}

Texture Renderer::loadPNG(const std::string &path, uint8_t r, uint8_t g, uint8_t b) const
{
    Texture texture = createTexture();

    Surface loadedSurface;
    loadedSurface.loadPNG(path);
    if (loadedSurface.isLoaded())
    {
        loadedSurface.setKeyColor(SDL_TRUE, SDL_MapRGB(loadedSurface.pixelFormat(), r, g, b));
        texture.load(loadedSurface);
    }

    return texture;
}

Texture Renderer::loadSolidText(const std::string &text, TTFFont &font, SDL_Color textColor) const
{
    Texture texture = createTexture();

    Surface surface;

    surface.load(TTF_RenderText_Solid((TTF_Font *)font, text.c_str(), textColor));

    if (surface.isLoaded())
    {
        texture.load(surface);
    }

    return texture;
}

Texture Renderer::loadBlendedText(const std::string &text, TTFFont &font, SDL_Color textColor) const
{
    Texture texture = createTexture();

    Surface surface;

    surface.load(TTF_RenderText_Blended((TTF_Font *)font, text.c_str(), textColor));

    if (surface.isLoaded())
    {
        texture.load(surface);
    }

    return texture;
}

Texture Renderer::createTexture() const
{
    return Texture(m_contained.get());
}

bool Renderer::drawCircle(int centerX, int centerY, int radius) {
    // midpoint algorithm
    const int diameter = (radius * 2);
    bool result = false;

    int x = (radius - 1);
    int y = 0;
    int tx = 1;
    int ty = 1;
    int error = (tx - diameter);

    while (x >= y)
    {
      //  Each of the following renders an octant of the circle
      drawPoint(centerX + x, centerY - y);
      drawPoint(centerX + x, centerY + y);
      drawPoint(centerX - x, centerY - y);
      drawPoint(centerX - x, centerY + y);

      drawPoint(centerX + y, centerY - x);
      drawPoint(centerX + y, centerY + x);
      drawPoint(centerX - y, centerY - x);
      drawPoint(centerX - y, centerY + x);

      if (error <= 0)
      {
        ++y;
        error += ty;
        ty += 2;
      }

      if (error > 0)
      {
        --x;
        tx += 2;
        error += (tx - diameter);
      }
   }
    return result;
}

bool Renderer::fillCircle(int centerX, int centerY, int radius) {
    const int diameter = (radius * 2);

    int x = (radius - 1);
    int y = 0;
    int tx = 1;
    int ty = 1;
    int error = (tx - diameter);

    while (x >= y)
    {
        // the hamburger

        // top bun
        drawLine(centerX - y, centerY - x, centerX + y, centerY - x);

        // cheese
        drawLine(centerX - x, centerY - y, centerX + x, centerY - y);

        // meat
        drawLine(centerX - x, centerY + y, centerX + x, centerY + y);

        // lower bun
        drawLine(centerX - y, centerY + x, centerX + y, centerY + x);

        if (error <= 0)
        {
        ++y;
        error += ty;
        ty += 2;
        }

        if (error > 0)
        {
        --x;
        tx += 2;
        error += (tx - diameter);
        }
    }

    return true;
}

std::string Renderer::getBasePath(void) {
    return SDL_GetBasePath();
}

SDL_BlendMode Renderer::drawBlendMode(void) {
    SDL_BlendMode result;
    int rc = SDL_GetRenderDrawBlendMode(m_contained.get(), &result);
    CheckError<SDL_GetError>(rc, "Could not get render draw blend mode");
    return result;
}

void Renderer::drawBlendMode(SDL_BlendMode mode) {
    int rc = SDL_SetRenderDrawBlendMode(m_contained.get(), mode);
    CheckError<SDL_GetError>(rc, "Could not set render draw blend mode");
}
