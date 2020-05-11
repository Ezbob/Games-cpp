#pragma once

#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include <iostream>
#include <stdexcept>
#include <memory>

namespace asa {

    using ErrorGetterFunction_t = const char *(*)(void);

    template<typename T>
    struct sdl_error_message_getter {
        using type = ErrorGetterFunction_t;
        constexpr static type value = SDL_GetError;
    };

    template<typename T>
    using DeleteFunction_t = void (*)(T);

    const char *NoDescriptionGetter(void);

    template<typename T>
    T *CheckNullError(T *returned, const char *message) {
        if ( returned == nullptr ) { 
            std::cerr << "Error: " << message << ": " << sdl_error_message_getter<T>::value() << std::endl; 
        }
        return returned;
    }

    template<ErrorGetterFunction_t ErrorGetter>
    int CheckError(int success, const char *message) {
        if ( success != 0 ) { 
            std::cerr << "Error: " << message << ": " << ErrorGetter() << std::endl; 
        }
        return success == 0;
    }

    template<typename T>
    T *ThrowOnNull(T *returned, const char *message) {
        if ( returned == nullptr ) {
            std::string msg = "Error: " + std::string(message) + ": ";
            msg += sdl_error_message_getter<T>::value();
            throw std::runtime_error(msg);
        }
        return returned;
    }

    template<ErrorGetterFunction_t ErrorGetter>
    void ThrowOnNonZero(int success, const char *message) {
        if ( success != 0 ) {
            std::string msg = "Error: " + std::string(message) + ": ";
            msg += ErrorGetter();
            throw std::runtime_error(msg); 
        }
    }

    template<ErrorGetterFunction_t ErrorGetter>
    void ThrowOnCondition(bool condition, const char *message) {
        if ( condition ) { 
            std::string msg = "Error: " + std::string(message) + ": ";
            msg += ErrorGetter();
            throw std::runtime_error(msg); 
        }
    }


}
