#ifndef HEADER_GUARD_9d0f70840337db3e8dfc9f59e0796fbd
#define HEADER_GUARD_9d0f70840337db3e8dfc9f59e0796fbd

#include "SDL_ttf.h"
#include "SDL2cpp/MemoryContainer.hpp"
#include <string>

class TTFFont : public ContainerBase<TTFFont, TTF_Font, TTF_CloseFont> {

public:
    void loadTTF(const std::string filepath, int ptsize);
};

#endif
