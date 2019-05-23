#include "SDL.hpp"
#include "SDL_extra.hpp"
#include "Animator.hpp"
#include "GameState.hpp"
#include "GameClock.hpp"
#include "Actor.hpp"
#include "Tweening.hpp"
#include <iostream>
#include <stack>


const int SCREEN_WIDTH = 840;
const int SCREEN_HEIGHT = 480;

sdl2::Globals globals;
sdl2::Window window;
sdl2::Renderer renderer;


const double MS_PER_UPDATE = 15.0;
    // how much time the update step has been given (in ms)
    // this parameter has to be minimized, but if it is too small
    // then the game update (physics, AI, etc) will never catch up.
    // Also > 0 value

GameClock clock;

struct Man : public Actor {
    SpriteSheetAnimator<4, 1> spriteAnimation{renderer, 64, 205, 7};

    bool isMoving = false;

    Man() {
        worldX = 215.0;
        worldY = 225.0;
    }

    void left() {
        worldX -= 2;

        spriteAnimation.unflip();
        spriteAnimation.run();
        isMoving = true;
    }

    void right() {
        worldX += 2;

        spriteAnimation.flipHorizontal();
        spriteAnimation.run();
        isMoving = true;
    }

    void stop() {
        spriteAnimation.stop();
        spriteAnimation.gotoFrame(0);
        isMoving = false;
    }
};

struct FirstState : public GameState {

    sdl2::Texture background = renderer.createTexture();
    Man manActor;

    const uint8_t *key_state;
    SDL_Event event;

    void handleInput() override {
        while ( SDL_PollEvent(&event) != 0 ) {
            if ( event.type == SDL_QUIT ) {
                globals.isPlaying = false;
            }
        }
        key_state = SDL_GetKeyboardState(nullptr);

        if ( key_state[SDL_SCANCODE_ESCAPE] ) {
            globals.isPlaying = false;
        }
    }

    bool load() override {
        background = sdl2::loadPNG(renderer, "assets/landscape.png");
        manActor.spriteAnimation.load(sdl2::loadPNG(renderer, "assets/man.png"));

        return background.isLoaded() && manActor.spriteAnimation.isLoaded();
    }

    void update() override {
        if ( key_state[SDL_SCANCODE_LEFT] ) {
            manActor.left();
        } else if ( key_state[SDL_SCANCODE_RIGHT] ) {
            manActor.right();
        } else {
            manActor.stop();
        }

        manActor.spriteAnimation.tick();
    }

    void render() override {
        background.render();

        manActor.spriteAnimation.render(manActor.worldX, manActor.worldY);

        renderer.updateScreen();
    }

    ~FirstState() = default;
};

bool init() {
    bool result = true;

    if ( globals.init(SDL_INIT_VIDEO | SDL_INIT_TIMER) ) {
        globals.loadExternLib(sdl2::ExternLibs::SDL_IMAGE, IMG_INIT_PNG);
        globals.loadExternLib(sdl2::ExternLibs::SDL_TTF);
        window.loadWindow("SDL Tutorial",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN);

        renderer = window.getRenderer(-1, SDL_RENDERER_ACCELERATED);
        renderer.setColor(0xFF, 0xFF, 0xFF, 0xFF);
    }

    result = globals.is_initialized && window.isLoaded();

    return result;
}

bool initStates(std::stack<std::shared_ptr<GameState>> &stack) {
    stack.emplace(new FirstState());

    return true;
}

int WinMain() {

    std::stack<std::shared_ptr<GameState>> gameStack;

    if ( !init() ) {
        return 1;
    }

    if ( !initStates(gameStack) ) {
        return 1;
    }

    while ( globals.isPlaying && !gameStack.empty() ) {
        auto state = gameStack.top();

        if ( state->load() ) {
            while ( globals.isPlaying && state->isPlaying ) {
                state->handleInput();

                while ( clock.lag >= MS_PER_UPDATE) {
                    state->update();
                    clock.lag -= MS_PER_UPDATE;
                }

                state->prerender();
                state->render();

                clock.tick();
            }
        }

        gameStack.pop();
    }

    return 0;
}
