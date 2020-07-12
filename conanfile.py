from conans import ConanFile, CMake

class SynodicCoreConan(ConanFile):
	name = "synodic-core"
	version = "0.1.0"
	settings = "os", "compiler", "build_type", "arch"
	requires = "doctest/2.4.0", "di/1.1.0"
	generators = "cmake_find_package"
	default_options  = {"di:with_extensions": True}

	def build(self):
		cmake = CMake(self) # it will find the packages by using our auto-generated FindXXX.cmake files
		cmake.configure()
		cmake.build()