from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps
from conan.tools.files import get,copy, replace_in_file


class unitconvertRecipe(ConanFile):
    name = "unitconvert"
    version = "0.15"
    package_type = "library"

    # Optional metadata
    license = "MIT"
    author = "CD Clark III"

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "CMakeLists.txt", "src/*", "include/*"

    def requirements(self):
        self.requires("boost/1.80.0", transitive_headers=True)

    def source(self):
        get(self,url="https://github.com/CD3/UnitConvert/archive/refs/tags/0.15.tar.gz", sha256="20731fbb1d384760c8f63d488b5d32e7252a2c90724448c09f2cb33f102fe94e", strip_root=True)
        replace_in_file(self,"CMakeLists.txt", "Boost::disable_autolinking","")


    def config_options(self):
        if self.settings.os == "Windows":
            self.options.rm_safe("fPIC")

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure(variables={'BUILD_UNIT_TESTS':False})
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        if self.settings.build_type == "Debug":
            self.cpp_info.libs = ["UnitConvert-d"]
        else:
            self.cpp_info.libs = ["UnitConvert"]
        self.cpp_info.set_property("cmake_file_name","UnitConvert")
        self.cpp_info.set_property("cmake_target_name","UnitConvert::UnitConvert")





