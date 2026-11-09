cmake_minimum_required(VERSION 4.4.4)
project(gstuff LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(glfw3 REQUIRED)
find_package(OpenMP REQUIRED)

add_executable(gstuff
    main.cpp
    glad/src/gl.c
)

target_include_directories(gstuff PRIVATE
    ${CMAKE_SOURCE_DIR}/glad/include
    lib/
)

if (OpenMP_CXX_FOUND)
    target_link_libraries(gstuff PRIVATE glfw OpenMP::OpenMP_CXX)
endif()
