
#pragma once

#include <any>
#include <memory>

namespace asa {

    struct MessageQueueInterface {
        virtual std::shared_ptr<std::any> poll(void) = 0;
        virtual bool offer(std::shared_ptr<std::any>) = 0;
        virtual std::shared_ptr<std::any> peek(void) const = 0;
        virtual std::size_t size(void) const = 0;
    };

}; // namespace asa
