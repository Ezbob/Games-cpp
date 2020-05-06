
#include "ErrorCheck.hpp"
#include "Window.hpp"
#include "Renderer.hpp"

using namespace asa;

Window::Window(const char *title, int x, int y, int w, int h, uint32_t flags)
{
    auto win = SDL_CreateWindow(title, x, y, w, h, flags);
    m_contained = CheckNullError<SDL_Window, SDL_GetError>(win, "Window could not be initialized");
    height(h);
    width(w);
}

Window::Window(const char *title, int xy, int w, int h, uint32_t flags)
{
    auto win = SDL_CreateWindow(title, xy, xy, w, h, flags);
    m_contained = CheckNullError<SDL_Window, SDL_GetError>(win, "Window could not be initialized");
    height(h);
    width(w);
}

Window::Window() {}

void Window::load(SDL_Window *wind)
{
    m_contained = CheckNullError<SDL_Window, SDL_GetError>(wind, "Window could not be loaded");

    auto surf = getSurface();

    width(surf.width());
    height(surf.height());
}

Surface Window::getSurface() const
{
    return Surface(SDL_GetWindowSurface(m_contained));
}

bool Window::updateScreen() const
{
    return SDL_UpdateWindowSurface(m_contained) == 0;
}

void Window::loadWindow(std::string windowName, int x, int y, int w, int h, uint32_t flags)
{
    auto win = SDL_CreateWindow(windowName.c_str(), x, y, w, h, flags);
    m_contained = CheckNullError<SDL_Window, SDL_GetError>(win, "Window could not be loaded");
    height(h);
    width(w);
}

Renderer Window::getRenderer(uint32_t rendererFlags, int index)
{
    return Renderer(m_contained, index, rendererFlags);
}
