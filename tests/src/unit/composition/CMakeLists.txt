target_sources(tests
	PRIVATE
		FILE_SET CXX_MODULES
		FILES
			test_calculator_fixtures.ixx
)

target_sources(tests
	PRIVATE
		test_service.cpp
		test_storage.cpp
)
