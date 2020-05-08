
#include "ErrorCheck.hpp"
#include "Window.hpp"
#include "Renderer.hpp"

using namespace asa;

Window::Window(const char *title, int w, int h, int x, int y, uint32_t flags)
{
    auto win = SDL_CreateWindow(title, x, y, w, h, flags);
    m_contained = CheckNullError<SDL_Window, SDL_GetError>(win, "Window could not be initialized");
    height(h);
    width(w);
}

Surface Window::getSurface() const
{
    return Surface(SDL_GetWindowSurface(m_contained));
}

bool Window::updateScreen() const
{
    return SDL_UpdateWindowSurface(m_contained) == 0;
}

Renderer Window::getRenderer(uint32_t rendererFlags, int index)
{
    return Renderer(m_contained, index, rendererFlags);
}
