#include "synodic/composition/module.h"
#include "doctest/doctest.h"

#include "dynamic_application_types.h"


TEST_SUITE("Dynamic Polymorphic Injection")
{

	TEST_CASE("Single Implementation")
	{
		const synodic::ModuleParameters params;
		const auto synodicModule = synodic::CreateModule<DynamicInterfaceA, DynamicImplementationA>(params);

		SUBCASE("reference")
		{
			const auto app = synodicModule.Create<ReferenceApp>();

			CHECK(app.Get() == 3);
		}
	}

	TEST_CASE("Multiple Implementations")
	{
		const synodic::ModuleParameters params;
		const auto synodicModule =
			synodic::CreateModule<DynamicInterfaceA, DynamicImplementationA, DynamicImplementationB>(params);

		SUBCASE("Reference")
		{
			const auto app = synodicModule.Create<ReferenceApp>();

			CHECK(app.Get() == 3);
		}
	}
}