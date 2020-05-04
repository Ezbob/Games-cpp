#pragma once

#include "SDL_video.h"
#include "Surface.hpp"
#include "MemoryContainer.hpp"
#include <cstdint>
#include <string>

namespace asa
{

class Renderer;

class Window : public ContainerBase<SDL_Window, SDL_DestroyWindow>
{
private:
    int m_height = 0;
    int m_width = 0;

public:
    Window(const char *title, int x, int y, int width, int height, uint32_t flags = SDL_WINDOW_SHOWN);
    Window(const char *title, int xy, int width, int height, uint32_t flags = SDL_WINDOW_SHOWN);
    Window(void);

    void loadWindow(std::string windowName, int x, int y, int width, int height, uint32_t flags);
    void load(SDL_Window *wind);
    Surface getSurface() const;
    bool updateScreen() const;

    int height(void) const;
    int width(void) const;

    Renderer getRenderer(uint32_t rendererFlags = 0, int index = -1);
};
} // namespace asa
