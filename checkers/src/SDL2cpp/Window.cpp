#include "SDL2cpp/ErrorCheck.hpp"
#include "SDL2cpp/Window.hpp"
#include "SDL2cpp/Renderer.hpp"
#include "SDL2/SDL.h"

using namespace sdl2;

Window::Window(const char *title, int x, int y, int width, int height, uint32_t flags) {
    m_contained = CheckNullError<SDL_Window, SDL_GetError>(SDL_CreateWindow(
        title, x, y, width, height, flags
    ), "Window could not be initialized");
}

Window::Window(const char *title, int xy, int width, int height, uint32_t flags) {
    m_contained = CheckNullError<SDL_Window, SDL_GetError>(SDL_CreateWindow(
        title, xy, xy, width, height, flags
    ), "Window could not be initialized");
}

Window::Window() {}

void Window::load(SDL_Window *wind) {
    m_contained = CheckNullError<SDL_Window, SDL_GetError>(wind,
        "Window could not be loaded");
}

Surface Window::getSurface() const {
    return Surface(SDL_GetWindowSurface(m_contained));
}

bool Window::updateScreen() const {
    return SDL_UpdateWindowSurface(m_contained) == 0;
}

void Window::loadWindow(std::string windowName, int x, int y, int width, int height, uint32_t flags) {
    m_contained = CheckNullError<SDL_Window, SDL_GetError>(SDL_CreateWindow(windowName.c_str(), x, y, width, height, flags),
        "Window could not be loaded");
}

Renderer Window::getRenderer(uint32_t rendererFlags, int index) {
    return Renderer(m_contained, index, rendererFlags);
}
