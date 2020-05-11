
#pragma once

#include "SDL.h"
#include "SDL_ttf.h"

namespace asa
{

    template <typename T>
    using Deleter_Func_t = void (*)(T *);

    using Lib_Deleter_Func_t = void (*)(void);


    template <typename T>
    void NoDeletionOperation(T *)
    {
        /*Do nothing*/
    }

    /**
     * Functor that patches in deleter/close functions defined as
     * normal functions. This is for interfacing with code that is
     * originally written in C.
     */
    template <typename T, Deleter_Func_t<T> deleter_function>
    struct Deleter
    {
        void operator()(T *a)
        {
            deleter_function(a);
        }
    };

    template <Lib_Deleter_Func_t deleter_function>
    struct LibDeleter
    {
        ~LibDeleter(void)
        {
            deleter_function();
        }
    };

    template <typename T>
    struct sdl_deleter
    {
        using value = void;
    };

    template <>
    struct sdl_deleter<SDL_Surface>
    {
        using value = Deleter<SDL_Surface, NoDeletionOperation>;
    };

    template <>
    struct sdl_deleter<SDL_Texture>
    {
        using value = Deleter<SDL_Texture, SDL_DestroyTexture>;
    };

    template <>
    struct sdl_deleter<SDL_Window>
    {
        using value = Deleter<SDL_Window, SDL_DestroyWindow>;
    };

    template <>
    struct sdl_deleter<SDL_Renderer>
    {
        using value = Deleter<SDL_Renderer, SDL_DestroyRenderer>;
    };

    template <>
    struct sdl_deleter<TTF_Font>
    {
        using value = Deleter<TTF_Font, TTF_CloseFont>;
    };

}; // namespace asa
