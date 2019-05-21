#include "SDL.h"
#include "SDL2cpp/Texture.hpp"
#include <memory>

using namespace sdl2;

Texture::Texture(Renderer &renderer) : m_renderer((SDL_Renderer *) renderer), m_width(0), m_height(0) {}
Texture::Texture(SDL_Renderer *renderer) : m_renderer(renderer), m_width(0), m_height(0) {}

void Texture::load(SDL_Texture *texture, int width, int height) {
    m_contained = std::shared_ptr<SDL_Texture>(texture, SDL_DestroyTexture);
    m_height = height;
    m_width = width;
    if (m_contained == nullptr) {
        std::cerr << "Error: Could not load texture: " << SDL_GetError() << std::endl;
    }
}

void Texture::load(SDL_Texture *texture) {
    m_contained = std::shared_ptr<SDL_Texture>(texture, SDL_DestroyTexture);

    if (m_contained == nullptr) {
        std::cerr << "Error: Could not load texture: " << SDL_GetError() << std::endl;
        return;
    }
    SDL_QueryTexture(texture, nullptr, nullptr, &m_width, &m_height);
}

void Texture::load(SDL_Surface *surface) {
    SDL_Texture *newtexture = SDL_CreateTextureFromSurface(m_renderer, surface);
    m_contained = std::shared_ptr<SDL_Texture>(newtexture, SDL_DestroyTexture);
    if (m_contained == nullptr) {
        std::cerr << "Error: Could not load texture: " << SDL_GetError() << std::endl;
        return;
    }
    m_height = surface->h;
    m_width = surface->w;
}

void Texture::load(Surface &surface) {
    SDL_Texture *newtexture = SDL_CreateTextureFromSurface(m_renderer, (SDL_Surface *) surface);
    m_contained = std::shared_ptr<SDL_Texture>(newtexture, SDL_DestroyTexture);
    if (m_contained == nullptr) {
        std::cerr << "Error: Could not load texture: " << SDL_GetError() << std::endl;
        return;
    }
    m_height = surface.getHeight();
    m_width = surface.getWidth();
}

void Texture::render(const int x, const int y) {
    SDL_Rect quad = {x, y, m_width, m_height};
    CheckError<SDL_GetError>(SDL_RenderCopy(m_renderer, m_contained.get(), nullptr, &quad), "Cloud not render texture");
}

void Texture::render(const int x, const int y, const SDL_Rect &clip) {
    SDL_Rect quad = {x, y, clip.w, clip.h};
    CheckError<SDL_GetError>(SDL_RenderCopy(m_renderer, m_contained.get(), &clip, &quad), "Cloud not render clip texture");
}

void Texture::render(const int x, const int y, const SDL_Rect &clip, SDL_RendererFlip &flip) {
    SDL_Rect quad = {x, y, clip.w, clip.h};
    CheckError<SDL_GetError>(SDL_RenderCopyEx(m_renderer, m_contained.get(), &clip, &quad, 0, nullptr, flip), "Cloud not render clip texture");
}

void Texture::render(const int x, const int y, SDL_RendererFlip &flip) {
    SDL_Rect quad = {x, y, m_width, m_height};
    CheckError<SDL_GetError>(SDL_RenderCopyEx(m_renderer, m_contained.get(), nullptr, &quad, 0, nullptr, flip), "Cloud not render clip texture");
}