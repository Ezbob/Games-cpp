
#include "SDL2cpp/Globals.hpp"
#include <iostream>

bool SDLGlobals::init(uint32_t init_flags) {
    if ( SDL_Init( m_flags_set ) < 0 ) {
        std::cerr << "Error: SDL could not be initialized: " << SDL_GetError() << std::endl;
        is_initialized = false;
        return false;
    }
    m_flags_set = init_flags;
    is_initialized = true;
    return true;
}

bool SDLGlobals::loadExternLib(SDLExternLibs libsChosen, uint32_t libFlags) {

    switch (libsChosen) {
        case SDLExternLibs::SDL_IMAGE:
            if ( !( IMG_Init(libFlags) & libFlags ) ) { // IMG_Init returns the flags set
                std::cerr << "Error: Could not initialize SDL_Image: " << IMG_GetError() << std::endl;
                is_initialized = false;
            } else {
                is_initialized = is_initialized && true;
            }
            break;
        case SDLExternLibs::SDL_TTF:
            if ( TTF_Init() == -1 ) {
                std::cerr << "Error: Could not initialize SDL_TFF: " << TTF_GetError() << std::endl;
                is_initialized = false;
            } else {
                is_initialized = is_initialized && true;
            }
            break;
        
        default:
            break;
    }

    if (is_initialized)
        m_libs_set |= static_cast<int>(libsChosen);

    return is_initialized;
}