cmake_minimum_required(VERSION 4.5 FATAL_ERROR)

set(CMAKE_CXX_MODULE_STD ON)

project(cppaste
	VERSION 0.1.0
	DESCRIPTION "Synodic Software's Core"
	LANGUAGES CXX
)

option(BUILD_TESTING "Build test projects" ON)
option(BUILD_SHARED_LIBS "Build shared library" OFF)

add_library(cppaste)
add_library(synodic::cppaste ALIAS cppaste)

set_target_properties(cppaste
	PROPERTIES
	LINKER_LANGUAGE CXX
	CXX_STANDARD 23
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS NO
)

target_compile_features(cppaste PUBLIC cxx_std_23)

target_link_libraries(cppaste
	PRIVATE
)

target_sources(cppaste
	PUBLIC
	FILE_SET cppaste_public
	TYPE CXX_MODULES
	PRIVATE
	FILE_SET cppaste_private
	TYPE CXX_MODULES
)

# Gather and define all the project and sub-project sources
add_subdirectory(
	src
)

# NOTE: For IDE setups that do not use CMake directly, we setup all IDE directories to mimic
# their system directory structure. See https://gitlab.kitware.com/cmake/cmake/-/issues/25992
file(GLOB_RECURSE file_list ${CMAKE_CURRENT_SOURCE_DIR}/src/*)

# Must be called from the target definition directory. Only matched against 'target_sources'
# files (No nested CMakeList.txt, docs, etc).
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src FILES ${file_list})

if(BUILD_TESTING)
	enable_testing()
	add_subdirectory(tests)
endif()

# Installation
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Generate package config files
write_basic_package_version_file(
	cppasteConfigVersion.cmake
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
	tool/cmake/cppasteConfig.cmake.in
	cppasteConfig.cmake
	INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cppaste
)

install(TARGETS cppaste
	EXPORT cppasteTargets
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}

	FILE_SET cppaste_public DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cppaste
	FILE_SET cppaste_private DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cppaste
)

# Install public headers (export macros for downstream plugin development)
install(
	DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Export and package configuration
install(EXPORT cppasteTargets
	FILE cppasteTargets.cmake
	NAMESPACE synodic::
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cppaste
	CXX_MODULES_DIRECTORY cxx-modules
)

install(
	FILES
	${CMAKE_CURRENT_BINARY_DIR}/cppasteConfig.cmake
	${CMAKE_CURRENT_BINARY_DIR}/cppasteConfigVersion.cmake
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cppaste
)

# Build-tree package for local-source (editable) consumers. It mirrors the installed
# lib/cmake/cppaste layout, so a local checkout resolves through the same path as the package.
set(cppaste_BUILD_PACKAGE_DIR ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}/cmake/cppaste)

export(EXPORT cppasteTargets
	FILE ${cppaste_BUILD_PACKAGE_DIR}/cppasteTargets.cmake
	NAMESPACE synodic::
	CXX_MODULES_DIRECTORY cxx-modules
)

configure_file(${CMAKE_CURRENT_BINARY_DIR}/cppasteConfig.cmake ${cppaste_BUILD_PACKAGE_DIR}/cppasteConfig.cmake COPYONLY)
configure_file(${CMAKE_CURRENT_BINARY_DIR}/cppasteConfigVersion.cmake ${cppaste_BUILD_PACKAGE_DIR}/cppasteConfigVersion.cmake COPYONLY)
