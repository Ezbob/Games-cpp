
#pragma once

#include "FreeingFunctionType.hpp"
#include <memory>
#include <iostream>

namespace asa {

    template<typename ContainedType, FreeingFunctionType<ContainedType> FreeingFunction>
    class SharedContainerBase {

    protected:
        constexpr static FreeingFunctionType<ContainedType> freeingFunction = FreeingFunction;
        std::shared_ptr<ContainedType> m_contained = nullptr;

    public:
        SharedContainerBase() = default;

        explicit operator ContainedType *() const {
            return m_contained.get();
        }

        operator const ContainedType *() const {
            return m_contained.get();
        }

        ContainedType *get_ptr() const noexcept
        {
            return m_contained.get();
        }

        bool isLoaded() const {
            return m_contained != nullptr;
        }

        void load(ContainedType *newStuff) {
            if ( newStuff == nullptr ) {
                std::cerr << "Error: Could not load item into shared memory container " << std::endl;
            } else {
                m_contained = std::shared_ptr<ContainedType>(newStuff, FreeingFunction);
            }
        }
    };

}; // namespace asa
