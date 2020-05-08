
#pragma once

#include "FreeingFunctionType.hpp"
#include <iostream>

namespace asa
{

    template <typename ContainedType, FreeingFunctionType<ContainedType> FreeingFunction>
    class ContainerBase
    {

    protected:
        constexpr static FreeingFunctionType<ContainedType> freeingFunction = FreeingFunction;
        ContainedType *m_contained = nullptr;

    public:
        ~ContainerBase()
        {
            if (m_contained != nullptr)
            {
                FreeingFunction(m_contained);
            }
        }

        explicit operator ContainedType *() noexcept
        {
            return m_contained;
        }

        operator const ContainedType *() noexcept
        {
            return m_contained;
        }

        operator bool() noexcept
        {
            return isLoaded();
        }

        ContainedType *get_ptr() const noexcept
        {
            return m_contained;
        }

        bool isLoaded() const noexcept
        {
            return m_contained != nullptr;
        }

        void load(ContainedType *newStuff)
        {
            m_contained = newStuff;
            if (m_contained == nullptr)
            {
                std::cerr << "Error: Could not load item into memory container" << std::endl;
            }
        }
    };

}; // namespace asa
