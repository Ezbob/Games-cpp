#pragma once

#include <iostream>
#include <stdexcept>
#include <string>

namespace asa {

    using ErrorGetterFunction_t = const char *(*)(void);

    const char *NoErrorGetter(void);

    template<typename T, ErrorGetterFunction_t ErrorGetter>
    T *CheckNullError(T *returned, const char *message) noexcept {
        if ( returned == nullptr ) {
            std::cerr << "Error: " << message << ": " << ErrorGetter() << std::endl; 
        }
        return returned;
    }

    template<ErrorGetterFunction_t ErrorGetter>
    int CheckError(int success, const char *message) noexcept {
        if ( success != 0 ) { 
            std::cerr << "Error: " << message << ": " << ErrorGetter() << std::endl; 
        }
        return success == 0;
    }

    template<ErrorGetterFunction_t ErrorGetter, typename T>
    T *ThrowOnNullError(T *returned, std::string const& message) {
        if ( returned == nullptr ) {
            std::string err_msg = "Error: " + message + ": ";
            err_msg += ErrorGetter();
            throw std::runtime_error(err_msg);
        }
        return returned;
    }

    template<ErrorGetterFunction_t ErrorGetter>
    int ThrowOnError(int return_code, std::string const& message) {
        if ( return_code != 0 ) {
            std::string err_msg = "Error: " + message + ": ";
            err_msg += ErrorGetter();
            throw std::runtime_error(err_msg); 
        }
        return return_code == 0;
    }

    template<ErrorGetterFunction_t ErrorGetter>
    bool ThrowOnCondition(bool condition, std::string const& message) {
        if ( condition ) {
            std::string err_msg = "Error: " + message + ": ";
            err_msg += ErrorGetter();
            throw std::runtime_error(err_msg); 
        }
        return condition;
    }
}
