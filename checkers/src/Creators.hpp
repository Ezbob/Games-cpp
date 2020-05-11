
#pragma once

#include "ErrorCheck.hpp"
#include <string>
#include <memory>

namespace asa
{

    using ErrorStrGetter_t = const char *(*)(void);

    template <typename T>
    auto createShared(T *w)
    {
        using Deleter_t = typename sdl_deleter<T>::value;
        auto ptr = asa::ThrowOnNull(w, "Could not create shared_ptr");
        return std::shared_ptr<T>(ptr, Deleter_t());
    }

    template <typename T>
    auto createUnique(T *w)
    {
        using Deleter_t = typename sdl_deleter<T>::value;
        auto ptr = asa::ThrowOnNull(w, "Could not create unique_ptr");
        return std::unique_ptr<T, Deleter_t>(ptr, Deleter_t());
    }

}; // namespace asa
