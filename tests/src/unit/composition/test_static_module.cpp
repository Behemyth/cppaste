#include "doctest/doctest.h"
#include "static_application_types.h"
#include "static_implementations/static_implementation_a.h"
#include "static_implementations/static_implementation_b.h"
#include "static_implementations/static_implementation_c.h"
#include "synodic/composition/module.h"
#include "synodic/composition/package.h"

TEST_SUITE(" Polymorphic Injection")
{
	TEST_CASE("Template Function")
	{
		const synodic::ModuleParameters params;
		const auto synodicModule =
			synodic::CreateModule<StaticInterfaceA, StaticImplementationA, StaticImplementationC>(
				params);

		const auto package = synodic::Package(synodicModule);

		SUBCASE("Template")
		{
			const auto app = package.Create<TemplateApp>();
		}
	}
}