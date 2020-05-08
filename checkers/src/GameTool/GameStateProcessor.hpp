#pragma once

#include <stack>
#include "GameClock.hpp"
#include "GameState.hpp"
#include "SingleThreadedMessageQueue.hpp"
#include <functional>
#include <memory>

namespace asa
{

class GameStateProcessor
{
    std::stack<std::shared_ptr<GameState>> gameStates;

    bool m_isPlaying = true;
    bool m_shouldReload = false;

public:

    void addState(std::shared_ptr<GameState> state);

    void processStates(GameClock &);

    void startFromNewState(const std::shared_ptr<GameState> state);

    void quitGame(void);
};

} // namespace asa
