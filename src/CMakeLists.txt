# cppaste source directories

# Top-level module
target_sources(cppaste
	PUBLIC
		FILE_SET cppaste_public
		TYPE CXX_MODULES
		FILES
			cppaste.ixx
)

add_subdirectory(composition)
add_subdirectory(injection)
add_subdirectory(binding)
