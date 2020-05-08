#pragma once

#include "SDL_ttf.h"
#include "container_bases/ContainerBase.hpp"
#include <string>

namespace asa
{

    class TTFFont : public ContainerBase<TTF_Font, TTF_CloseFont>
    {
    private:
        int m_ptsize;
        std::string m_path;
    public:
        TTFFont(const std::string filepath, int ptsize);

        inline int point_size(void) const {
            return m_ptsize;
        }

        inline const std::string path(void) const {
            return m_path;
        }
    };

} // namespace asa
