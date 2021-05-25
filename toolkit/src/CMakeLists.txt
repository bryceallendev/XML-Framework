# @file CMakeLists.txt
#
# CMake files for XMLFramework/toolkit

# Assume all .cpp files are part of the toolkit
file(GLOB SOURCES "*.cpp")

# static object library for toolkit
add_library(XMLtoolkit OBJECT ${SOURCES})

# include directories for toolkit
target_include_directories(XMLtoolkit PUBLIC ${XMLLIBRARY_INCLUDE_DIR})

# static libraries for toolkit
target_link_libraries(XMLtoolkit XMLlibrary)
