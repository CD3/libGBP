from conans import ConanFile, CMake, tools
import os

class ConanBuild(ConanFile):
    settings = "os", "arch", "build_type"
    generators = "CMakeDeps", "CMakeToolchain"
    requires = 'boost/1.72.0','eigen/3.3.7', 'BoostUnitDefinitions/0.2@cd3/devel'
