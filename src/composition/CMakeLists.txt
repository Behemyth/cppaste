# Composition module sources

target_sources(cppaste
	PUBLIC
		FILE_SET cppaste_public
		TYPE CXX_MODULES
		FILES
			composition.ixx
			interface.ixx
			storage.ixx
			service.ixx
	PRIVATE
		FILE_SET cppaste_private
		TYPE CXX_MODULES
)
