#pragma once

#include <memory>
#include <iostream>
#include "ErrorCheck.hpp"

namespace asa {

    template<typename T>
    using FreeingFunctionType = void (*)(T *);

    /*
    * The SDL_Window type "inherits" from this base, because it is an incomplete type,
    * and Windows generally outlives most other object of a game. 
    */
    template<typename ContainedType, FreeingFunctionType<ContainedType> FreeingFunction>
    class ContainerBase {

    protected:
        constexpr static FreeingFunctionType<ContainedType> freeingFunction = FreeingFunction;
        ContainedType *m_contained = nullptr;

    public:
        ~ContainerBase() {
            if (m_contained != nullptr) {
                FreeingFunction(m_contained);
            }
        }

        explicit operator ContainedType *() {
            return m_contained;
        }

        operator const ContainedType *() {
            return m_contained;
        }

        operator bool() {
            return isLoaded();
        }

        bool isLoaded() const {
            return m_contained != nullptr;
        }

        void load(ContainedType *newStuff) {
            m_contained = newStuff;
            if (m_contained == nullptr) {
                std::cerr << "Error: Could not load item into memory container" << std::endl;
            }
        }
    };

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
}
