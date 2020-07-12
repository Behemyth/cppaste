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
	scm = {
         "type": "git",
         "subfolder": "synodic-core",
         "url": "auto",
         "revision": "auto",
     }

	def build(self):
		cmake = CMake(self)
		cmake.configure()
		cmake.build()

	def package(self):
		self.copy("*.h", dst="include", src="synodic-core")
		self.copy("*synodic-core.lib", dst="lib", keep_path=False)
		self.copy("*.dll", dst="bin", keep_path=False)
		self.copy("*.so", dst="lib", keep_path=False)
		self.copy("*.dylib", dst="lib", keep_path=False)
		self.copy("*.a", dst="lib", keep_path=False)

	def package_info(self):
		self.cpp_info.libs = ["synodic-core"]