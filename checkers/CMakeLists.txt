cmake_minimum_required(VERSION 3.12)

project("checkers"
    VERSION 0.1
    DESCRIPTION "A simple checker game"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)


# Also look for Find-cmake modules here
set(CMAKE_MODULE_PATH  ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/sdl2")

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
if(${ENABLE_STATS})
    set(STATS_ENV "-D_STATS")
elseif($ENV{ENABLE_STATS})
    set(STATS_ENV "-D_STATS")
else()
    set(STATS_ENV "")
endif()

if(${WIN32})
    SET(ASAN "")
else()
    SET(ASAN "-fsanitize=address,undefined")
endif()

    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG=1 ${STATS_ENV} -Wall -Wextra ${ASAN}")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -D_DEBUG=0")
    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
if(${ENABLE_STATS})
    set(STATS_ENV "/D _STATS")
elseif($ENV{ENABLE_STATS})
    set(STATS_ENV "/D _STATS")
else()
    set(STATS_ENV "")
endif()

  	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Wall /D _DEBUG=1 ${STATS_ENV}")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /D _DEBUG=0")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif()


set(SOURCES
    src/Main.cpp
    src/states/AllStates.cpp
    src/GameTool/GameState.cpp
    src/GameTool/GameClock.cpp
    src/GameTool/SingleThreadedMessageQueue.cpp
    src/GameTool/GameStateProcessor.cpp
    src/GameTool/Easers.cpp

    src/SDL2cpp/ErrorCheck.cpp
    src/SDL2cpp/Renderer.cpp
    src/SDL2cpp/Texture.cpp
    src/SDL2cpp/Window.cpp
    src/SDL2cpp/Surface.cpp
    src/SDL2cpp/Globals.cpp
    src/SDL2cpp/Color.cpp
    src/SDL2cpp/Font.cpp

    src/SDL2cpp/libraries/SDLLibrary.cpp
    src/SDL2cpp/libraries/ImageLibrary.cpp
    src/SDL2cpp/libraries/TTFLibrary.cpp

)

set(INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/src
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS})

if(WIN32)
    # I use the gcc toolchain even on windows
    # so this branch just checks for the included libs
    add_subdirectory(libs/SDL2_packed)

    target_link_directories(${PROJECT_NAME} PRIVATE ${SDL2_PACKED_LINK_DIRS})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_PACKED_LINK_LIBS})
    target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_PACKED_INCLUDE_DIRS})

    # This copies over the dll's to the executable dir so we can run it on windows
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${SDL2_PACKED_DLL_DIRS} $<TARGET_FILE_DIR:${PROJECT_NAME}>)

else(WIN32)
    
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
