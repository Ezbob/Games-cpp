#include <stdlib.h>
#include <stdio.h>
#include "sdl2/SDL.h"

#if defined(_WIN32)
    #define MAIN_NAME WinMain
#else
    #define MAIN_NAME main
#endif

#define UNUSED(x) ((void) x)
#define RC_OPAQUE_RED   0xff,0x00,0x00,0xff
#define RC_OPAQUE_GREEN 0x00,0xff,0x00,0xff
#define RC_OPAQUE_BLUE  0x00,0x00,0xff,0xff
#define RC_OPAQUE_WHITE 0xff,0xff,0xff,0xff
#define RC_OPAQUE_BLACK 0x00,0x00,0x00,0xff
#define BOARD_LENGTH 8

static const int SCREEN_WIDTH = 800;
static const int SCREEN_HEIGHT = 600;

SDL_Rect g_board[BOARD_LENGTH * BOARD_LENGTH];
SDL_Window *g_window = NULL;
SDL_Renderer *g_renderer = NULL;
SDL_bool g_is_playing = SDL_TRUE;

SDL_bool sdlInit() {
    if ( SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 
                                     SDL_WINDOW_SHOWN, &g_window, &g_renderer) == -1 )
        goto error_label;

    if ( SDL_SetRenderDrawColor(g_renderer, RC_OPAQUE_WHITE) == -1 )
        goto error_label;

    if ( SDL_RenderClear(g_renderer) == -1 )
        goto error_label;

    
    SDL_SetWindowTitle(g_window, "Pure C thing");

    return SDL_TRUE;

error_label:
    perror(SDL_GetError());
    return SDL_FALSE;
}

void sdlDestroy() {
    SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow(g_window);
}

void handleEvent(const SDL_Event *event) {
    switch(event->type) {
        case SDL_QUIT:
            g_is_playing = SDL_FALSE;
            break;
        default:
            break;
    }
}

void handleKeyState(const Uint8 *states) {
    UNUSED(states);
}

void pumpEvents() {
    static SDL_Event inputEvent;
    while ( SDL_PollEvent(&inputEvent) != 0 ) {
        handleEvent(&inputEvent);
    }
    handleKeyState(SDL_GetKeyboardState(NULL));
}



int MAIN_NAME(int argc, char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);

    if ( !sdlInit() ) {
        return EXIT_FAILURE;
    }

    while (g_is_playing) {
        pumpEvents();
    }

    sdlDestroy();
    return EXIT_SUCCESS;
}
