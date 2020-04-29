cmake_minimum_required(VERSION 3.12)

project(SDL2_packed)

set(PREFIX ${CMAKE_CURRENT_SOURCE_DIR})

set(SDL2_PACKED_INCLUDE_DIRS
    ${PREFIX}/SDL2-2.0.9/x86_64-w64-mingw32/include
    ${PREFIX}/SDL2_image-2.0.4/x86_64-w64-mingw32/include
    ${PREFIX}/SDL2_ttf-2.0.15/x86_64-w64-mingw32/include
    ${PREFIX}/SDL2-2.0.9/x86_64-w64-mingw32/include/SDL2
    ${PREFIX}/SDL2_image-2.0.4/x86_64-w64-mingw32/include/SDL2
    ${PREFIX}/SDL2_ttf-2.0.15/x86_64-w64-mingw32/include/SDL2
    PARENT_SCOPE
)

set(SDL2_PACKED_LINK_DIRS
    ${PREFIX}/SDL2-2.0.9/x86_64-w64-mingw32/lib
    ${PREFIX}/SDL2_image-2.0.4/x86_64-w64-mingw32/lib
    ${PREFIX}/SDL2_ttf-2.0.15/x86_64-w64-mingw32/lib
    PARENT_SCOPE
)

set(SDL2_PACKED_LINK_LIBS
    SDL2
    SDL2_image
    SDL2_ttf
    PARENT_SCOPE
)

set(SDL2_PACKED_DLL_DIRS
    ${PREFIX}/SDL2-2.0.9/x86_64-w64-mingw32/bin
    ${PREFIX}/SDL2_image-2.0.4/x86_64-w64-mingw32/bin
    ${PREFIX}/SDL2_ttf-2.0.15/x86_64-w64-mingw32/bin
    PARENT_SCOPE
)