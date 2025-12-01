# Binding module sources

target_sources(cppaste
	PUBLIC
		FILE_SET cppaste_public
		TYPE CXX_MODULES
		FILES
			binding.ixx
			c_abi.ixx
			plugin.ixx
	PRIVATE
		FILE_SET cppaste_private
		TYPE CXX_MODULES
)

add_subdirectory(backends)
