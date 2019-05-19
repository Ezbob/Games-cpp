#ifndef HEADER_GUARD_f608888504445bd55db8b149f35cfa17
#define HEADER_GUARD_f608888504445bd55db8b149f35cfa17

#include <memory>
#include <iostream>
#include "SDL2cpp/ErrorCheck.hpp"

template<typename T>
using FreeingFunction_t = void (*)(T *);

/*
 * The SDL_Window type "inherites" from this base, because it is an incomplete type,
 * and Windows generally outlives most other object of a game. 
 */
template<class Derived, typename ContainedType, FreeingFunction_t<ContainedType> FreeingFunction>
class ContainerBase {

protected:
    constexpr static FreeingFunction_t<ContainedType> freeingFunction = FreeingFunction;

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

template<class Derived, typename ContainedType, FreeingFunction_t<ContainedType> FreeingFunction>
class SharedContainerBase {

protected:
    constexpr static FreeingFunction_t<ContainedType> freeingFunction = FreeingFunction;
    std::shared_ptr<ContainedType> m_contained = nullptr;

public:
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

#endif
