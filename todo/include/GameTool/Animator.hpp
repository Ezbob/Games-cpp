#ifndef ANIMATOR_H_dbc46f9133f3f0f7103ecf9fae61de05
#define ANIMATOR_H_dbc46f9133f3f0f7103ecf9fae61de05

#include "SDL2cpp/Texture.hpp"
#include <array>

namespace gtool {

    template<int columns, int rows>
    class SpriteSheetAnimator {

        sdl2::Texture spriteSheetTexture;
        int currentFrame;
        int sprite_width;
        int sprite_height;
        int framerate;
        int numberOfFrames = columns * rows;

        bool is_running = false;

        SDL_RendererFlip flip = SDL_FLIP_NONE;

        std::array<SDL_Rect, columns * rows> frames;

        void initFrames() {
            int frame_index = 0;
            for (int j = 0; j < rows; ++j) {
                for (int i = 0; i < columns; ++i) {
                    frames[frame_index].x = sprite_width * i;
                    frames[frame_index].y = sprite_height * j;
                    frames[frame_index].w = sprite_width;
                    frames[frame_index].h = sprite_height;
                    frame_index++;
                }
            }
        }

    public:
        SpriteSheetAnimator(sdl2::Texture spriteSheet, int sprite_width, int sprite_height, int framerate = 16) : 
            spriteSheetTexture(std::move(spriteSheet)),
            currentFrame(0), sprite_width(sprite_width),
            sprite_height(sprite_height),
            framerate(framerate) {

            initFrames();
        }

        SpriteSheetAnimator(sdl2::Renderer &renderer, int sprite_width, int sprite_height, int framerate = 16) 
            : spriteSheetTexture(renderer)
            , currentFrame(0)
            , sprite_width(sprite_width)
            , sprite_height(sprite_height)
            , framerate(framerate) {

            initFrames();
        }

        void load(sdl2::Texture &&spriteSheet) {
            spriteSheetTexture = spriteSheet;
        }

        bool isLoaded() {
            return spriteSheetTexture.isLoaded();
        }

        void run() {
            is_running = true;
        }

        void stop() {
            is_running = false;
        }

        void render(const int x, const int y) {
            spriteSheetTexture.render(x, y, frames[(currentFrame / framerate)], flip);
        }

        void flipHorizontal() {
            flip = SDL_FLIP_HORIZONTAL;
        }
        
        void unflip() {
            flip = SDL_FLIP_NONE;
        }

        void update() {
            if (is_running) {
                ++currentFrame;

                if (currentFrame / framerate >= numberOfFrames) {
                    currentFrame = 0;
                }
            }
        }

        int getSpriteHeight() const {
            return sprite_height;
        }

        int getSpriteWidth() const {
            return sprite_width;
        }

        bool isRunning() const {
            return is_running;
        }

        void gotoFrame(int index) {
            if (index >= 0 && index < numberOfFrames) currentFrame = index;
        }

        int getFramerate() const {
            return framerate;
        }

        void setFramerate(int framerate) {
            framerate = framerate;
        }
    };
}



#endif