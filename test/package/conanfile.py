#    File:    conanfile.py
#    Author:  Marvin Smith
#    Date:    7/8/2023
import os

from conan import ConanFile, tools
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps, cmake_layout
from conan.tools.build import can_run

class OutcomePackageTestConan( ConanFile ):

    name = "terminus_outcome_test"
    version = "0.0.2"
    description = "Test for Terminus Outcome Package"

    settings = "os", "compiler", "build_type", "arch"

    def requirements(self):
        self.requires("terminus_outcome/0.0.10")

    def _configure_cmake(self):
        cmake = CMake(self)
        cmake.configure()
        return cmake

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()

        deps = CMakeDeps(self)
        deps.generate()

    def build(self):
        cmake = self._configure_cmake()
        cmake.build()

    def test(self):
        if can_run(self):
            self.run( ".%sexample" % os.sep )