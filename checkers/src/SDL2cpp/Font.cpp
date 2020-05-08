#include "Font.hpp"
#include "sdl2cpp/ErrorCheck.hpp"
#include "SDL_ttf.h"

using namespace asa;

TTFFont::TTFFont(const std::string filepath, int ptsize) : m_ptsize(ptsize) {
    TTF_Font *rc = TTF_OpenFont(filepath.c_str(), ptsize);
    m_contained = CheckNullError<TTF_Font, TTF_GetError>(rc, "Could not initialize font");
}
