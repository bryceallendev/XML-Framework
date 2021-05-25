# @file CMakeLists.txt
#
# CMake files for XMLFramework/framework

# Assume all .cpp files are part of the framework
file(GLOB SOURCES "*.cpp")

# static object library for framework
add_library(XMLframework OBJECT ${SOURCES})

# include directories for framework
target_include_directories(XMLframework PUBLIC ${XMLTOOLKIT_INCLUDE_DIR})

# static libraries for framework
target_link_libraries(XMLframework XMLtoolkit)
