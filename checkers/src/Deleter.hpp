
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

    using SurfaceDeleter = Deleter<SDL_Surface, NoDeletionOperation>;
    using TextureDeleter = Deleter<SDL_Texture, SDL_DestroyTexture>;
    using WindowDeleter = Deleter<SDL_Window, SDL_DestroyWindow>;
    using RendererDeleter = Deleter<SDL_Renderer, SDL_DestroyRenderer>;
    using FontDeleter = Deleter<TTF_Font, TTF_CloseFont>;

    template <typename T>
    struct sdl_deleter
    {
        using value = void;
    };

    template <>
    struct sdl_deleter<SDL_Surface>
    {
        using value = SurfaceDeleter;
    };

    template <>
    struct sdl_deleter<SDL_Texture>
    {
        using value = TextureDeleter;
    };

    template <>
    struct sdl_deleter<SDL_Window>
    {
        using value = WindowDeleter;
    };

    template <>
    struct sdl_deleter<SDL_Renderer>
    {
        using value = RendererDeleter;
    };

    template <>
    struct sdl_deleter<TTF_Font>
    {
        using value = FontDeleter;
    };

}; // namespace asa
