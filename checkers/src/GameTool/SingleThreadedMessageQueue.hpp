
#pragma once

#include "MessageQueueInterface.hpp"
#include <deque>

namespace asa {

    class SingleThreadedMessageQueue : public MessageQueueInterface {
    private:
        std::deque<std::shared_ptr<std::any>> m_queue;
    public:
        bool offer(std::shared_ptr<std::any> elem) override;
        std::shared_ptr<std::any> poll(void) override;
        std::shared_ptr<std::any> peek(void) const override;
        std::size_t size(void) const override;
    };

}; // namespace asa
