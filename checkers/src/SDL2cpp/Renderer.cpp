#include "Renderer.hpp"
#include "Texture.hpp"
#include "ErrorCheck.hpp"
#include "SDL2/SDL.h"
#include <memory>

using namespace asa;

Renderer::Renderer(SDL_Window *window, int index, uint32_t rendererFlags) {
    m_contained = std::shared_ptr<SDL_Renderer>(SDL_CreateRenderer(window, index, rendererFlags), SDL_DestroyRenderer);
    m_window_parent = window;

    CheckNullError<SDL_Renderer, SDL_GetError>(m_contained.get(), "Could not initialize renderer");
}

void Renderer::load(SDL_Window *window, int index, uint32_t rendererFlags) {
    m_contained = std::shared_ptr<SDL_Renderer>(SDL_CreateRenderer(window, index, rendererFlags), SDL_DestroyRenderer);
    m_window_parent = window;
    CheckNullError<SDL_Renderer, SDL_GetError>(m_contained.get(), "Could not initialize renderer");
}

void Renderer::load(Window &window, int index, uint32_t rendererFlags) {
    m_contained = std::shared_ptr<SDL_Renderer>(SDL_CreateRenderer((SDL_Window *) window, index, rendererFlags), SDL_DestroyRenderer);
    m_window_parent = (SDL_Window *) window;
    CheckNullError<SDL_Renderer, SDL_GetError>(m_contained.get(), "Could not initialize renderer");
}

SDL_Texture *Renderer::createTextureFromSurface(SDL_Surface &surface) {
    return SDL_CreateTextureFromSurface(m_contained.get(), &surface);
}

SDL_Texture *Renderer::createTextureFromSurface(SDL_Surface *surface) {
    return SDL_CreateTextureFromSurface(m_contained.get(), surface);
}

bool Renderer::clear() {
    return CheckError<SDL_GetError>(SDL_RenderClear(m_contained.get()), "Could not clear renderer");
}

bool Renderer::copyTexture(Texture &texture, SDL_Rect *src, SDL_Rect *dest) {
    return CheckError<SDL_GetError>(SDL_RenderCopy(m_contained.get(), (SDL_Texture *) texture, src, dest), "Could not copy texture to renderer");
}

void Renderer::updateScreen() const {
    SDL_RenderPresent(m_contained.get());
}

bool Renderer::setColor(int r, int g, int b, int a) {
    return CheckError<SDL_GetError>(SDL_SetRenderDrawColor(m_contained.get(), r, g, b, a), "Could not set renderer color");
}

bool Renderer::setColor(asa::Colors color) {
    uint8_t r = 0x0, g = 0x0, b = 0x0;

    switch (color) {
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

bool Renderer::drawRect(const SDL_Rect *fillRect) {
    return CheckError<SDL_GetError>(SDL_RenderDrawRect(m_contained.get(), fillRect), "Could not draw rectangle");
}

bool Renderer::drawRect(const SDL_Rect &fillRect) {
    return CheckError<SDL_GetError>(SDL_RenderDrawRect(m_contained.get(), &fillRect), "Could not draw rectangle");
}

bool Renderer::fillRect(const SDL_Rect *fillRect) {
    return CheckError<SDL_GetError>(SDL_RenderFillRect(m_contained.get(), fillRect), "Could not fill rectangle");
}

bool Renderer::fillRect(const SDL_Rect &fillRect) {
    return CheckError<SDL_GetError>(SDL_RenderFillRect(m_contained.get(), &fillRect), "Could not fill rectangle");
}

bool Renderer::drawLine(int x1, int y1, int x2, int y2) {
    return CheckError<SDL_GetError>(SDL_RenderDrawLine(m_contained.get(), x1, y1, x2, y2), "Could not draw line");
}

bool Renderer::drawPoint(int x, int y) {
    return CheckError<SDL_GetError>(SDL_RenderDrawPoint(m_contained.get(), x, y), "Could not draw point");
}

bool Renderer::setViewPort(SDL_Rect &rect) {
    return CheckError<SDL_GetError>(SDL_RenderSetViewport(m_contained.get(), &rect), "Could not set view port");
}

bool Renderer::drawRects(const std::vector<SDL_Rect> &rects) {
    return CheckError<SDL_GetError>(SDL_RenderFillRects(m_contained.get(), rects.data(), rects.size()), "Could not fill rectangle");
}

bool Renderer::fillRects(const std::vector<SDL_Rect> &fillRect) {
    return CheckError<SDL_GetError>(SDL_RenderFillRects(m_contained.get(), fillRect.data(), fillRect.size()), "Could not fill rectangle");
}

Texture Renderer::createTexture() const {
    return Texture(m_contained.get());
}
