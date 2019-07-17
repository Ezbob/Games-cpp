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
//           name        r    g    b    a
#define PC_OPAQUE_RED   0xff,0x00,0x00,0xff
#define PC_OPAQUE_GREEN 0x00,0xff,0x00,0xff
#define PC_OPAQUE_BLUE  0x00,0x00,0xff,0xff
#define PC_OPAQUE_WHITE 0xff,0xff,0xff,0xff
#define PC_OPAQUE_BLACK 0x00,0x00,0x00,0xff

#define BOARD_LENGTH 8

static const double MS_PER_UPDATE = 16.0;
static const int SCREEN_WIDTH = 840;
static const int SCREEN_HEIGHT = 860;

static const int checkerLength = 60;

enum PlayingColor {
    GREEN,
    RED
};

struct Checker {
    enum PlayingColor color;
    SDL_Rect *rect; // the actual rendered rect
    SDL_Rect next; // the next point in the lerp
};

struct Cell {
    SDL_Rect *container;
    struct Checker *occubant;
    int columnIndex;
    int rowIndex;
};

static const char *g_window_title = "Pure C Checkers";
SDL_Window *g_window = NULL;
SDL_Renderer *g_renderer = NULL;
SDL_bool g_is_playing = SDL_TRUE;

SDL_Rect g_board[BOARD_LENGTH * BOARD_LENGTH];
SDL_Rect g_checker_rects[BOARD_LENGTH * BOARD_LENGTH];


int g_green_length = 0;
int g_red_length = 0;

struct Checker g_checkers[BOARD_LENGTH * BOARD_LENGTH];
struct Cell g_cellboard[BOARD_LENGTH * BOARD_LENGTH];
struct GameClock g_gameclock;

struct Cell *g_selected = NULL;
SDL_Point g_mouse;

enum PlayingColor g_playingColor = GREEN;

void printCell(const struct Cell * c) {
    printf("CELL %p %p\n", c->container, c->occubant);
}

void findSelected() {
    for (int i = 0; i < BOARD_LENGTH; i++) {
        for (int j = 0; j < BOARD_LENGTH; ++j) {

            int index = i * BOARD_LENGTH + j;
            struct Cell *gridCell = g_cellboard + index;
            //printCell(gridCell);

            if ( SDL_PointInRect(&g_mouse, gridCell->container)
                && gridCell->occubant != NULL
                && gridCell->occubant->color == g_playingColor
                && !SDL_RectEmpty(gridCell->occubant->rect)
            ) {
                g_selected = gridCell;
                return;
            }
        }
    }
}

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
    Uint32 mouseState;
    switch(event->type) {
        case SDL_QUIT:
            g_is_playing = SDL_FALSE;
            break;
        case SDL_MOUSEBUTTONDOWN:
            mouseState = SDL_GetMouseState(&g_mouse.x, &g_mouse.y);
            if ( mouseState & SDL_BUTTON(SDL_BUTTON_LEFT) ) {
                findSelected();
            }
            break;
        default:
            break;
    }
}

void handleKeyState(const Uint8 *states) {
    if (states[SDL_SCANCODE_ESCAPE])
        g_is_playing = SDL_FALSE;
}

