# Platform Properties Example Tests

# Add the example module files to make modules available
target_sources(tests
	PRIVATE
		FILE_SET CXX_MODULES
		BASE_DIRS ${CMAKE_SOURCE_DIR}/examples/platform_properties/src
		FILES
			${CMAKE_SOURCE_DIR}/examples/platform_properties/src/platform_interface.ixx
			${CMAKE_SOURCE_DIR}/examples/platform_properties/src/windows_implementation.ixx
			${CMAKE_SOURCE_DIR}/examples/platform_properties/src/linux_implementation.ixx
			${CMAKE_SOURCE_DIR}/examples/platform_properties/src/macos_implementation.ixx
)

# Add the test files as regular source files
# TODO: Update tests to use new unified polymorphism framework
target_sources(tests
	PRIVATE
)
