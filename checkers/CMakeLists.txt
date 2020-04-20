cmake_minimum_required(VERSION 3.12)

project("checkers"
    VERSION 0.1
    DESCRIPTION "A simple checker game"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)

if(${ENABLE_STATS})
    set(STATS_ENV "-D_STATS")
elseif($ENV{ENABLE_STATS})
    set(STATS_ENV "-D_STATS")
else()
    set(STATS_ENV "")
endif()

# Also look for Find-cmake modules here
set(CMAKE_MODULE_PATH  ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/sdl2")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG=1 ${STATS_ENV} -Wall -Wextra -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -D_DEBUG=0")
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

set(SOURCES
    src/Main.cpp
    src/states/AllStates.cpp
    src/GameTool/GameState.cpp
    src/GameTool/GameClock.cpp
    src/GameTool/GameStateProcessor.cpp
    src/GameTool/Tweening2DPoint.cpp
    src/GameTool/Easers.cpp
    src/SDL2cpp/Renderer.cpp
    src/SDL2cpp/Texture.cpp
    src/SDL2cpp/Window.cpp
    src/SDL2cpp/Surface.cpp
    src/SDL2cpp/Globals.cpp
    src/SDL2cpp/Color.cpp
    src/SDL2cpp/Font.cpp
)

set(INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/src
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS})

if(WIN32)
    # I use the gcc toolchain even on windows
    # so this branch just checks for the included libs
    set(SDL_INCLUDE_DIRS
        ${PROJECT_SOURCE_DIR}/libs/SDL2-2.0.9/x86_64-w64-mingw32/include
        ${PROJECT_SOURCE_DIR}/libs/SDL2_image-2.0.4/x86_64-w64-mingw32/include
        ${PROJECT_SOURCE_DIR}/libs/SDL2_ttf-2.0.15/x86_64-w64-mingw32/include
        ${PROJECT_SOURCE_DIR}/libs/SDL2-2.0.9/x86_64-w64-mingw32/include/SDL2
        ${PROJECT_SOURCE_DIR}/libs/SDL2_image-2.0.4/x86_64-w64-mingw32/include/SDL2
        ${PROJECT_SOURCE_DIR}/libs/SDL2_ttf-2.0.15/x86_64-w64-mingw32/include/SDL2
    )

    set(SDL_LINK_DIRS
        ${PROJECT_SOURCE_DIR}/libs/SDL2-2.0.9/x86_64-w64-mingw32/lib
        ${PROJECT_SOURCE_DIR}/libs/SDL2_image-2.0.4/x86_64-w64-mingw32/lib
        ${PROJECT_SOURCE_DIR}/libs/SDL2_ttf-2.0.15/x86_64-w64-mingw32/lib
    )

    set(SDL_LIBS
        SDL2
        SDL2_image
        SDL2_ttf
    )

    target_link_directories(${PROJECT_NAME} PRIVATE ${SDL_LINK_DIRS})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL_LIBS})
    target_include_directories(${PROJECT_NAME} PRIVATE ${SDL_INCLUDE_DIRS})

    set(SDL_DLL_DIRS
        ${PROJECT_SOURCE_DIR}/libs/SDL2-2.0.9/x86_64-w64-mingw32/bin
        ${PROJECT_SOURCE_DIR}/libs/SDL2_image-2.0.4/x86_64-w64-mingw32/bin
        ${PROJECT_SOURCE_DIR}/libs/SDL2_ttf-2.0.15/x86_64-w64-mingw32/bin
    )

    # This copies over the dll's to the executable dir so we can run it on windows
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${SDL_DLL_DIRS} $<TARGET_FILE_DIR:${PROJECT_NAME}>)

else(WIN32)

    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined")
    # on ubuntu we use the cmake modules to find the libraries and the header include dirs

    target_link_libraries(${PROJECT_NAME} PRIVATE "asan" "lsan")

    find_package(SDL2 REQUIRED)
    target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES})

    find_package(SDL2_image REQUIRED)
    target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_IMAGE_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_IMAGE_LIBRARIES})

    find_package(SDL2_ttf REQUIRED)
    target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_TTF_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_TTF_LIBRARIES})

endif(WIN32)

set(ASSET_DIR
    ${PROJECT_SOURCE_DIR}/assets
)
# Assets copy
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_directory ${ASSET_DIR} "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets")
