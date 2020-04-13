#ifndef HEADER_GUARD_a17e097f8bd12c4c14b2c994eb545006
#define HEADER_GUARD_a17e097f8bd12c4c14b2c994eb545006

#include "SDL_events.h"
#include <cstdint>

namespace gtool {

    struct GameState {
        using BaseClass = GameState;
        void pumpEvents();

        virtual ~GameState() = default;
        virtual void handleKeyState(const uint8_t *keyState);
        virtual void handleEvent(const SDL_Event &event);

        virtual bool load();
        virtual void update();
        virtual void render();

        bool isLoaded() const;
        bool isPlaying() const;
        bool isPlaying(bool);

    protected:
        bool isLoaded(bool);
        bool m_isPlaying = true;
        bool m_isLoaded = false; // as we push and pop states we need to only load once
    };
}

#endif
