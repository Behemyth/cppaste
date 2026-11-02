cmake_minimum_required(VERSION 4.5 FATAL_ERROR)
set(CMAKE_CXX_MODULE_STD ON)

project(PlatformPropertiesExample
	VERSION 0.1.0
	DESCRIPTION "Platform Properties Example using cppaste Framework"
	LANGUAGES CXX
)

add_executable(platform_properties_example)

set_target_properties(platform_properties_example
	PROPERTIES
		LINKER_LANGUAGE CXX
		CXX_STANDARD 23
		CXX_STANDARD_REQUIRED ON
		CXX_EXTENSIONS NO
)

target_compile_features(platform_properties_example PUBLIC cxx_std_23)

find_package(cppaste REQUIRED)

target_link_libraries(platform_properties_example
	PRIVATE
		synodic::cppaste
)

target_sources(platform_properties_example
	PRIVATE
		src/main.cpp
		src/platform_interface.ixx
		src/windows_implementation.ixx
		src/linux_implementation.ixx
		src/macos_implementation.ixx
)
