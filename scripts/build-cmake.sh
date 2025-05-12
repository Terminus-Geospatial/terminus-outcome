#!/usr/bin/env bash
#
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
#    File:    build-cmake.sh
#    Author:  Marvin Smith
#    Date:    May 18, 2025
#

set -e

#   Build Flags
BUILD_MODE='unix'
BUILD_TYPE='Release'

#  CMake Settings
PROJECT_NAME='terminus_outcome'
PROJECT_VERSION="$(grep 'version = ' conanfile.py | awk '{ print $3 }' | sed 's/"//g')"
PROJECT_URL="$(grep ' url ' conanfile.py | awk '{ print $3 }' | sed 's/"//g')"
PROJECT_DESC="Modern and predictable C++ error handling"

#  Make steps
DO_INSTALL='0'

#  Set the CMake Executable
CMAKE_EXE='cmake'
MAKE_EXE='make'
MAKE_ARGS=''
EXTRA_ARGS=''

#------------------------------------------------#
#-          Print Usage Instructions            -#
#------------------------------------------------#
function usage()
{
    echo "usage: $0 <flags>" 
    echo 
    echo '  -h|--help  : Print this message and exit.'
    echo
    echo '  --unix  : Build for unix (default)' 
    echo '  --pico  : Build for RPI Pico'
    echo '  --emsdk : Build for WASM'
    echo
    echo '  --debug : Build with symbols'
    echo 
    echo '  -v | --verbose : Run make with VERBOSE=1'
    echo 
    echo '  --install   : Run `make install` after build'
    echo
}

#------------------------------------------------#
#-          Parse Command-Line Options          -#
#------------------------------------------------#
while [ $# -gt 0 ]; do
    case $1 in
        -h|--help)
            usage
            exit 0
            ;;

        --unix)
            BUILD_MODE='unix'
            ;;
        
        --pico)
            BUILD_MODE='pico'
            ;;
        
        --emsdk)
            BUILD_MODE='emsdk'
            #CMAKE_EXE='emcmake cmake'
            #MAKE_EXE='emmake make'
            export EMSDK_SYSROOT="${EMSDK}/upstream/emscripten/cache/sysroot"
            EXTRA_ARGS="${EXTRA_ARGS} -DEMSDK_SYSROOT="${EMSDK}/upstream/emscripten/cache/sysroot"
            # -DCMAKE_TOOLCHAIN_FILE=${EMSDK}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake"
            ;;

        -c|--clean)
            echo 'Removing build folder'
            rm -rf build
            ;;

        --debug)
            BUILD_TYPE='Debug'
            ;;

        -v|--verbose)
            MAKE_ARGS='VERBOSE=1'
            ;;

        --install)
            DO_INSTALL='1'
            ;;

        *)
            echo "error: Unsupported flag: $1"
            ;;
    esac
    shift
done


mkdir -p build

pushd build

#  Do not run with Conan
echo "CMake Executable: ${CMAKE_EXE}"
${CMAKE_EXE} -DRENDER_DRIVER="${BUILD_MODE}" \
             -DCONAN_PKG_NAME="${PROJECT_NAME}" \
             -DCONAN_PKG_VERSION="${PROJECT_VERSION}" \
             -DCONAN_PKG_URL="${PROJECT_URL}" \
             -DCONAN_PKG_DESCRIPTION="${PROJECT_DESC}" \
             -DCMAKE_BUILD_TYPE="${BUILD_TYPE}" \
             ${EXTRA_ARGS} \
             -DSKIP_CONAN='1' .. 

# Run Compilation
${MAKE_EXE} ${MAKE_ARGS}

if [ "${DO_INSTALL}" == '1' ]; then
    ${MAKE_EXE} install
fi

popd
