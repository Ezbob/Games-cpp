#include <iostream>

#include "sdl2cpp/Renderer.hpp"
#include "sdl2cpp/Window.hpp"
#include "sdl2cpp/Font.hpp"
#include "sdl2cpp/libraries/Libraries.hpp"

#include "states/PauseState.hpp"
#include "states/WinState.hpp"
#include "states/BoardPlayState.hpp"

#if defined(_WIN32)
#define MAIN_NAME WinMain
#else
#define MAIN_NAME main
#endif

const int SCREEN_WIDTH = 840;
const int SCREEN_HEIGHT = 860;

/*
bool sdlInit()
{
    if (globals.init(SDL_INIT_VIDEO | SDL_INIT_TIMER))
    {
        globals.loadExternLib(asa::ExternLibs::SDL_IMAGE, IMG_INIT_PNG);
        globals.loadExternLib(asa::ExternLibs::SDL_TTF);

        window.loadWindow(
            "Checkers game",
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOW_SHOWN);

        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
        renderer = window.getRenderer();
        renderer.setColor(asa::Colors::WHITE);
    }

    return globals.is_initialized && window.isLoaded();
}

bool loadGlobalAssets()
{
    font.loadTTF(renderer.getBasePath() + "/assets/consola.ttf", 24);
    return font.isLoaded();
}
*/


int MAIN_NAME()
{
    asa::SDLLibrary sdl;
    asa::ImageLibrary image(IMG_INIT_PNG);
    asa::TTFLibrary ttf;

    asa::GameClock clock(60);
    asa::Window window("Checkers game", SCREEN_WIDTH, SCREEN_HEIGHT);
    asa::Renderer renderer = window.getRenderer();

    asa::SingleThreadedMessageQueue communications;

    asa::GameStateProcessor gameStateProcessor;

    asa::TTFFont font(renderer.getBasePath() + "/assets/consola.ttf", 24);

    gameStateProcessor.addState(std::make_shared<WinState>(renderer, gameStateProcessor, font, window, communications));

    gameStateProcessor.processStates(clock);
    return 0;
}
