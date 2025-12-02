cmake_minimum_required(VERSION 4.0 FATAL_ERROR)

# Enable std module support for MSVC - MUST be before project()
set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")
set(CMAKE_CXX_MODULE_STD ON)

project(BindingExample
	VERSION 0.1.0
	DESCRIPTION "Plugin Binding Example using CPPaste Framework"
	LANGUAGES CXX
)

add_library(binding_example)
add_library(example::binding ALIAS binding_example)

set_target_properties(binding_example
	PROPERTIES
		LINKER_LANGUAGE CXX
		CXX_STANDARD 23
		CXX_STANDARD_REQUIRED ON
		CXX_EXTENSIONS NO
		OUTPUT_NAME "binding_example"
)

target_compile_features(binding_example PUBLIC cxx_std_23)

find_package(cppaste REQUIRED)

target_link_libraries(binding_example
	PUBLIC
		synodic::cppaste
)

target_sources(binding_example
	PUBLIC
		FILE_SET binding_public
		TYPE CXX_MODULES
		FILES
			src/binding.ixx
			src/plugin_contract.ixx
			src/plugin_host.ixx
)

# Installation
include(GNUInstallDirs)

install(TARGETS binding_example
	EXPORT binding_exampleTargets
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	FILE_SET binding_public DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/binding_example
)

# C header for plugin interface
install(
	FILES src/plugin_interface.h
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/cppaste/binding
)
