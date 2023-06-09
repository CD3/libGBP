from conans import ConanFile, CMake, tools
import os

class ConanBuild(ConanFile):
    settings = "os", "arch", "build_type"
    generators = "CMakeDeps", "CMakeToolchain"
    requires = 'boost/1.80.0','eigen/3.3.7', 'cd3-boost-unit-definitions/0.2.2'
