#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    7/5/2023
#
#    Purpose:  Build terminus-result API
#
cmake_minimum_required( VERSION 3.0 FATAL_ERROR )

project( terminus-result )

# set some common default compiler options
set(CMAKE_CXX_STANDARD 20)


# add the new library, specifying SHARED or STATIC is possible
# if not specified it will depend on whether BUILD_SHARED_LIBS is set to ON or OFF
add_library(demo ${DEMO_SOURCES} ${DEMO_HEADERS})

target_include_directories(demo PUBLIC
    "${CMAKE_SOURCE_DIR}/include"
    "${CMAKE_BINARY_DIR}/exports")