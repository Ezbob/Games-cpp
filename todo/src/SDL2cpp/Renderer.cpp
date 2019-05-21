#include "SDL2cpp/Renderer.hpp"
#include "SDL2cpp/Texture.hpp"
#include "SDL2cpp/ErrorCheck.hpp"
#include "SDL.h"
#include <memory>

using namespace sdl2;

Renderer::Renderer(SDL_Window *window, int index, uint32_t rendererFlags) {
    m_contained = std::shared_ptr<SDL_Renderer>(SDL_CreateRenderer(window, index, rendererFlags), SDL_DestroyRenderer);
    m_window_parent = window;

    CheckNullError<SDL_Renderer, SDL_GetError>(m_contained.get(), "Could not initialize renderer");
}

Renderer::Renderer() {}

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

Texture Renderer::createTexture() const {
    return Texture(m_contained.get());
}
