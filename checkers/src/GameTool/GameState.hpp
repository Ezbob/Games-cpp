#pragma once

#include "SDL_events.h"
#include <cstdint>

namespace asa {

    struct GameState {
        using BaseClass = GameState;
        void pumpEvents();

        virtual ~GameState() = default;
        virtual void handleKeyState(const uint8_t *keyState);
        virtual void handleEvent(const SDL_Event &event);

        virtual bool load(void);
        virtual void update(void);
        virtual void render(void);

        bool isLoaded(void) const;
        bool isPlaying(void) const;
        bool isPlaying(bool);

    protected:
        bool isLoaded(bool);
        bool m_isPlaying = true;
        bool m_isLoaded = false; // as we push and pop states we need to only load once
    };
}
