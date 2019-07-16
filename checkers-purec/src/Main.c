#include <stdlib.h>
#include <stdio.h>
#include "sdl2/SDL.h"
#include "Gameclock.h"

#if defined(_WIN32)
    #define MAIN_NAME WinMain
#else
    #define MAIN_NAME main
#endif

#define UNUSED(x) ((void) x)

// PC = Parameter Color
#define PC_OPAQUE_RED   0xff,0x00,0x00,0xff
#define PC_OPAQUE_GREEN 0x00,0xff,0x00,0xff
#define PC_OPAQUE_BLUE  0x00,0x00,0xff,0xff
#define PC_OPAQUE_WHITE 0xff,0xff,0xff,0xff
#define PC_OPAQUE_BLACK 0x00,0x00,0x00,0xff

#define BOARD_LENGTH 8

static const double MS_PER_UPDATE = 16.0;
static const int SCREEN_WIDTH = 800;
static const int SCREEN_HEIGHT = 600;

struct Checker {
    enum {
        C_RED,
        C_GREEN
    } color;
    SDL_Rect *rect; // the actual rendered rect
    SDL_Rect next; // the next point in the lerp
};

struct Cell {
    SDL_Rect *container;
    struct Checker *occubant;
};

static const char *g_window_title = "Pure C Checkers";
SDL_Window *g_window = NULL;
SDL_Renderer *g_renderer = NULL;
SDL_bool g_is_playing = SDL_TRUE;

SDL_Rect g_board[BOARD_LENGTH * BOARD_LENGTH];
SDL_Rect g_checkers[BOARD_LENGTH * BOARD_LENGTH];

struct Cell g_cellboard[BOARD_LENGTH * BOARD_LENGTH];

struct GameClock g_gameclock;


SDL_bool sdlInit() {
    if ( SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT,
                                     SDL_WINDOW_SHOWN, &g_window, &g_renderer) == -1 )
        goto error_label;

    if ( SDL_SetRenderDrawColor(g_renderer, PC_OPAQUE_WHITE) == -1 )
        goto error_label;

    if ( SDL_RenderClear(g_renderer) == -1 )
        goto error_label;

    SDL_SetWindowTitle(g_window, g_window_title);

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

void initCells(struct Cell *cs, int length) {
    for (int i = 0; i < length; ++i) {
        cs->container = NULL;
        cs->occubant = NULL;
    }
}

void initData() {
    initCells(g_cellboard, BOARD_LENGTH * BOARD_LENGTH);
    GT_gameclock_init(&g_gameclock, MS_PER_UPDATE);
}

SDL_bool load() {

    return SDL_TRUE;
}

void update() {

}

void render() {

}


int MAIN_NAME(int argc, char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);

    if ( !sdlInit() ) {
        return EXIT_FAILURE;
    }

    if ( !load() ) {
        return EXIT_FAILURE;
    }

    while ( g_is_playing ) {
        pumpEvents();

        while ( g_gameclock.updateLag >= g_gameclock.msPerUpdate ) {
            update();
            g_gameclock.updateLag -= g_gameclock.msPerUpdate;
        }

        render();

        GT_gameclock_tick(&g_gameclock);
    }

    sdlDestroy();
    return EXIT_SUCCESS;
}
