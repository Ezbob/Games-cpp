
#pragma once

#include "ErrorCheck.hpp"
#include "Deleter.hpp"
#include <string>
#include <memory>

namespace asa
{

    using ErrorStrGetter_t = const char *(*)(void);

    template <typename T>
    using sdl_unique_ptr = std::unique_ptr<T, typename sdl_deleter<T>::type>;

    template <typename T>
    using sdl_shared_ptr = std::shared_ptr<T>;

    template <typename T>
    auto make_sdl_shared(T *w)
    {
        using Deleter_t = typename sdl_deleter<T>::type;
        auto ptr = asa::ThrowOnNull(w, "Could not create shared_ptr");
        return sdl_shared_ptr<T>(ptr, Deleter_t());
    }

    template <typename T>
    auto make_sdl_unique(T *w)
    {
        using Deleter_t = typename sdl_deleter<T>::type;
        auto ptr = asa::ThrowOnNull(w, "Could not create unique_ptr");
        return sdl_unique_ptr<T>(ptr, Deleter_t());
    }

}; // namespace asa
