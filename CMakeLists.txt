cmake_minimum_required(VERSION 3.10)

project(create_project CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Detect MSVC
if (MSVC)
    # Set MSVC-specific flags
    add_compile_options(/std:c++17 /W4 /O2)
    add_link_options(/INCREMENTAL:NO /DEBUG:NONE /OPT:REF /OPT:ICF)
else()
    # For GCC/Clang (Linux, MinGW)
    add_compile_options(-std=c++17 -Wall -O3 -s)
endif()

# Find all source files recursively
file(GLOB_RECURSE SOURCES src/code/*.cpp)

add_executable(create-project ${SOURCES})
