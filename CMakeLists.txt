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
cmake_minimum_required( VERSION 4.0.0 FATAL_ERROR )


#  Configure Conan Search Paths
include( "${CMAKE_BINARY_DIR}/conan_toolchain.cmake" )
set( CMAKE_FIND_PACKAGE_PREFER_CONFIG TRUE )


# Set the Project-Name
project( ${CONAN_PKG_NAME}
            VERSION      ${CONAN_PKG_VERSION}
            HOMEPAGE_URL ${CONAN_PKG_URL}
            DESCRIPTION  ${CONAN_PKG_DESCRIPTION}
            LANGUAGES    CXX )

#  Define C++ Level
set( CMAKE_CXX_STANDARD 23 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

#  Give Vscode a fighting chance
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

# Bring in CMake helpers for TERMINUS projects
include( terminus_cmake )

# Force the high warning preset published by terminus_cmake across this project
set( TERMINUS_CXX_FLAGS ${TERMINUS_CXX_FLAGS_HIGH} CACHE STRING "Terminus warning preset" FORCE )

#  Treat warnings as errors
set( CMAKE_COMPILE_WARNING_AS_ERROR ON )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror" )


#----------------------------------------#
#-          Update Version File         -#
#----------------------------------------#
# Get the current date
string(TIMESTAMP PROJECT_BUILD_DATE "%Y-%m-%d %H:%M:%S" )

configure_file( ${CMAKE_SOURCE_DIR}/templates/exports.hpp.in
                ${CMAKE_BINARY_DIR}/include/terminus/outcome/exports.hpp )

set( TERMINUS_OUTCOME_PUBLIC_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/library/include" )
set( TERMINUS_OUTCOME_PUBLIC_HEADERS
    terminus/error.hpp
    terminus/error/error_category.hpp
    terminus/error/error_code.hpp
    terminus/outcome.hpp
    terminus/outcome/error.hpp
    terminus/outcome/error_category.hpp
    terminus/outcome/macros.hpp
    terminus/outcome/optional.hpp
    terminus/outcome/result.hpp
)

set( TERMINUS_OUTCOME_INTERFACE_SOURCES )
foreach(_hdr IN LISTS TERMINUS_OUTCOME_PUBLIC_HEADERS)
    list(APPEND TERMINUS_OUTCOME_INTERFACE_SOURCES
        "$<BUILD_INTERFACE:${TERMINUS_OUTCOME_PUBLIC_INCLUDE_DIR}/${_hdr}>"
        "$<INSTALL_INTERFACE:include/${_hdr}>"
    )
endforeach()

#-------------------------------------#
#-          Find Dependencies        -#
#-------------------------------------#
find_package( Boost REQUIRED )

#  Enable code coverage
if( TERMINUS_OUTCOME_ENABLE_COVERAGE )
     terminus_coverage_enable()
endif()

terminus_lib_create_header_only()

target_link_libraries( ${PROJECT_NAME} INTERFACE
     Boost::headers
)

target_sources( ${PROJECT_NAME}
    INTERFACE
        ${TERMINUS_OUTCOME_INTERFACE_SOURCES}
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
install( DIRECTORY ${PROJECT_BINARY_DIR}/include/terminus DESTINATION include )
install( DIRECTORY ${TERMINUS_OUTCOME_PUBLIC_INCLUDE_DIR}/terminus DESTINATION include )
