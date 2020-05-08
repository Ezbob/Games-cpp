
#include "ImageLibrary.hpp"
#include <stdexcept>

ImageLibrary::ImageLibrary(uint32_t flags)
{
    if (IMG_Init(flags) & flags != flags)
    {
        throw std::runtime_err("Image library could not be initialized");
    }
    else
    {
        m_is_initialized = true;
    }
}

ImageLibrary::~ImageLibrary(void)
{
    if (m_is_initialized)
    {
        IMG_Quit();
    }
}