#include "synodic/composition/module.h"
#include "doctest/doctest.h"

//#include "dynamic_implementations/dynamic_implementation_a.h"
//#include "dynamic_implementations/dynamic_implementation_b.h"
//
//TEST_SUITE("Dynamic Polymorphic Injection")
//{
//
//	TEST_CASE("Single Implementation")
//	{
//		const cppaste::ModuleParameters params;
//		const auto synodicModule = cppaste::CreateModule<DynamicInterfaceA, DynamicImplementationA>(params);
//
//		SUBCASE("reference")
//		{
//			const auto app = synodicModule.Create<ReferenceApp>();
//
//			CHECK(app.Get() == 3);
//		}
//	}
//
//	TEST_CASE("Multiple Implementations")
//	{
//		const cppaste::ModuleParameters params;
//		const auto synodicModule =
//			cppaste::CreateModule<DynamicInterfaceA, DynamicImplementationA, DynamicImplementationB>(params);
//
//		SUBCASE("Reference")
//		{
//			const auto app = synodicModule.Create<ReferenceApp>();
//
//			CHECK(app.Get() == 3);
//		}
//	}
//}