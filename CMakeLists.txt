cmake_minimum_required(VERSION 3.22)

project(Zerializer)

add_subdirectory(include)
add_subdirectory(src)

option(BUILD_EXAMPLES "Build examples" ON)

if (BUILD_EXAMPLES)
    message(STATUS "Building examples...")
    add_subdirectory(examples)
endif()
