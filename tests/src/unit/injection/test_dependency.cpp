#include <gtest/gtest.h>

import std;
import synodic.cppaste.injection;

namespace
{

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

TEST(Dependency, ValueConstructor)
{
	cppaste::Dependency<int, int, cppaste::ScopeType::SHARED> dependency;
	ASSERT_TRUE(true);	// Test construction succeeded
}

TEST(Dependency, DerivedConstructor)
{
	cppaste::Dependency<Interface, Implementation, cppaste::ScopeType::SHARED> dependency;
	ASSERT_TRUE(true);	// Test construction succeeded
}

}