SDL_bool load() {

    for (int i = 0; i < BOARD_LENGTH; ++i) {
        for (int j = 0; j < BOARD_LENGTH; ++j) {
            int flatIndex = i * BOARD_LENGTH + j;

            SDL_Rect *container = g_board + flatIndex;
            struct Cell *cell = g_cellboard + flatIndex;

            container->h = 100;
            container->w = 100;
            container->x = 100 * (flatIndex % BOARD_LENGTH) + 20;
            container->y = 100 * (flatIndex / BOARD_LENGTH) + 20;

            cell->container = container;
            cell->columnIndex = j;
            cell->rowIndex = i;

            /** GREEN **/
            if ( i % 2 == 0 && i < (BOARD_LENGTH / 2) ) {
                if (j % 2 == 0) {
                    struct Checker *checker = g_checkers + (g_green_length);
                    SDL_Rect *rect = g_checker_rects + (g_green_length++);

                    rect->x = (100 * (j % BOARD_LENGTH)) + 40;
                    rect->y = (container->y + 20);
                    rect->w = checkerLength;
                    rect->h = checkerLength;

                    checker->color = GREEN;
                    checker->rect = rect;
                    cell->occubant = checker;
                    continue;
                }
            } else if ( i % 2 != 0 && i < (BOARD_LENGTH / 2) - 1) {
                if (j % 2 != 0) {
                    struct Checker *checker = g_checkers + (g_green_length);
                    SDL_Rect *rect = g_checker_rects + (g_green_length++);

                    rect->x = (100 * (j % BOARD_LENGTH)) + 40;
                    rect->y = (container->y + 20);
                    rect->w = checkerLength;
                    rect->h = checkerLength;

                    checker->color = GREEN;
                    checker->rect = rect;
                    cell->occubant = checker;
                    continue;
                }
            }

            /** RED **/
            if ( i % 2 == 0 && i > (BOARD_LENGTH / 2) ) {
                if (j % 2 == 0) {
                    int currentIndex =  (g_green_length + g_red_length);
                    struct Checker *checker = g_checkers + currentIndex;
                    SDL_Rect *rect = g_checker_rects + currentIndex;
                    g_red_length++;

                    rect->x = (100 * (j % BOARD_LENGTH)) + 40;
                    rect->y = (container->y + 20);
                    rect->w = checkerLength;
                    rect->h = checkerLength;

                    checker->color = RED;
                    checker->rect = rect;
                    cell->occubant = checker;
                    continue;
                }
            } else if ( i % 2 != 0 && i > (BOARD_LENGTH / 2) ) {
                if (j % 2 != 0) {
                    int currentIndex =  (g_green_length + g_red_length);
                    struct Checker *checker = g_checkers + currentIndex;
                    SDL_Rect *rect = g_checker_rects + currentIndex;
                    g_red_length++;

                    rect->x = (100 * (j % BOARD_LENGTH)) + 40;
                    rect->y = (container->y + 20);
                    rect->w = checkerLength;
                    rect->h = checkerLength;

                    checker->color = RED;
                    checker->rect = rect;
                    cell->occubant = checker;
                    continue;
                }
            }
        }
    }

    return SDL_TRUE;
}

void update() {
    if (g_selected != NULL) {
        printf("Found selected\n");
        g_selected = NULL;
    }
}

void render() {

    SDL_SetRenderDrawColor(g_renderer, PC_OPAQUE_BLACK);
    SDL_RenderDrawRects(g_renderer, g_board, BOARD_LENGTH * BOARD_LENGTH);

    SDL_SetRenderDrawColor(g_renderer, PC_OPAQUE_GREEN);
    SDL_RenderFillRects(g_renderer, g_checker_rects, g_green_length);

    SDL_SetRenderDrawColor(g_renderer, PC_OPAQUE_RED);
    SDL_RenderFillRects(g_renderer, g_checker_rects + g_green_length, g_red_length);

    SDL_RenderPresent(g_renderer);
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

void initGlobalData() {
    initCells(g_cellboard, BOARD_LENGTH * BOARD_LENGTH);
    GT_gameclock_init(&g_gameclock, MS_PER_UPDATE);
}

int MAIN_NAME(int argc, char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);

    if ( !sdlInit() ) {
        return EXIT_FAILURE;
    }

    initGlobalData();

    if ( !load() ) {
        return EXIT_FAILURE;
    }

    while ( g_is_playing ) {
        pumpEvents();

        while ( GT_CLOCK_SHOULD_UPDATE(g_gameclock) ) {
            update();

            GT_CLOCK_LAG_UPDATE(g_gameclock);
        }

        render();

        GT_CLOCK_TICK(g_gameclock);
    }

    sdlDestroy();
    return EXIT_SUCCESS;
}
