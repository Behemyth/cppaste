#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#include "module.h"
#include <memory>

#include "Implementations.h"
#include "Interfaces.h"

class App
{
public:
	App(std::unique_ptr<BasicInterface> module) : val(module->Get())
	{
	}

	int val;
};

class App2
{
public:
	App2(std::unique_ptr<BasicImplementation> module) : val(module->Get())
	{
	}

	int val;
};


TEST_CASE("Basic Static Linking")
{
	
	auto manager = synodic::ModuleManager<>();
	App app = manager.Create<App>();

	CHECK(app.val == 5);
	
}

TEST_CASE("Basic Dynamic Linking")
{

	auto manager = synodic::ModuleManager();

	// TODO: Write test
	
}


TEST_CASE("Basic Type Cast")
{
	
	auto manager = synodic::ModuleManager();
	App2 app = manager.Create<App2>();

	// TODO: Finish writing test.
	// TODO:The BasicImplementation should be the same object as the BasicInterface
	// TODO: Is the better option to have a type cast in each implementation manually by the user?

}