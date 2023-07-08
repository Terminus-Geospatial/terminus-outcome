#  Developer Documentation

## Building

This library uses the Terminus Integrated Build Process, which requires the scripts found in the `terminus-cmake` reepository.

Once you clone that repository and add `scripts` to your `PATH`, you can run the following command from the root of this repository to build the library:

    conan-build.bash -r

## Unit Tests

Unit-tests are driven by CMake and can be run from the build root after a successful build.

    conan-build.bash -r
    cd build
    ctest --output-on-failure

## Conan Package Testing

You can test the packaging done by Conan by running the following command from the root of this repository:

    conan test test/package/conanfile.py terminus_outcome/<version>@

Replace `<version>` with the version of the library you want to test.