#include "synodic/composition/module.h"
#include "doctest/doctest.h"
#include "types.h"


TEST_SUITE("Dynamic Polymorphic Injection")
{

	TEST_CASE("Single Implementation")
	{
		const synodic::StaticModuleParameters params;
		const auto dynamicModule = synodic::CreateStaticModule<Interface, ImplementationA>(params);

		SUBCASE("reference")
		{
			const auto app = dynamicModule.Create<ReferenceApp>();

			CHECK(app.Get() == 3);
		}
	}

	TEST_CASE("Multiple Implementations")
	{
		const synodic::StaticModuleParameters params;
		const auto dynamicModule =
			synodic::CreateStaticModule<Interface, ImplementationA, ImplementationB>(params);

		SUBCASE("reference")
		{
			const auto app = dynamicModule.Create<ReferenceApp>();

			CHECK(app.Get() == 3);
		}
	}
}


TEST_SUITE("Static Polymorphic Injection")
{
	TEST_CASE("Single Implementation")
	{
		const synodic::StaticModuleParameters params;
		const auto staticModule = synodic::CreateStaticModule<Interface, ImplementationA>(params);

		SUBCASE("Template")
		{
			const auto app = staticModule.Create<TemplateApp>();

			CHECK(app.Get() == 3);
		}
	}

	TEST_CASE("Multiple Implementations")
	{
		const synodic::StaticModuleParameters params;
		const auto staticModule =
			synodic::CreateStaticModule<Interface, ImplementationA, ImplementationB>(params);

		SUBCASE("Template")
		{
			const auto app = staticModule.Create<TemplateApp>();

			CHECK(app.Get() == 3);
		}
	}

	TEST_CASE("Template Function")
	{
		const synodic::StaticModuleParameters params;
		const auto staticModule = synodic::CreateStaticModule<
			InterfaceWithTemplateFunction, ImplementationWithTemplateFunction>(params);

		SUBCASE("Template")
		{
			const auto app = staticModule.Create<TemplateFunctionApp>();

			CHECK(app.Get() == 3);
		}
	}
}