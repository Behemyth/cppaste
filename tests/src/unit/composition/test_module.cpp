#include "synodic/composition/module.h"
#include "doctest/doctest.h"
#include "types.h"


TEST_SUITE("Dynamic Polymorphic Injection")
{

	TEST_CASE("Single Implementation")
	{
		const synodic::ModuleParameters params;
		const auto module = synodic::CreateModule<InterfaceAM, ImplementationAM>(params);

		SUBCASE("reference")
		{
			const auto app = module.Create<ReferenceApp>();

			CHECK(app.Get() == 3);
		}
	}

	TEST_CASE("Multiple Implementations")
	{
		const synodic::ModuleParameters params;
		const auto module =
			synodic::CreateModule<InterfaceAM, ImplementationAM, ImplementationBM>(params);

		SUBCASE("reference")
		{
			const auto app = module.Create<ReferenceApp>();

			CHECK(app.Get() == 3);
		}
	}
}


TEST_SUITE(" Polymorphic Injection")
{
	TEST_CASE("Single Implementation")
	{
		const synodic::ModuleParameters params;
		const auto module = synodic::CreateModule<InterfaceAM, ImplementationAM>(params);

		SUBCASE("Template")
		{
			const auto app = module.Create<TemplateApp>();

			CHECK(app.Get() == 3);
		}
	}

	TEST_CASE("Multiple Implementations")
	{
		const synodic::ModuleParameters params;
		const auto module =
			synodic::CreateModule<InterfaceAM, ImplementationAM, ImplementationBM>(params);

		SUBCASE("Template")
		{
			const auto app = module.Create<TemplateApp>();

			CHECK(app.Get() == 3);
		}
	}

	TEST_CASE("Template Function")
	{
		const synodic::ModuleParameters params;
		const auto module = synodic::CreateModule<
			InterfaceBM, ImplementationCM>(params);

		SUBCASE("Template")
		{
			const auto app = module.Create<TemplateFunctionApp>();

			CHECK(app.Get() == 3);
		}
	}
}