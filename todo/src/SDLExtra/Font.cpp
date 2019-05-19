#include "SDLExtra/Font.hpp"
#include "SDL2cpp/ErrorCheck.hpp"
#include "SDL_ttf.h"


void TTFFont::loadTTF(const std::string filepath, int ptsize) {
    m_contained = CheckNullError<TTF_Font, TTF_GetError>(TTF_OpenFont(filepath.c_str(), ptsize), "Could not initialze font");
}
