cmake_minimum_required(VERSION 3.19)

set(CMAKE_TOOLCHAIN_FILE "C:/local/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file" FORCE)

include(${CMAKE_TOOLCHAIN_FILE})

set(CMAKE_CXX_STANDARD 20)

set(VCPKG_DEFAULT_TRIPLET "x64-windows")

project(sdl-paint)

find_package(SDL2 CONFIG REQUIRED)

find_package(SDL2_image REQUIRED)

file(GLOB_RECURSE SRC_FILES src/*.cpp src/*.h)

add_executable(${PROJECT_NAME} src/main.cpp ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2 SDL2_image::SDL2_image)