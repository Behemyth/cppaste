#include <memory>

#include "composition/module.h"
#include "composition/package.h"
#include "doctest/doctest.h"
#include "implementations/basic_implementation.h"
#include "interfaces/basic_interface.h"

TEST_CASE("Static Injection")
{

	Module<BasicImplementation> doot;
	
	synodic::Package testPackage(doot);

	
	std::unique_ptr<AppInterface> app = synodic::CreateModule<AppInterface>();

	// TODO: Write test
	REQUIRE(app->Get() == 5);
}

TEST_CASE("Static Polymorphic Injection")
{
	Module<test::BasicInterface> doot;
	
	// TODO: Write test
}

TEST_CASE("Dynamic Polymorphic Injection")
{
	Module<test::BasicInterface> doot;

	synodic::Package testPackage(doot);
	
	// TODO: Write test
}

TEST_CASE("Implicit Dynamic Linking")
{
	// TODO: Write test
}

TEST_CASE("Explicit Dynamic Linking")
{
	// TODO: Write test
}

TEST_CASE("Type Cast")
{
	// TODO: Write test
	// TODO: The BasicImplementation should be the same object as the BasicInterface
	// TODO: Is the better option to have a type cast in each implementation manually by the user?
}

TEST_CASE("Implementation Swap")
{
	// TODO: Write test
}