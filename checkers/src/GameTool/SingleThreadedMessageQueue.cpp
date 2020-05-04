
#include "SingleThreadedMessageQueue.hpp"

using namespace asa;

bool SingleThreadedMessageQueue::offer(std::shared_ptr<std::any> elem) {
    m_queue.push_back(elem);
    return true;
}

std::shared_ptr<std::any> SingleThreadedMessageQueue::poll(void) {
    auto front = m_queue.front();
    m_queue.pop_front();
    return front;
}

std::shared_ptr<std::any> SingleThreadedMessageQueue::peek(void) const {
    return m_queue.front();
}

std::size_t SingleThreadedMessageQueue::size(void) const {
    return m_queue.size();
}