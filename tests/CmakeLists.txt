cmake_minimum_required(VERSION 4.5 FATAL_ERROR)

project(tests
	DESCRIPTION "Test project for cppaste"
	LANGUAGES CXX
)

add_executable(tests)

set_target_properties(tests
	PROPERTIES
		LINKER_LANGUAGE CXX
		CXX_STANDARD 23
		CXX_STANDARD_REQUIRED ON
		CXX_EXTENSIONS NO
)

find_package(GTest REQUIRED)

target_link_libraries(tests
	PRIVATE
	synodic::cppaste
	GTest::gtest
	GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests)

add_subdirectory(src/integration)
add_subdirectory(src/unit)
add_subdirectory(src/examples)

if(CMAKE_CONFIGURATION_TYPES)
	set(_test_is_multi_config ON)
else()
	set(_test_is_multi_config OFF)
endif()
add_test(
	NAME test_package_consumer
	COMMAND ${CMAKE_COMMAND}
		"-DTEST_PROJECT_BINARY_DIR=${CMAKE_BINARY_DIR}"
		"-DTEST_SOURCE_DIR=${CMAKE_SOURCE_DIR}/test_package"
		"-DTEST_GENERATOR=${CMAKE_GENERATOR}"
		"-DTEST_GENERATOR_PLATFORM=${CMAKE_GENERATOR_PLATFORM}"
		"-DTEST_GENERATOR_TOOLSET=${CMAKE_GENERATOR_TOOLSET}"
		"-DTEST_CONFIG=$<CONFIG>"
		"-DTEST_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
		"-DTEST_IS_MULTI_CONFIG=${_test_is_multi_config}"
		"-DTEST_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}"
		"-DTEST_MAKE_PROGRAM=${CMAKE_MAKE_PROGRAM}"
		"-DTEST_CXX_COMPILER=${CMAKE_CXX_COMPILER}"
		"-DTEST_CXX_FLAGS=${CMAKE_CXX_FLAGS}"
		"-DTEST_EXE_LINKER_FLAGS=${CMAKE_EXE_LINKER_FLAGS}"
		"-DTEST_CXX_STDLIB_MODULES_JSON=${CMAKE_CXX_STDLIB_MODULES_JSON}"
		-P "${CMAKE_CURRENT_LIST_DIR}/test_package.cmake"
)

# Glob all files to add a source group tree
file(GLOB_RECURSE file_list ${CMAKE_CURRENT_SOURCE_DIR}/*)

# Must be called from the target definition directory. Only matched against target source files (No nested CMakeList.txt, docs, etc).
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${file_list})

# Make the 'tests' project the default startup
set_property(
	DIRECTORY
	${CMAKE_SOURCE_DIR}
	PROPERTY
	VS_STARTUP_PROJECT tests
)
