#include "composition/module.h"
#include "composition/package.h"
#include "doctest/doctest.h"
#include "implementations/basic_app_implementation.h"
#include "interfaces/basic_interface.h"


TEST_CASE("Static Injection")
{

	//const synodic::Module<test::BasicImplementation> staticModule;
	//
	//synodic::Package testPackage(staticModule);

	//const auto app = testPackage.Create<test::BasicAppImplementation>();

	//CHECK(app.Get() == 5);
}

TEST_CASE("Static Polymorphic Injection")
{	
	// TODO: Write test
}

TEST_CASE("Dynamic Polymorphic Injection")
{
//	const synodic::Module<test::BasicInterface> dynamicModule;
//
//	synodic::Package testPackage(dynamicModule);
//	
//	test::BasicAppImplementation app = testPackage.Create<BasicAppImplementation>();
//
//	CHECK(app->Get() == 5);
}

TEST_CASE("Implicit Dynamic Linking")
{
	// TODO: Write test - Issue #10 
}

TEST_CASE("Explicit Dynamic Linking")
{
	// TODO: Write test - Issue #10 
}

TEST_CASE("Type Cast")
{
	 synodic::Module<test::BasicInterface> staticModule;
	
	 synodic::Package testPackage(staticModule);

	 const auto app = testPackage.Create<test::BasicAppImplementation>();

	 CHECK(app.Get() == 5);
}

TEST_CASE("Implementation Swap")
{
	// TODO: Write test - Issue #11
}