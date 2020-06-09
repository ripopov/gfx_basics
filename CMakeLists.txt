cmake_minimum_required(VERSION 3.15)

# All dependencies for this project are available in vcpkg
if (DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
    message(STATUS "VCPKG CMAKE_TOOLCHAIN_FILE = ${CMAKE_TOOLCHAIN_FILE}")
endif ()

project(gfx_basics)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(GTest CONFIG REQUIRED)
find_package(tinyobjloader CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
add_compile_definitions(GLM_ENABLE_EXPERIMENTAL)
find_package(SDL2 CONFIG REQUIRED)
find_package(sdl2-image CONFIG REQUIRED)

if (MSVC)
    add_compile_options(/W3 /WX)
else ()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif ()

add_subdirectory(resource)
add_subdirectory(mygfxlib)

enable_testing()
add_subdirectory(test)
