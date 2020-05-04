#pragma once

#include <stack>
#include "GameClock.hpp"
#include "GameState.hpp"
#include "SingleThreadedMessageQueue.hpp"
#include <functional>
#include <memory>

namespace asa
{
using GameStateStack = std::stack<std::shared_ptr<GameState>>;

class GameStateProcessor
{
    GameStateStack gameStates;
    GameClock clock;

    std::unique_ptr<MessageQueueInterface> comm_queue;

    bool m_isPlaying = true;
    bool m_shouldReload = false;

public:
    GameStateProcessor(unsigned int targetFps);
    GameStateProcessor(unsigned int, std::unique_ptr<MessageQueueInterface>);

    void initStates(std::function<void(GameStateStack &, MessageQueueInterface&)> initFunction);

    void processStates();

    void startFromNewState(const std::shared_ptr<GameState> state);

    asa::GameClock const &getClock() const;

    void quitGame();
};

} // namespace asa
