/**
 * @brief Platform-specific export macros for CPPaste
 *
 * This header provides cross-platform macros for DLL/shared library symbol visibility.
 * Since C++23 modules cannot export preprocessor macros, this header must be included
 * by downstream projects that need to create plugins or shared libraries.
 */

#ifndef CPPASTE_EXPORT_H
#define CPPASTE_EXPORT_H

#if defined(_WIN32) || defined(__CYGWIN__)
	#define CPPASTE_EXPORT __declspec(dllexport)
	#define CPPASTE_IMPORT __declspec(dllimport)
#else
	#define CPPASTE_EXPORT __attribute__((visibility("default")))
	#define CPPASTE_IMPORT __attribute__((visibility("default")))
#endif

#endif // CPPASTE_EXPORT_H
