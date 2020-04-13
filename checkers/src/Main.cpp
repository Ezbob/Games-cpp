#include "SDL.hpp"
#include "SDL_extra.hpp"
#include "game_tool.hpp"
#include <iostream>
#include <stack>
#include <vector>
#include <array>
#include <optional>
#include "PauseState.hpp"
#include "WinState.hpp"
#include "BoardPlayState.hpp"

#if defined(_WIN32)
#define MAIN_NAME WinMain
#else
#define MAIN_NAME main
#endif

const int SCREEN_WIDTH = 840;
const int SCREEN_HEIGHT = 860;

sdl2::Globals globals;
sdl2::Window window;
sdl2::Renderer renderer;
sdl2::TTFFont font;

gtool::GameStateProcessor gameStateProcessor(16.);

bool sdlInit()
{
    if (globals.init(SDL_INIT_VIDEO | SDL_INIT_TIMER))
    {
        globals.loadExternLib(sdl2::ExternLibs::SDL_IMAGE, IMG_INIT_PNG);
        globals.loadExternLib(sdl2::ExternLibs::SDL_TTF);

        window.loadWindow(
            "Checkers game",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN);

        renderer = window.getRenderer();
        renderer.setColor(sdl2::Colors::WHITE);
    }

    return globals.is_initialized && window.isLoaded();
}

bool loadGlobalAssets()
{
    font.loadTTF("assets/consola.ttf", 24);
    return font.isLoaded();
}

int MAIN_NAME()
{

    if (!sdlInit())
    {
        return 1;
    }

    if (!loadGlobalAssets())
    {
        return 1;
    }

    gameStateProcessor.initStates([](auto &stack) {
        stack.emplace(new WinState(renderer, gameStateProcessor, font, SCREEN_WIDTH, SCREEN_HEIGHT));
        stack.emplace(new BoardPlayState(renderer, gameStateProcessor, font, SCREEN_WIDTH, SCREEN_HEIGHT));
    });

    gameStateProcessor.processStates();

    return 0;
}
