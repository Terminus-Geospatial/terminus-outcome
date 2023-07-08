#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    7/5/2023
#
#    Purpose:  Build terminus-result API
#
cmake_minimum_required( VERSION 3.0 FATAL_ERROR )

#  Configure Conan Search Paths
include( "${CMAKE_BINARY_DIR}/conan_toolchain.cmake" )
set( CMAKE_FIND_PACKAGE_PREFER_CONFIG TRUE )


project( ${CONAN_PKG_NAME}
            VERSION ${CONAN_PKG_VERSION}
            HOMEPAGE_URL ${CONAN_PKG_URL}
            DESCRIPTION ${CONAN_PKG_DESCRIPTION}
            LANGUAGES CXX
     )

# set some common default compiler options
set( CMAKE_CXX_STANDARD 20 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

#  Give Vscode a fighting chance
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Bring in CMake helpers for TERMINUS projects
include( terminus_cmake )

#  Enable code coverage
if( TERMINUS_OUTCOME_ENABLE_COVERAGE )
     terminus_coverage_enable()
endif()

#  This function is defined inside terminus_cmake package
#  It creates an INTERFACE library target needed for a header-only library.
terminus_lib_create_header_only()

#  Link in dependencies
find_package( Boost REQUIRED )
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

