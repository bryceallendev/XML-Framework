# @file CMakeLists.txt
# 
# CMake build files for XMLFramework/apps
#
# Place any custom app run commands in this build file

# Download CLI11 into directory named external
set(CMAKE_EXTERNAL_SOURCE_DIR ${CMAKE_BINARY_DIR}/external)
file(DOWNLOAD https://github.com/CLIUtils/CLI11/releases/download/v1.9.1/CLI11.hpp ${CMAKE_EXTERNAL_SOURCE_DIR}/CLI11.hpp)

# Assume each .cpp file is a separate application
file(GLOB APPLICATIONS "*.cpp")

# Generate executable for each application
foreach(APP ${APPLICATIONS})

    # Use basename (no path, no extension) of .cpp file for application name
    get_filename_component(APPBASENAME "${APP}" NAME_WLE)

    # Application is based on .cpp file only
    add_executable(${APPBASENAME} ${APPBASENAME}.cpp)

    # include directories for entire framework
    target_include_directories(${APPBASENAME} PUBLIC
        ${CMAKE_EXTERNAL_SOURCE_DIR}
        ${XMLFRAMEWORK_INCLUDE_DIR}
        ${XMLTOOLKIT_INCLUDE_DIR}
        ${XMLLIBRARY_INCLUDE_DIR}
    )

    # static libraries for entire framework
    target_link_libraries(${APPBASENAME} XMLlibrary XMLtoolkit XMLframework)

endforeach()

# Run command-line client example
configure_file(fragment.cpp.xml ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/fragment.cpp.xml COPYONLY)
add_custom_target(run COMMENT "Run example command-line client xmlpipeline"
                      COMMAND $<TARGET_FILE:xmlpipeline> --xpath //src:unit fragment.cpp.xml
                      DEPENDS xmlpipeline
                      USES_TERMINAL
                      WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
