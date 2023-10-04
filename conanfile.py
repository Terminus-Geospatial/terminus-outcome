#    File:    conanfile.py
#    Author:  Marvin Smith
#    Date:    7/8/2023
from conan import ConanFile, tools
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps
from conan.tools.files import copy

class ConanProject(ConanFile):

    name = "terminus_outcome"
    version = "0.0.2"

    license = "Terminus Proprietary"
    author  = "Marvin Smith <marvin_smith1@me.com>"
    url     = "https://bitbucket.org/msmith81886/terminus-outcome/src"
    description = "Modern and predictable C++ error handling"
    topics = ("terminus","outcome")

    options = { "shared": [True, False],
                "with_tests": [True, False],
                "with_docs": [True, False],
                "with_coverage": [True, False]
    }

    default_options = { "shared": True,
                        "with_tests": True,
                        "with_docs": True,
                        "with_coverage": False,
                        "boost/*:shared": True }

    settings = "os", "compiler", "build_type", "arch"

    def build_requirements(self):
        self.test_requires("gtest/1.13.0")
        self.tool_requires("terminus_cmake/1.0.1")

    def requirements(self):
        self.requires("boost/1.82.0")
        self.requires("terminus_log/0.0.2")

    def _configure_cmake(self):
        cmake = CMake(self)
        cmake.configure()
        return cmake

    def generate(self):
        tc = CMakeToolchain(self)
        tc.variables["CONAN_PKG_NAME"]        = self.name
        tc.variables["CONAN_PKG_VERSION"]     = self.version
        tc.variables["CONAN_PKG_DESCRIPTION"] = self.description
        tc.variables["CONAN_PKG_URL"]         = self.url

        tc.variables["TERMINUS_OUTCOME_ENABLE_TESTS"]    = self.options.with_tests
        tc.variables["TERMINUS_OUTCOME_ENABLE_DOCS"]     = self.options.with_docs
        tc.variables["TERMINUS_OUTCOME_ENABLE_COVERAGE"] = self.options.with_coverage
        tc.generate()

        deps = CMakeDeps(self)
        deps.generate()

    def build(self):
        cmake = self._configure_cmake()
        cmake.build()

    def package(self):
        cmake = self._configure_cmake()
        cmake.install()

        # This will also copy the "include" folder
        copy(self, "*.h", self.source_folder, self.package_folder)
        self.cpp_info.libs = tools.files.collect_libs(self)

    def package_info(self):
        # For header-only packages, libdirs and bindirs are not used
         # so it's necessary to set those as empty.
         self.cpp_info.bindirs = []
         self.cpp_info.libdirs = []

    def package_id(self):
        self.info.clear()

    def export_sources(self):

        for p in [ "CMakeLists.txt", "include/*", "test/*", "README.md" ]:
            copy( self, p, self.recipe_folder, self.export_sources_folder )