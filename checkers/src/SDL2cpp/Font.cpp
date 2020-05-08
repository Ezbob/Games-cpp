#include "Font.hpp"
#include "sdl2cpp/ErrorCheck.hpp"
#include "SDL_ttf.h"

using namespace asa;

TTFFont::TTFFont(const std::string filepath, int ptsize) : m_ptsize(ptsize) {
    m_contained = ThrowOnNullError<TTF_GetError>(TTF_OpenFont(filepath.c_str(), ptsize), "Could not initialize font at" + filepath);
}
