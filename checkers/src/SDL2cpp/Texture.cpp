#include "Texture.hpp"
#include <memory>
#include "ErrorCheck.hpp"

using namespace asa;

Texture::Texture(SDL_Texture *ptr)
{
    m_contained = std::shared_ptr<SDL_Texture>(ThrowOnNullError<SDL_GetError>(ptr, "Cannot construct texture from a null pointer"), SDL_DestroyTexture);
    if (ptr)
    {
        query();
    }
}

Texture::Texture(SDL_Texture *ptr, uint32_t f, int a, int w, int h) {
    m_contained = std::shared_ptr<SDL_Texture>(ThrowOnNullError<SDL_GetError>(ptr, "Cannot construct texture from a null pointer"), SDL_DestroyTexture);
    height(h);
    width(w);
    m_format = f;
    m_access = a;
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

void Texture::query(void)
{
    int w = 0, h = 0;
    SDL_QueryTexture(m_contained.get(), &m_format, &m_access, &w, &h);
    height(h);
    width(w);
}