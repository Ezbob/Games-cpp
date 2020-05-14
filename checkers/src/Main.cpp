#include <iostream>
#include "ErrorCheck.hpp"
#include "SmartPointers.hpp"
#include "Shortcuts.hpp"

#include <memory>
#include "SDL.h"

#include "gametool/GameClock.hpp"
#include "gametool/GameStateProcessor.hpp"
#include "gametool/GameState.hpp"

#include "states/PauseState.hpp"
#include "states/WinState.hpp"

/*
#include "states/BoardPlayState.hpp"
*/

#if defined(_WIN32)
#define MAIN_NAME WinMain
#else
#define MAIN_NAME main
#endif

const int SCREEN_WIDTH = 840;
const int SCREEN_HEIGHT = 860;

int MAIN_NAME()
{
    asa::ThrowOnNonZero<SDL_GetError>(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER), "SDL could not be initialized");
    asa::LibDeleter<SDL_Quit> sdl_quit;

    asa::ThrowOnNonZero<SDL_GetError>(TTF_Init(), "Could not initialize TTF library");
    asa::LibDeleter<TTF_Quit> ttf_quit;

    asa::ThrowOnCondition<SDL_GetError>((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG, "Could not initialize SDL image");
    asa::LibDeleter<IMG_Quit> image_quit;

    auto window = asa::make_sdl_shared(
        SDL_CreateWindow("Checkers", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN));

    auto renderer = asa::make_sdl_shared(
        SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED));

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    asa::GameClock clock(60);
    asa::GameStateProcessor processor;
    asa::SingleThreadedMessageQueue message_queue;

    auto font = asa::make_sdl_shared(TTF_OpenFont((asa::getBasePath() + "/assets/consola.ttf").c_str(), 24));

    processor.addState(std::make_shared<WinState>(renderer, processor, font, window, message_queue));
    processor.addState(std::make_shared<PauseState>(renderer, processor, font, window));

    processor.processStates(clock);

    return 0;
}
