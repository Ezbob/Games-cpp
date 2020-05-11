#include <iostream>
#include "ErrorCheck.hpp"
#include "Deleter.hpp"
#include "Creators.hpp"

#include <memory>
#include "SDL.h"

#include "gametool/GameClock.hpp"
#include "gametool/GameStateProcessor.hpp"
#include "gametool/GameState.hpp"
/*
#include "states/PauseState.hpp"
#include "states/WinState.hpp"
#include "states/BoardPlayState.hpp"
*/

#if defined(_WIN32)
#define MAIN_NAME WinMain
#else
#define MAIN_NAME main
#endif

const int SCREEN_WIDTH = 840;
const int SCREEN_HEIGHT = 860;

/*
asa::Globals globals;
asa::Window window;
asa::Renderer renderer;
asa::TTFFont font;

asa::GameStateProcessor gameStateProcessor(60);

bool sdlInit()
{
    if (globals.init(SDL_INIT_VIDEO | SDL_INIT_TIMER))
    {
        globals.loadExternLib(asa::ExternLibs::SDL_IMAGE, IMG_INIT_PNG);
        globals.loadExternLib(asa::ExternLibs::SDL_TTF);

        window.loadWindow(
            "Checkers game",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
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
    /*
    SDL_CreateWindow("Checkers", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN),
            "Could not initialize the window"
    */
    asa::ThrowOnNonZero<SDL_GetError>(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER), "SDL could not be initialized");
    asa::LibDeleter<SDL_Quit> sdl_quit;

    asa::ThrowOnNonZero<SDL_GetError>(TTF_Init(), "Could not initialize TTF library");
    asa::LibDeleter<TTF_Quit> ttf_quit;


    auto window = asa::createShared(
        SDL_CreateWindow("Checkers", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN));

    auto renderer = asa::createShared(
        SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED));

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    asa::GameClock clock(60);
    asa::GameStateProcessor processor;

    auto font = asa::createShared(TTF_OpenFont((std::string(SDL_GetBasePath()) + "assets/consola.ttf").c_str(), 24));


    /*
    if (!sdlInit() || !loadGlobalAssets())
    {
        return 1;
    }

    int has_green_won = 0;

    gameStateProcessor.initStates([&has_green_won](auto &stack, auto &comm) {
        stack.emplace(new WinState(renderer, gameStateProcessor, font, window, comm));
        stack.emplace(new BoardPlayState(renderer, gameStateProcessor, font, window, comm));
    });

    gameStateProcessor.processStates();

    */

    return 0;
}
