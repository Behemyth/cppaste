#include "doctest/doctest.h"
#include "types.h"


TEST_SUITE("Dynamic Polymorphic Injection")
{

	// Modules have the capability to provide an implementation to an interface with dynamic
	// inheritance

	TEST_CASE("Single Implementation")
	{
		const synodic::StaticModuleParameters params;
		const auto dynamicModule = synodic::CreateStaticModule<Interface, ImplementationA>(params);

		SUBCASE("value")
		{
			const auto app = dynamicModule.Create<ValueApp>();

			CHECK(app.Get() == 3);
		}

		SUBCASE("reference")
		{
			const auto app = dynamicModule.Create<ReferenceApp>();

			CHECK(app.Get() == 3);
		}

		SUBCASE("const reference")
		{
			const auto app = dynamicModule.Create<ConstReferenceApp>();

			CHECK(app.Get() == 3);
		}

		SUBCASE("pointer")
		{
			const auto app = dynamicModule.Create<PointerApp>();

			CHECK(app.Get() == 3);
		}

		SUBCASE("const pointer")
		{
			const auto app = dynamicModule.Create<ConstPointerApp>();

			CHECK(app.Get() == 3);
		}

		SUBCASE("universal reference")
		{
			const auto app = dynamicModule.Create<UniversalReferenceApp>();

			CHECK(app.Get() == 3);
		}
		
		SUBCASE("unique_ptr")
		{
			const auto app = dynamicModule.Create<UniquePtrApp>();

			CHECK(app.Get() == 3);
		}

		SUBCASE("shared_ptr")
		{
			const auto app = dynamicModule.Create<SharedPtrApp>();

			CHECK(app.Get() == 3);
		}

		SUBCASE("weak_ptr")
		{
			const auto app = dynamicModule.Create<WeakPtrApp>();

			CHECK(app.Get() == 3);
		}
	}

	TEST_CASE("Multiple Implementations")
	{
		const synodic::StaticModuleParameters params;
		const auto dynamicModule =
			synodic::CreateStaticModule<Interface, ImplementationA, ImplementationB>(params);

		SUBCASE("value")
		{
			const auto app = dynamicModule.Create<ValueApp>();

			CHECK(app.Get() == 3);
		}

		SUBCASE("reference")
		{
			const auto app = dynamicModule.Create<ReferenceApp>();

			CHECK(app.Get() == 3);
		}

		SUBCASE("const reference")
		{
			const auto app = dynamicModule.Create<ConstReferenceApp>();

			CHECK(app.Get() == 3);
		}

		SUBCASE("pointer")
		{
			const auto app = dynamicModule.Create<PointerApp>();

			CHECK(app.Get() == 3);
		}

		SUBCASE("const pointer")
		{
			const auto app = dynamicModule.Create<ConstPointerApp>();

			CHECK(app.Get() == 3);
		}

		SUBCASE("universal reference")
		{
			const auto app = dynamicModule.Create<UniversalReferenceApp>();

			CHECK(app.Get() == 3);
		}

		SUBCASE("unique_ptr")
		{
			const auto app = dynamicModule.Create<UniquePtrApp>();

			CHECK(app.Get() == 3);
		}

		SUBCASE("shared_ptr")
		{
			const auto app = dynamicModule.Create<SharedPtrApp>();

			CHECK(app.Get() == 3);
		}

		SUBCASE("weak_ptr")
		{
			const auto app = dynamicModule.Create<WeakPtrApp>();

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