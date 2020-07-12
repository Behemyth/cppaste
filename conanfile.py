from conans import ConanFile, CMake

class SynodicCoreConan(ConanFile):
	name = "synodic-core"
	version = "0.1.0"
	license = "GPL-3.0"
	homepage = "https://github.com/Synodic-Software/Synodic-Core"
	settings = "os", "compiler", "build_type", "arch"
	requires = "doctest/2.4.0", "di/1.1.0"
	generators = "cmake_find_package"
	default_options  = {"di:with_extensions": True}
	no_copy_source = True

	def build(self):
		cmake = CMake(self)
		cmake.configure()
		cmake.build()
