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
        Window(const char *title, int width, int height,
               int x = SDL_WINDOWPOS_UNDEFINED, 
               int y = SDL_WINDOWPOS_UNDEFINED, 
               uint32_t flags = SDL_WINDOW_SHOWN);

        Surface getSurface() const;
        bool updateScreen() const;

        Renderer getRenderer(uint32_t rendererFlags = SDL_RENDERER_ACCELERATED, int index = -1);
    };
} // namespace asa
