# @file CMakeLists.txt
#
# CMake files for XMLFramework/library

# Assume all .cpp files are part of the toolkit
file(GLOB SOURCES "*.cpp")

# static object library for framework library
add_library(XMLlibrary OBJECT ${SOURCES})
