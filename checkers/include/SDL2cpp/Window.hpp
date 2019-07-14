#ifndef HEADER_GUARD_08570ec71d9036246ac3d05623b1f7bb
#define HEADER_GUARD_08570ec71d9036246ac3d05623b1f7bb

#include "ErrorCheck.hpp"
#include "SDL2/SDL.h"
#include "Surface.hpp"
#include "SDL2cpp/MemoryContainer.hpp"

namespace sdl2 {

    class Renderer;

    class Window : public ContainerBase<SDL_Window, SDL_DestroyWindow> {

    public:
        Window(const char *title, int x, int y, int width, int height, uint32_t flags = SDL_WINDOW_SHOWN);
        Window(const char *title, int xy, int width, int height, uint32_t flags = SDL_WINDOW_SHOWN);
        Window();

        void loadWindow(std::string windowName, int x, int y, int width, int height, uint32_t flags);
        void load(SDL_Window *wind);
        Surface getSurface() const;
        bool updateScreen() const;

        Renderer getRenderer(uint32_t rendererFlags = 0, int index = -1);

    };
}

#endif
