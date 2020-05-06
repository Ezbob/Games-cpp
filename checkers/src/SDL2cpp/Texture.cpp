#include "SDL_render.h"
#include "Texture.hpp"
#include <memory>

using namespace asa;

Texture::Texture(SDL_Renderer *renderer) : m_renderer(renderer) {}

void Texture::load(SDL_Texture *texture, int w, int h)
{
    m_contained = std::shared_ptr<SDL_Texture>(texture, SDL_DestroyTexture);
    height(h);
    width(w);
    if (m_contained == nullptr)
    {
        std::cerr << "Error: Could not load texture: " << SDL_GetError() << std::endl;
    }
}

void Texture::load(SDL_Texture *texture)
{
    m_contained = std::shared_ptr<SDL_Texture>(texture, SDL_DestroyTexture);

    if (m_contained == nullptr)
    {
        std::cerr << "Error: Could not load texture: " << SDL_GetError() << std::endl;
        return;
    }
    int w, h;
    SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
    height(h);
    width(w);
}

void Texture::load(SDL_Surface *surface)
{
    SDL_Texture *newtexture = SDL_CreateTextureFromSurface(m_renderer, surface);
    m_contained = std::shared_ptr<SDL_Texture>(newtexture, SDL_DestroyTexture);
    if (m_contained == nullptr)
    {
        std::cerr << "Error: Could not load texture: " << SDL_GetError() << std::endl;
        return;
    }
    height(surface->h);
    width(surface->w);
}

void Texture::load(Surface &surface)
{
    SDL_Texture *newtexture = SDL_CreateTextureFromSurface(m_renderer, (SDL_Surface *)surface);
    m_contained = std::shared_ptr<SDL_Texture>(newtexture, SDL_DestroyTexture);
    if (m_contained == nullptr)
    {
        std::cerr << "Error: Could not load texture: " << SDL_GetError() << std::endl;
        return;
    }
    height(surface.height());
    width(surface.width());
}

void Texture::load(asa::Texture &&texture)
{
    m_contained.swap(texture.m_contained);
    if (m_contained == nullptr)
    {
        std::cerr << "Error: Could not load texture: " << SDL_GetError() << std::endl;
        return;
    }
    height(texture.height());
    width(texture.width());
}

void Texture::render(const SDL_Rect &quad)
{
    CheckError<SDL_GetError>(SDL_RenderCopy(m_renderer, m_contained.get(), nullptr, &quad), "Cloud not render texture");
}

void Texture::render(const int x, const int y)
{
    SDL_Rect quad = {x, y, width(), height()};
    CheckError<SDL_GetError>(SDL_RenderCopy(m_renderer, m_contained.get(), nullptr, &quad), "Cloud not render texture");
}

void Texture::render(const int x, const int y, const SDL_Rect &clip)
{
    SDL_Rect quad = {x, y, clip.w, clip.h};
    CheckError<SDL_GetError>(SDL_RenderCopy(m_renderer, m_contained.get(), &clip, &quad), "Cloud not render clip texture");
}

void Texture::render(const int x, const int y, const SDL_Rect &clip, SDL_RendererFlip &flip)
{
    SDL_Rect quad = {x, y, clip.w, clip.h};
    CheckError<SDL_GetError>(SDL_RenderCopyEx(m_renderer, m_contained.get(), &clip, &quad, 0, nullptr, flip), "Cloud not render clip texture");
}

void Texture::render(const int x, const int y, SDL_RendererFlip &flip)
{
    SDL_Rect quad = {x, y, width(), height()};
    CheckError<SDL_GetError>(SDL_RenderCopyEx(m_renderer, m_contained.get(), nullptr, &quad, 0, nullptr, flip), "Cloud not render clip texture");
}

void Texture::blendMode(SDL_BlendMode mode)
{
    CheckError<SDL_GetError>(SDL_SetTextureBlendMode(m_contained.get(), mode), "Cloud not set texture blend mode");
}

SDL_BlendMode Texture::blendMode(void)
{
    SDL_BlendMode result;
    CheckError<SDL_GetError>(SDL_GetTextureBlendMode(m_contained.get(), &result), "Cloud not get texture blend mode");
    return result;
}

void Texture::alphaMod(uint8_t mod)
{
    CheckError<SDL_GetError>(SDL_SetTextureAlphaMod(m_contained.get(), mod), "Could not set texture alpha mod");
}

uint8_t Texture::alphaMod(void)
{
    uint8_t result;
    CheckError<SDL_GetError>(SDL_GetTextureAlphaMod(m_contained.get(), &result), "Could not set texture alpha mod");
    return result;
}