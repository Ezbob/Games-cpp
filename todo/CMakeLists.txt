cmake_minimum_required(VERSION 3.0)

SET(EXEC "game")

project(${EXEC})
set(${EXEC}_VERSION_MAJOR 0)
set(${EXEC}_VERSION_MINOR 1)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

set(SOURCES
    src/Main.cpp
    src/SDL2cpp/Renderer.cpp
    src/SDL2cpp/Texture.cpp
    src/SDL2cpp/Window.cpp
    src/SDL2cpp/Surface.cpp
    src/SDL2cpp/Globals.cpp
    src/SDLExtra/Font.cpp
)

set(INCLUDE_DIRS
    "${PROJECT_SOURCE_DIR}/include"
)

set(SDL_INCLUDE_DIRS
    "${PROJECT_SOURCE_DIR}/libs/SDL2-2.0.9/x86_64-w64-mingw32/include/SDL2"
    "${PROJECT_SOURCE_DIR}/libs/SDL2_image-2.0.4/x86_64-w64-mingw32/include/SDL2"
    "${PROJECT_SOURCE_DIR}/libs/SDL2_ttf-2.0.15/x86_64-w64-mingw32/include/SDL2"
)

set(SDL_LINK_DIRS
    "${PROJECT_SOURCE_DIR}/libs/SDL2-2.0.9/x86_64-w64-mingw32/lib"
    "${PROJECT_SOURCE_DIR}/libs/SDL2_image-2.0.4/x86_64-w64-mingw32/lib"
    "${PROJECT_SOURCE_DIR}/libs/SDL2_ttf-2.0.15/x86_64-w64-mingw32/lib"
)

SET(SDL_LIBS
    SDL2
    SDL2_image
    SDL2_ttf
)

add_executable(${EXEC} ${SOURCES})

target_link_directories(${EXEC} PRIVATE ${SDL_LINK_DIRS})
target_link_libraries(${EXEC} PRIVATE ${SDL_LIBS})
target_include_directories(${EXEC} PRIVATE ${SDL_INCLUDE_DIRS})
target_include_directories(${EXEC} PRIVATE ${INCLUDE_DIRS})
