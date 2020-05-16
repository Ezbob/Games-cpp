#pragma once

#include <vector>
#include "GameClock.hpp"
#include "GameState.hpp"
#include "SingleThreadedMessageQueue.hpp"
#include <memory>

namespace asa
{

class GameStateProcessor
{
private:
    std::vector<std::shared_ptr<GameState>> m_gameStates;

    bool m_isPlaying = true;
    bool m_shouldReload = false;
    uint32_t m_currentIndex = 0;

    std::shared_ptr<GameState> currentState(void) const;

    void removeCurrentState(void);
public:
    GameStateProcessor(void) = default;

    void addState(std::shared_ptr<GameState> state);

    void processStates(GameClock &clock);

    void startFromNewState(const std::shared_ptr<GameState> state);

    void startFromState(const uint32_t index);

    void quitGame(void);

    uint32_t lastIndex(void) const;
};

} // namespace asa
