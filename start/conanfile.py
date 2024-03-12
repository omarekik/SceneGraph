from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps
 
class Start(ConanFile):
    name = "start"
    version = "0.1"
    settings = "os", "arch", "compiler", "build_type"
    generators = "VirtualBuildEnv", "VirtualRunEnv"
    
    def requirements(self):
        self.requires("boost/1.79.0@")
        self.requires("gtest/1.12.1@")
        self.requires("ms-gsl/4.0.0@")

    def generate(self):
        CMakeToolchain(self).generate()
        CMakeDeps(self).generate()

    def build(self):
        cmake = CMake(self)                # CMake helper auto-formats CLI arguments for CMake
        cmake.configure()                  # cmake -DCMAKE_TOOLCHAIN_FILE=conantoolchain.cmake
        cmake.build()                      # cmake --build .  
        cmake.test()

    def package(self):
        cmake = CMake(self)                # For CMake projects which define an install target, leverage it
        cmake.install()                    # cmake --build . --target=install 
                                           # sets CMAKE_INSTALL_PREFIX = <appropriate directory in conan cache>
