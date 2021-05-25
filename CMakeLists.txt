# @file CMakeLists.txt
# 
# CMake files for XMLFramework

cmake_minimum_required(VERSION 3.19)

# project info
project(XMLFramework)

# C++17 standard
set(CMAKE_CXX_STANDARD 17)

# Common bin directory for all executables
if (NOT MSVC)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif()

# Common lib directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Include directories for each layer
set(XMLFRAMEWORK_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/framework/include)
set(XMLTOOLKIT_INCLUDE_DIR   ${PROJECT_SOURCE_DIR}/toolkit/include)
set(XMLLIBRARY_INCLUDE_DIR   ${PROJECT_SOURCE_DIR}/library/include)

# Turn on warnings
if (MSVC)
    # warning level 4
    add_compile_options(/W4)
else()
    # standard warnings
    add_compile_options(-Wall)
endif()

# Build applications
add_subdirectory(apps)

# Build framework
add_subdirectory(framework)

# Build toolkit
add_subdirectory(toolkit)

# Build library
add_subdirectory(library)
