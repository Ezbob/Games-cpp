#pragma once

#include <stack>
#include "GameClock.hpp"
#include "GameState.hpp"
#include "SingleThreadedMessageQueue.hpp"
#include <memory>

namespace asa
{

class GameStateProcessor
{
    std::stack<std::shared_ptr<GameState>> m_gameStates;

    bool m_isPlaying = true;
    bool m_shouldReload = false;

public:
    GameStateProcessor(void) = default;

    void addState(std::shared_ptr<GameState> state);

    void processStates(GameClock &clock);

    void startFromNewState(const std::shared_ptr<GameState> state);

    void quitGame(void);
};

} // namespace asa
