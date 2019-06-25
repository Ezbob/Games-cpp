cmake_minimum_required(VERSION 3.0)

project("game")

SET(EXEC ${PROJECT_NAME})

set(${EXEC}_VERSION_MAJOR 0)
set(${EXEC}_VERSION_MINOR 1)

set(CMAKE_CXX_STANDARD 17)

set(STATS_ENV "")
if($ENV{ENABLE_STATS})
    set(STATS_ENV "-D_STATS")
endif()

LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${STATS_ENV} -Wall -Wextra -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

set(SOURCES
    src/Main.cpp
    src/SDL2cpp/AssetLoading.cpp
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

add_executable(${EXEC} ${SOURCES})

target_include_directories(${EXEC} PRIVATE ${INCLUDE_DIRS})

if(WIN32)
  set(SDL_INCLUDE_DIRS
      "${PROJECT_SOURCE_DIR}/libs/SDL2-2.0.9/x86_64-w64-mingw32/include"
      "${PROJECT_SOURCE_DIR}/libs/SDL2_image-2.0.4/x86_64-w64-mingw32/include"
      "${PROJECT_SOURCE_DIR}/libs/SDL2_ttf-2.0.15/x86_64-w64-mingw32/include"
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

  target_link_directories(${EXEC} PRIVATE ${SDL_LINK_DIRS})
  target_link_libraries(${EXEC} PRIVATE ${SDL_LIBS})
  target_include_directories(${EXEC} PRIVATE ${SDL_INCLUDE_DIRS})

else()

  # Find and link SDL2
  find_package(SDL2 REQUIRED)
  target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIRS})
  target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})

  # Find and link SDL2_image
  find_package(SDL2_image REQUIRED)
  target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_IMAGE_INCLUDE_DIRS})
  target_link_libraries(${PROJECT_NAME} ${SDL2_IMAGE_LIBRARIES})

  # Find and link SDL2_gfx
  find_package(SDL2_ttf REQUIRED)
  target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_TTF_INCLUDE_DIRS})
  target_link_libraries(${PROJECT_NAME} ${SDL2_TTF_LIBRARIES})

endif(WIN32)

