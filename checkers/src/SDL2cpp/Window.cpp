
#include "ErrorCheck.hpp"
#include "Window.hpp"
#include "Renderer.hpp"

using namespace asa;

Window::Window(const char *title, int x, int y, int width, int height, uint32_t flags)
: m_height(height), m_width(width) {
    m_contained = CheckNullError<SDL_Window, SDL_GetError>(SDL_CreateWindow(
                                                               title, x, y, width, height, flags),
                                                           "Window could not be initialized");
}

Window::Window(const char *title, int xy, int width, int height, uint32_t flags)
: m_height(height), m_width(width) {
    m_contained = CheckNullError<SDL_Window, SDL_GetError>(SDL_CreateWindow(
                                                               title, xy, xy, width, height, flags),
                                                           "Window could not be initialized");
}

Window::Window() {}

void Window::load(SDL_Window *wind)
{
    m_contained = CheckNullError<SDL_Window, SDL_GetError>(wind,
                                                           "Window could not be loaded");

    auto surf = getSurface();

    m_width = surf.getWidth();
    m_height = surf.getHeight();
}

Surface Window::getSurface() const
{
    return Surface(SDL_GetWindowSurface(m_contained));
}

bool Window::updateScreen() const
{
    return SDL_UpdateWindowSurface(m_contained) == 0;
}

void Window::loadWindow(std::string windowName, int x, int y, int width, int height, uint32_t flags)
{
    m_contained = CheckNullError<SDL_Window, SDL_GetError>(SDL_CreateWindow(windowName.c_str(), x, y, width, height, flags),
                                                           "Window could not be loaded");
    m_height = height;
    m_width = width;
}

Renderer Window::getRenderer(uint32_t rendererFlags, int index)
{
    return Renderer(m_contained, index, rendererFlags);
}

int Window::height(void) const
{
    return m_height;
}
int Window::width(void) const
{
    return m_width;
}