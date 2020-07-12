#include "synodic/composition/package.h"
#include "doctest/doctest.h"
#include "types.h"


TEST_SUITE("Dynamic Polymorphic Injection")
{
	TEST_CASE("Single Implementation")
	{
		const synodic::StaticModuleParameters params;
		const auto dynamicModule = synodic::CreateStaticModule<Interface, ImplementationA>(params);

		const auto package = synodic::Package(dynamicModule);

		SUBCASE("unique_ptr")
		{
			const auto app = package.Create<UniquePtrApp>();

			CHECK(app.Get() == 3);
		}
	}

	TEST_CASE("Multiple Implementations")
	{
		const synodic::StaticModuleParameters params;
		const auto dynamicModule =
			synodic::CreateStaticModule<Interface, ImplementationA, ImplementationB>(params);

		const auto package = synodic::Package(dynamicModule);

		SUBCASE("unique_ptr")
		{
			const auto app = package.Create<UniquePtrApp>();

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

		const auto package = synodic::Package(staticModule);

		SUBCASE("Template")
		{
			const auto app = package.Create<TemplateApp>();

			CHECK(app.Get() == 3);
		}
	}

	TEST_CASE("Multiple Implementations")
	{
		const synodic::StaticModuleParameters params;
		const auto staticModule =
			synodic::CreateStaticModule<Interface, ImplementationA, ImplementationB>(params);

		const auto package = synodic::Package(staticModule);

		SUBCASE("Template")
		{
			const auto app = package.Create<TemplateApp>();

			CHECK(app.Get() == 3);
		}
	}
}