############################# INTELLECTUAL PROPERTY RIGHTS #############################
##                                                                                    ##
##                           Copyright (c) 2024 Terminus LLC                          ##
##                                All Rights Reserved.                                ##
##                                                                                    ##
##          Use of this source code is governed by LICENSE in the repo root.          ##
##                                                                                    ##
############################# INTELLECTUAL PROPERTY RIGHTS #############################
#
#    File:    conanfile.py
#    Author:  Marvin Smith
#    Date:    7/8/2023
#
from conan import ConanFile, tools
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps
from conan.tools.files import copy

class ConanProject(ConanFile):

    name = "terminus_outcome"
    version = "0.0.7"

    license = "Terminus Proprietary"
    author  = "Marvin Smith <marvin_smith1@me.com>"
    url     = "git@github.com:Terminus-Geospatial/terminus-outcome.git"
    description = "Modern and predictable C++ error handling"
    topics = ("terminus","outcome")

    implements = ["auto_header_only"]

    options = { "render_driver": [ 'unix', 'pico', 'emsdk' ],
                "with_tests": [True, False],
                "with_docs": [True, False],
                "with_coverage": [True, False]
    }

    default_options = { "render_driver": 'unix',
                        "with_tests": True,
                        "with_docs": True,
                        "with_coverage": False }

    settings = "os", "compiler", "build_type", "arch"

    def build_requirements(self):
        self.build_requires("cmake/4.0.1")
        self.test_requires("gtest/1.16.0")
        self.tool_requires("terminus_cmake/1.0.6")

    def requirements(self):
        self.requires("boost/1.87.0")

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

        tc.variables["SKIP_CONAN"]    = False
        tc.variables["RENDER_DRIVER"] = self.options.render_driver
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

    def export_sources(self):

        for p in [ "CMakeLists.txt", "include/*", "test/*", "README.md" ]:
            copy( self,
                  p,
                  self.recipe_folder,
                  self.export_sources_folder )
