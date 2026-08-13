cmake_minimum_required(VERSION 3.20)

project(visualizer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL3 REQUIRED CONFIG)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

file(GLOB SOURCES
    ${CMAKE_SOURCE_DIR}/src/*.cpp
    ${CMAKE_SOURCE_DIR}/external/imgui/*.cpp
    ${CMAKE_SOURCE_DIR}/external/kissfft/*.c
)

add_executable(main ${SOURCES})

target_include_directories(main PRIVATE 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/external/imgui
    ${CMAKE_SOURCE_DIR}/external/kissfft
)

target_link_libraries(main PRIVATE
    SDL3::SDL3
    OpenGL::GL
    GLEW::GLEW
)