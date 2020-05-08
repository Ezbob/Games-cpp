#pragma once

#include "SDL_video.h"
#include "Surface.hpp"
#include "container_bases/ContainerBase.hpp"
#include "Sized2DEntity.hpp"
#include <cstdint>
#include <string>

namespace asa
{

    class Renderer;

    class Window : public ContainerBase<SDL_Window, SDL_DestroyWindow>, public Sized2DEntity<Window>
    {
    public:
        Window(const char *title, int x, int y, int width, int height, uint32_t flags = SDL_WINDOW_SHOWN);
        Window(const char *title, int xy, int width, int height, uint32_t flags = SDL_WINDOW_SHOWN);
        Window(void);

        void loadWindow(std::string windowName, int x, int y, int width, int height, uint32_t flags);
        void load(SDL_Window *wind);
        Surface getSurface() const;
        bool updateScreen() const;

        Renderer getRenderer(uint32_t rendererFlags = 0, int index = -1);
    };
} // namespace asa
