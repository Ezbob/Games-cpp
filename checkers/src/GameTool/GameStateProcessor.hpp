#pragma once

#include <stack>
#include "GameClock.hpp"
#include "GameState.hpp"
#include <functional>
#include <memory>

namespace asa
{
using GameStateStack = std::stack<std::shared_ptr<GameState>>;

class GameStateProcessor
{
    GameStateStack gameStates;
    GameClock clock;

    bool m_isPlaying = true;
    bool m_shouldReload = false;

public:
    GameStateProcessor(unsigned int targetFps);

    void initStates(std::function<void(GameStateStack &)> initFunction);

    void processStates();

    void startFromNewState(const std::shared_ptr<GameState> state);

    asa::GameClock const &getClock() const;

    void quitGame();
};

} // namespace asa
