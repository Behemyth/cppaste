#include "synodic/composition/module.h"
#include "doctest/doctest.h"

#include "static_application_types.h"

TEST_SUITE(" Polymorphic Injection")
{
	TEST_CASE("Template Function")
	{
		const synodic::ModuleParameters params;
		const auto synodicModule = synodic::CreateModule<
			StaticInterfaceA, StaticImplementationA>(params);

		SUBCASE("Template")
		{
			const auto app = synodicModule.Create<TemplateApp>();

			CHECK(app.Get() == 3);
		}
	}
}