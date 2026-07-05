cmake_minimum_required(VERSION 3.21)

project(GBOne VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported OUTPUT error)
if(ipo_supported)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

add_library(GBOne_options INTERFACE)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(GBOne_options INTERFACE
        /W4 /WX /permissive-
        
        /Zc:__cplusplus
        /Zc:preprocessor
        /utf-8
        
        /GR-
        /arch:AVX2
        /Gw
    )

    target_compile_definitions(GBOne_options INTERFACE _CRT_SECURE_NO_WARNINGS)
endif()

find_package(SDL3 REQUIRED)

add_executable(GBOne)

target_sources(GBOne PRIVATE 
    src/main.cpp
    src/core/System.cpp
    src/core/memory/Cartridge.cpp
    src/core/video/Ppu.cpp
    src/frontend/Application.cpp
    src/frontend/Renderer.cpp
    src/frontend/Texture.cpp
    src/frontend/Window.cpp
)

target_include_directories(GBOne PRIVATE src)

target_link_libraries(GBOne PRIVATE 
    GBOne_options
    SDL3::SDL3
)
