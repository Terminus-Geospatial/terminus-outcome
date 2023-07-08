#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    7/8/2023
#
#    Purpose:  Build the Terminus-Outcome documentation.
cmake_minimum_required( VERSION 3.17 )

find_package( Doxygen
    COMPONENTS dot
    OPTIONAL_COMPONENTS dia
)

if( DOXYGEN_FOUND )
    doxygen_add_docs( docs
        ${PROJECT_SOURCE_DIR}/include
    )
endif()