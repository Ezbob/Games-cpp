
#include "ImageLibrary.hpp"
#include <stdexcept>
#include "sdl2cpp/ErrorCheck.hpp"

using namespace asa;

ImageLibrary::ImageLibrary(uint32_t flags)
{
    ThrowOnCondition<NoErrorGetter>((IMG_Init(flags) & flags) != flags, "SDL Image could not be initialized");
    m_is_initialized = true;
}

ImageLibrary::~ImageLibrary(void)
{
    if (m_is_initialized)
    {
        IMG_Quit();
    }
}