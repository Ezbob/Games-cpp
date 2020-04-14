#pragma once

#include "SDL2/SDL.h"
#include <iostream>

namespace asa {

    using ErrorGetterFunction_t = const char *(*)();

    template<typename T, ErrorGetterFunction_t ErrorGetter>
    T *CheckNullError(T *returned, const char *message) {
        if ( returned == nullptr ) { 
            std::cerr << "Error: " << message << ": " << ErrorGetter() << std::endl; 
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
}
