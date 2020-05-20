from conans import ConanFile, CMake

class SynodicCoreConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    requires = "doctest/2.3.7", "boost/1.73.0"
    generators = "cmake_find_package", "cmake_paths"

    def build(self):
        cmake = CMake(self) # it will find the packages by using our auto-generated FindXXX.cmake files
        cmake.configure()
        cmake.build()