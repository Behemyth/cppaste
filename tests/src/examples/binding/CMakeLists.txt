# Binding Example Tests

# Add the example module files to make modules available
target_sources(tests
	PRIVATE
		FILE_SET CXX_MODULES
		BASE_DIRS ${CMAKE_SOURCE_DIR}/examples/binding/src
		FILES
			${CMAKE_SOURCE_DIR}/examples/binding/src/binding.ixx
			${CMAKE_SOURCE_DIR}/examples/binding/src/plugin_contract.ixx
			${CMAKE_SOURCE_DIR}/examples/binding/src/plugin_host.ixx
)

# Add the test files
target_sources(tests
	PRIVATE
		test_binding_example.cpp
)

# Build test plugin as a shared library for integration tests
add_library(test_plugin SHARED
	test_plugin.cpp
)

set_target_properties(test_plugin
	PROPERTIES
		CXX_STANDARD 23
		CXX_STANDARD_REQUIRED ON
		CXX_EXTENSIONS NO
		# Output to the same directory as the test executable
		RUNTIME_OUTPUT_DIRECTORY $<TARGET_FILE_DIR:tests>
		LIBRARY_OUTPUT_DIRECTORY $<TARGET_FILE_DIR:tests>
)

# Include the plugin interface header and library export header
target_include_directories(test_plugin
	PRIVATE
		${CMAKE_SOURCE_DIR}/include
		${CMAKE_SOURCE_DIR}/examples/binding/src
)

# Ensure test plugin is built before tests run
add_dependencies(tests test_plugin)

# The plugin's file name and location differ by platform, so the tests load it by its built path.
target_compile_definitions(tests
	PRIVATE
		"CPPASTE_TEST_PLUGIN_PATH=\"$<TARGET_FILE:test_plugin>\""
)
