#pragma once

#include "SDL_ttf.h"
#include "container_bases/ContainerBase.hpp"
#include <string>

namespace asa
{

    class TTFFont : public ContainerBase<TTF_Font, TTF_CloseFont>
    {
    public:
        void loadTTF(const std::string filepath, int ptsize);
    };

} // namespace asa
