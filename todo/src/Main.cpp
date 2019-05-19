#include "SDL.hpp"
#include "SDL_extra.hpp"
#include "Animator.hpp"
#include <iostream>
#include <cmath>

const int SCREEN_WIDTH = 840;
const int SCREEN_HEIGHT = 480;

SDLGlobals globals;
SDLWindow window;
SDLRenderer renderer;

SDL_Event event;
//SpriteSheetAnimator<4, 1> animation(renderer, 64, 205, 5);

SDLTexture fooTexture = renderer.createTexture();
SDLTexture backgroundTexture = renderer.createTexture();
SDLTexture text = renderer.createTexture();

TTFFont font;


template<typename T>
constexpr void lerp(T &out, double f = 0) {
    double t = 1.0 - f;
    out.x[0] = out.x[0] * t + out.x[1] * f;
    out.y[0] = out.y[0] * t + out.y[1] * f;
}

const double MS_PER_UPDATE = 15.0;
    // how much time the update step has been given (in ms)
    // this parameter has to be minimized, but if it is too small
    // then the game update (physics, AI, etc) will never catch up.
    // Also > 0 value

struct Clock {
    uint64_t now = SDL_GetPerformanceCounter();
    uint64_t last = 0;
    double elapsed = 0.0;
    double lag = 0.0;

    void tick() {
        last = now;
        now = SDL_GetPerformanceCounter();
        elapsed = (now - last) * 1000 / (double) SDL_GetPerformanceFrequency();
        lag += elapsed;
    }
} clock;

struct Man {
    double x[2] = { 330, 330 }; // current and next x
    double y[2] = { 250, 250 };
    double speed = 0;
    int moveDirection = 0;
} man;

SDLSurface loadSurface(std::string path) {
    SDLSurface image;
    image.loadPNG(path);

    bool isLoaded = image.isLoaded();
    if ( isLoaded ) {
        image.convertToFormat(window.getSurface());
    }

    return image;
}

SDLTexture loadTexture(std::string path) {
    SDLTexture texture = renderer.createTexture();

    SDLSurface loadedSurface;
    loadedSurface.loadPNG(path);
    if ( loadedSurface.isLoaded() ) {
        loadedSurface.setKeyColor(SDL_TRUE, SDL_MapRGB( loadedSurface.pixelFormat(), 0, 0xFF, 0xFF ));
        texture.load(loadedSurface);
    }

    return texture;
}

SDLTexture loadTextTexture(std::string text, TTF_Font *font, SDL_Color textColor) {
    SDLTexture texture = renderer.createTexture();

    SDLSurface surface;

    surface.load(TTF_RenderText_Solid(font, text.c_str(), textColor));

    if (surface.isLoaded()) {
        texture.load(surface);
    }

    return texture;
}


bool init() {
    bool result = true;

    if ( globals.init(SDL_INIT_VIDEO | SDL_INIT_TIMER) ) {
        globals.loadExternLib(SDLExternLibs::SDL_IMAGE, IMG_INIT_PNG);
        globals.loadExternLib(SDLExternLibs::SDL_TTF);
        window.loadWindow("SDL Tutorial",
            SDL_WINDOWPOS_UNDEFINED, 
            SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH, 
            SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN);

        renderer = window.getRenderer(-1, SDL_RENDERER_ACCELERATED);
        renderer.setColor(0xFF, 0xFF, 0xFF, 0xFF);
        //SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest"); // basically the anti-aliasing
    }

    result = globals.is_initialized && window.isLoaded();

    return result;
}

bool load() {
/*
    fooTexture = loadTexture("assets/foo.png");

    if ( fooTexture.isLoaded() ) {
        animation.load(fooTexture);
        animation.run();
    }

    font.loadTTF("assets/lazy.ttf", 28);

    if ( font.isLoaded() ) {
        text = loadTextTexture("Hello world!", (TTF_Font *) font, {0xff, 0xff, 0xff, 0xff});
    }

    backgroundTexture = loadTexture("assets/landscape.png");

    return fooTexture.isLoaded() && backgroundTexture.isLoaded() && font.isLoaded();
*/
    return true;
}

void update() {

    /*
    if (man.moveDirection > 0) {
        man.x[1] = man.x[0] + 5.0;
        animation.flipHorizontal();
        animation.run();
    } else if (man.moveDirection < 0) {
        man.x[1] = man.x[0] - 5.0;
        animation.unflip();
        animation.run();
    } else {
        animation.gotoFrame(0);
        animation.stop();
    }

    animation.tick();
    */
}

void prerender() {
    /*
    double nextFrameRatio = clock.lag / MS_PER_UPDATE;
    if (man.moveDirection != 0) // if we interpolate while we introduce jitter and wastes computation time (although it's not much here)
        lerp(man, nextFrameRatio);
    */
}

void render() {
    /*
    backgroundTexture.render();
    animation.render(man.x[0], man.y[0]);
    text.render((SCREEN_WIDTH  - text.getWidth()) / 2, (SCREEN_HEIGHT - text.getHeight()) / 4);
    renderer.updateScreen();
    */
}

void handleInput() {
    /*
    while ( SDL_PollEvent(&event) != 0 ) {
        if ( event.type == SDL_QUIT ) {
            globals.is_playing = false;
        }
    }
    const uint8_t *currentKeyState = SDL_GetKeyboardState(nullptr);

    if ( currentKeyState[SDL_SCANCODE_LEFT] ) {
        man.moveDirection = -1;
    } else if ( currentKeyState[SDL_SCANCODE_RIGHT] ) {
        man.moveDirection = 1;
    } else if ( !currentKeyState[SDL_SCANCODE_LEFT] && !currentKeyState[SDL_SCANCODE_LEFT] ) {
        man.moveDirection = 0;
    }
    */
}


int WinMain() {

    if ( init() && load() ) {
        while ( globals.is_playing ) {
            handleInput();

            while ( clock.lag >= MS_PER_UPDATE) {
                update();
                clock.lag -= MS_PER_UPDATE;
            }

            prerender();
            render();

            clock.tick();
        }
    }

    return 0;
}
