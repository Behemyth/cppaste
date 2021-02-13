#include "doctest/doctest.h"
#include "synodic/injection/dependency.h"


class Interface
{
public:
	virtual ~Interface() noexcept = default;

	virtual void TestFunction() = 0;
};

class Implementation final : public Interface
{
public:
	~Implementation() noexcept override = default;

	void TestFunction() override
	{
	}
};

TEST_SUITE("Dependency")
{

	TEST_CASE("Value Constructor")
	{
		cppaste::Dependency<int, int, cppaste::ScopeType::SHARED> dependency;
	}

	TEST_CASE("Derived Constructor")
	{
		cppaste::Dependency<Interface, Implementation, cppaste::ScopeType::SHARED> dependency;
	}
	
	
}