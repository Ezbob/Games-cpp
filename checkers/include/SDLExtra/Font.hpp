#ifndef HEADER_GUARD_9d0f70840337db3e8dfc9f59e0796fbd
#define HEADER_GUARD_9d0f70840337db3e8dfc9f59e0796fbd

#include "SDL2/SDL_ttf.h"
#include "SDL2cpp/MemoryContainer.hpp"
#include <string>

namespace sdl2 {

    class TTFFont : public ContainerBase<TTF_Font, TTF_CloseFont> {
    public:
        void loadTTF(const std::string filepath, int ptsize);
    };

}

#endif
