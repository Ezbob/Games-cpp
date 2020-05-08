#pragma once

#include "Surface.hpp"
#include "container_bases/SharedContainerBase.hpp"
#include "Sized2DEntity.hpp"

namespace asa
{

    class Texture : public SharedContainerBase<SDL_Texture, SDL_DestroyTexture>, public Sized2DEntity<Texture>
    {
    private:
        int m_access;
        uint32_t m_format;
    public:
        Texture(SDL_Texture *);
        Texture(SDL_Texture *, uint32_t format, int access, int w, int h);

        void blendMode(SDL_BlendMode mode);
        SDL_BlendMode blendMode(void);

        void alphaMod(uint8_t mod);
        uint8_t alphaMod(void);

        int access(void);
        uint32_t format(void);

        void query(void);
    };
} // namespace asa
