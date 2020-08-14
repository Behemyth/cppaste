#include "synodic/composition/package.h"
#include "doctest/doctest.h"
#include "types.h"


TEST_SUITE("Dynamic Polymorphic Injection")
{
	TEST_CASE("Single Implementation")
	{
		const synodic::ModuleParameters params;
		const auto module = synodic::CreateModule<InterfaceAM, ImplementationAM>(params);

		const auto package = synodic::Package(module);

		SUBCASE("unique_ptr")
		{
			const auto app = package.Create<UniquePtrApp>();

			CHECK(app.Get() == 3);
		}
	}

	TEST_CASE("Multiple Implementations")
	{
		const synodic::ModuleParameters params;
		const auto module =
			synodic::CreateModule<InterfaceAM, ImplementationAM, ImplementationBM>(params);

		const auto package = synodic::Package(module);

		SUBCASE("unique_ptr")
		{
			const auto app = package.Create<UniquePtrApp>();

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

		const auto package = synodic::Package(module);

		SUBCASE("Template")
		{
			const auto app = package.Create<TemplateApp>();

			CHECK(app.Get() == 3);
		}
	}

	TEST_CASE("Multiple Implementations")
	{
		const synodic::ModuleParameters params;
		const auto module =
			synodic::CreateModule<InterfaceAM, ImplementationAM, ImplementationBM>(params);

		const auto package = synodic::Package(module);

		SUBCASE("Template")
		{
			const auto app = package.Create<TemplateApp>();

			CHECK(app.Get() == 3);
		}
	}
}