from conans import ConanFile, CMake

class SynodicCoreConan(ConanFile):
	name = "cppaste"
	version = "0.1.0"
	license = "MIT"
	homepage = "https://github.com/Synodic-Software/CPPaste"
	settings = "os", "compiler", "build_type", "arch"
	requires = "doctest/2.4.0", "di/1.2.0"
	generators = "cmake_find_package"
	default_options  = {"di:with_extensions": True}
	scm = {
         "type": "git",
         "subfolder": "cppaste",
         "url": "auto",
         "revision": "auto",
     }

	def build(self):
		cmake = CMake(self)
		cmake.configure(source_folder="cppaste")
		cmake.build()

	def package(self):
		self.copy("*.h", dst="include", src="cppaste")
		self.copy("*cppaste.lib", dst="lib", keep_path=False)
		self.copy("*.dll", dst="bin", keep_path=False)
		self.copy("*.so", dst="lib", keep_path=False)
		self.copy("*.dylib", dst="lib", keep_path=False)
		self.copy("*.a", dst="lib", keep_path=False)

	def package_info(self):
		self.cpp_info.libs = ["cppaste"]