#include "synodic/composition/module.h"
#include "doctest/doctest.h"

#include "static_application_types.h"

//#include "static_implementations/static_implementation_a.h"
//#include "static_implementations/static_implementation_b.h"
//#include "static_implementations/static_implementation_c.h"
//
//TEST_SUITE(" Polymorphic Injection")
//{
//	TEST_CASE("Template Function")
//	{
//		const synodic::ModuleParameters params;
//		const auto synodicModule = synodic::CreateModule<
//			StaticInterfaceA, StaticImplementationA, StaticImplementationC>(params);
//
//		SUBCASE("Template")
//		{
//			const auto app = synodicModule.Create<TemplateApp>();
//
//			CHECK(app.Get() == 3);
//		}
//	}
//}