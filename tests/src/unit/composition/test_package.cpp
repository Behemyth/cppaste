#include "doctest/doctest.h"
#include "synodic/composition/package.h"
#include "types.h"


TEST_SUITE("Polymorphic Injection")
{
	TEST_CASE("Single Modules")
	{
		const synodic::ModuleParameters params;
		const auto module = synodic::CreateModule<InterfaceAM, ImplementationAM>(params);

		const auto package = synodic::Package(module);
		const auto app	   = package.Create<UniquePtrApp>();

		CHECK(app.Get() == 3);
	}

	TEST_CASE("Mutliple Modules")
	{
		const synodic::ModuleParameters moduleAParames;
		const auto moduleA = synodic::CreateModule<InterfaceAM, ImplementationAM>(moduleAParames);

		const synodic::ModuleParameters moduleBParams;
		const auto moduleB = synodic::CreateModule<InterfaceBM, ImplementationCM>(moduleBParams);
		
		const auto package = synodic::Package(moduleA, moduleB);

		SUBCASE("unique_ptr")
		{
			const auto app = package.Create<UniquePtrApp>();

			CHECK(app.Get() == 3);
		}
	}
}