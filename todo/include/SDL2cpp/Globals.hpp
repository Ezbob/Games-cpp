#ifndef HEADER_GUARD_7f5035ba17841c33d97ad3913dd17572
#define HEADER_GUARD_7f5035ba17841c33d97ad3913dd17572

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <cstddef>

enum class SDLExternLibs {
    SDL_IMAGE = 1,
    SDL_TTF
};

struct SDLGlobals {

    bool init(uint32_t init_flags);
    bool loadExternLib(SDLExternLibs libsChosen, uint32_t libFlags = 0);

    ~SDLGlobals() {
        SDL_Quit();
        uint32_t imageFeature = static_cast<uint32_t>(SDLExternLibs::SDL_IMAGE);
        if ( (m_libs_set & imageFeature) == imageFeature ) {
            IMG_Quit();
        }

        uint32_t fontFeature = static_cast<uint32_t>(SDLExternLibs::SDL_TTF);
        if ( (m_libs_set & fontFeature) == fontFeature ) {
            TTF_Quit();
        }
    }

    uint32_t m_libs_set = 0;
    uint32_t m_flags_set = 0;
    bool is_initialized = false;
    bool is_playing = true;
};


#endif
