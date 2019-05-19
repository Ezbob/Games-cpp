#include "SDL2cpp/ErrorCheck.hpp"
#include "SDL2cpp/Window.hpp"
#include "SDL2cpp/Renderer.hpp"
#include "SDL.h"

SDLWindow::SDLWindow(const char *title, int x, int y, int width, int height, uint32_t flags) {
    m_contained = CheckNullError<SDL_Window, SDL_GetError>(SDL_CreateWindow(
        title, x, y, width, height, flags
    ), "Window could not be initialized");
}

SDLWindow::SDLWindow(const char *title, int xy, int width, int height, uint32_t flags) {
    m_contained = CheckNullError<SDL_Window, SDL_GetError>(SDL_CreateWindow(
        title, xy, xy, width, height, flags
    ), "Window could not be initialized");
}

SDLWindow::SDLWindow() {}

void SDLWindow::load(SDL_Window *wind) {
    m_contained = CheckNullError<SDL_Window, SDL_GetError>(wind,
        "Window could not be loaded");
}

SDLSurface SDLWindow::getSurface() const {
    return SDLSurface(SDL_GetWindowSurface(m_contained));
}

bool SDLWindow::updateScreen() const {
    return SDL_UpdateWindowSurface(m_contained) == 0;
}

void SDLWindow::loadWindow(std::string windowName, int x, int y, int width, int height, uint32_t flags) {
    m_contained = CheckNullError<SDL_Window, SDL_GetError>(SDL_CreateWindow(windowName.c_str(), x, y, width, height, flags),
        "Window could not be loaded");
}

SDLRenderer SDLWindow::getRenderer(int index, uint32_t rendererFlags) {
    return SDLRenderer(m_contained, index, rendererFlags);
}
