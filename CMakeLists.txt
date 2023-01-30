cmake_minimum_required(VERSION 3.22)

project(Zerializer)

# All CMakeFiles in subdirectories will be executed
add_subdirectory(include)
add_subdirectory(src)

# Flags for building
option(BUILD_EXAMPLES "Build examples" ON)

# Checking specified flags and start executing
if (BUILD_EXAMPLES)
    message(STATUS "Building examples...")
    add_subdirectory(examples)
endif()
