#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    7/8/2023
#

#  Configure GoogleTest
find_package( GTest REQUIRED )
include( GoogleTest )

include_directories( ${CMAKE_BINARY_DIR}/include )
include_directories( ${CMAKE_SOURCE_DIR}/include )

set( TEST ${PROJECT_NAME}_test )
add_executable( ${TEST}
    TEST_Optional.cpp
    TEST_Result.cpp
)

target_link_libraries( ${TEST} PRIVATE
    GTest::gtest_main
    ${PROJECT_NAME}
)

gtest_discover_tests( ${TEST} )