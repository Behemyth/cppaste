# Injection module sources

target_sources(cppaste
	PUBLIC
		FILE_SET cppaste_public
		TYPE CXX_MODULES
		FILES
			injection.ixx
			configuration.ixx
			dependency.ixx
			injector.ixx
			requirements.ixx
			scope.ixx
			solver.ixx
			source.ixx
			type.ixx
	PRIVATE
		FILE_SET cppaste_private
		TYPE CXX_MODULES
)
