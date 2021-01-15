#include "doctest/doctest.h"
#include "synodic/composition/package.h"

#include "dynamic_application_types.h"


//TEST_SUITE("Polymorphic Injection")
//{
//	TEST_CASE("Single Modules")
//	{
//		const cppaste::ModuleParameters params;
//		const auto synodicModule = cppaste::CreateModule<DynamicInterfaceA, DynamicImplementationA>(params);
//
//		const auto package = cppaste::Package(synodicModule);
//		const auto app	   = package.Create<UniquePtrApp>();
//
//		CHECK(app.Get() == 3);
//	}
//	
//}