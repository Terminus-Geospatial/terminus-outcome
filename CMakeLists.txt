#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    7/5/2023
#
#    Purpose:  Build terminus-result API
#
cmake_minimum_required( VERSION 3.0 FATAL_ERROR )

project( ${CONAN_PKG_NAME}
            VERSION ${CONAN_PKG_VERSION}
            HOMEPAGE_URL ${CONAN_PKG_URL}
            DESCRIPTION ${CONAN_PKG_DESCRIPTION}
            LANGUAGES CXX
     )

# set some common default compiler options
set( CMAKE_CXX_STANDARD 20 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

#  Configure Conan Search Paths
include( "${CMAKE_BINARY_DIR}/conan_paths.cmake" )
set( CMAKE_FIND_PACKAGE_PREFER_CONFIG TRUE )


# add the new library, specifying SHARED or STATIC is possible
# if not specified it will depend on whether BUILD_SHARED_LIBS is set to ON or OFF
add_library( terminus-outcome  include/terminus/Outcome.hpp )

