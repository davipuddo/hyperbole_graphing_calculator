cmake_minimum_required(VERSION 4.4.4)
project(hgc LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(glfw3 REQUIRED)

add_executable(hgc
    main.cpp
    glad/src/gl.c
)

target_include_directories(hgc PRIVATE
    ${CMAKE_SOURCE_DIR}/glad/include
    lib/
)

target_link_libraries(hgc PRIVATE glfw)
