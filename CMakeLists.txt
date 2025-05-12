#**************************** INTELLECTUAL PROPERTY RIGHTS ****************************#
#*                                                                                    *#
#*                           Copyright (c) 2025 Terminus LLC                          *#
#*                                                                                    *#
#*                                All Rights Reserved.                                *#
#*                                                                                    *#
#*          Use of this source code is governed by LICENSE in the repo root.          *#
#*                                                                                    *#
#**************************** INTELLECTUAL PROPERTY RIGHTS ****************************#
#
#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    7/5/2023
#
#    Purpose: Build terminus-result API

#  Set CMake Version
cmake_minimum_required( VERSION 3.22 FATAL_ERROR )


if( "${SKIP_CONAN}" STREQUAL "1" )
    message( "Skipping Conan" )
else()
     #  Configure Conan Search Paths
     include( "${CMAKE_BINARY_DIR}/conan_toolchain.cmake" )
     
endif()
set( CMAKE_FIND_PACKAGE_PREFER_CONFIG TRUE )


#  Pull in the Pico SDK if we are using it
if( ${RENDER_DRIVER} STREQUAL "pico" )
    message( "Import PICO SDK: ${PICO_SDK_PATH}" )
    include( "$ENV{PICO_SDK_PATH}/pico_sdk_init.cmake" )
endif()

# Set the Project-Name
project( ${CONAN_PKG_NAME}
            VERSION      ${CONAN_PKG_VERSION}
            HOMEPAGE_URL ${CONAN_PKG_URL}
            DESCRIPTION  ${CONAN_PKG_DESCRIPTION}
            LANGUAGES    CXX )

set( CMAKE_POSITION_INDEPENDENT_CODE ON )

#  Define C++ Level
set( CMAKE_CXX_STANDARD 23 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

#  Give Vscode a fighting chance
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

message( "Application Platform: ${RENDER_DRIVER}" )

# Setup libdl for debug GCC builds
if( CMAKE_BUILD_TYPE STREQUAL "debug" OR CMAKE_BUILD_TYPE STREQUAL "Debug" )
    if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
        set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -rdynamic -ldl" )
    endif()

#  Set Really Strict Compiler Settings
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -pedantic-errors -Wall -Wextra -Wconversion -Wsign-conversion")
endif()


#----------------------------------------#
#-          Update Version File         -#
#----------------------------------------#
# Get the current date
string(TIMESTAMP PROJECT_BUILD_DATE "%Y-%m-%d %H:%M:%S" )

configure_file( ${CMAKE_SOURCE_DIR}/templates/Exports.hpp.in
                ${CMAKE_BINARY_DIR}/include/terminus/outcome/Exports.hpp )

#----------------------------------------#
#-           Find dependencies          -#
#----------------------------------------#
find_package( Boost REQUIRED )

#  Enable code coverage
if( TERMINUS_OUTCOME_ENABLE_COVERAGE )
     terminus_coverage_enable()
endif()

add_library( ${PROJECT_NAME} INTERFACE )

# Configure include paths
target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/terminus>
    $<INSTALL_INTERFACE:include/terminus>  # <prefix>/include/mylib
)

target_link_libraries( ${PROJECT_NAME} INTERFACE
     Boost::headers
)

if( TERMINUS_OUTCOME_ENABLE_TESTS )
     enable_testing()
     add_subdirectory( test/unit )
endif()

#  We will want documentation in the future
#if( TERMINUS_OUTCOME_ENABLE_DOCS or TERMINUS_ENABLE_DOCS )
#    add_subdirectory(docs)
#endif()

#  Install Header Files
install( DIRECTORY ${PROJECT_SOURCE_DIR}/include/terminus DESTINATION include)
install( DIRECTORY ${PROJECT_BINARY_DIR}/include/terminus DESTINATION include )
