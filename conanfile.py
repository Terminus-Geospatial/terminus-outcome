from conans import ConanFile, CMake, tools

class ConanProject(ConanFile):

    name = "terminus_outcome"
    version = "0.0.1"

    license = "Terminus Proprietary"
    author  = "Marvin Smith"
    url     = "https://bitbucket.org/msmith81886/terminus-outcome/src"
    description = "Modern and predictable C++ error handling"
    topics = ()

    options = {
        "with_tests": [True, False]
    }

    default_options = {
        "with_tests": True
    }

    generators = "cmake_paths", "cmake_find_package"
    export_sources = "CMakeLists.txt", "include/*", "test/*", "README.md"

    def build_requirements(self):
        self.build_requires( "gtest/1.13.0" )

    def _configure_cmake(self):
        cmake = CMake(self)

        cmake.definitions["CONAN_PKG_NAME"]        = self.name
        cmake.definitions["CONAN_PKG_VERSION"]     = self.version
        cmake.definitions["CONAN_PKG_DESCRIPTION"] = self.description
        cmake.definitions["CONAN_PKG_URL"]         = self.url

        cmake.configure()
        return cmake

    def build(self):
        cmake = self._configure_cmake()
        cmake.build()

    def package(self):
        cmake = self._configure_cmake()
        cmake.install()

    def package_id(self):
        self.info.header_only()
