#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    7/8/2023
#
#    Purpose:  Build demo package
cmake_minimum_required( VERSION 3.17 )

#  Configure Conan Search Paths
include( "${CMAKE_BINARY_DIR}/conan_toolchain.cmake" )
set( CMAKE_FIND_PACKAGE_PREFER_CONFIG TRUE )

project( Outcome_Package_Test )

set( CMAKE_CXX_STANDARD 20 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

find_package( terminus_outcome REQUIRED )

add_executable( example example.cpp )

target_link_libraries( example terminus_outcome::terminus_outcome )
