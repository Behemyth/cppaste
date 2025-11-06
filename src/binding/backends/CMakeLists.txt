# Binding backends module sources

target_sources(cppaste
	PUBLIC
		FILE_SET cppaste_public
		TYPE CXX_MODULES
		FILES
			nanobind.ixx
	PRIVATE
		FILE_SET cppaste_private
		TYPE CXX_MODULES
)
