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

TEST(Scope, UniqueCreate)
{
	cppaste::UniqueScope unique;

	const auto result1 = unique.Create<int>();
	const auto result2 = unique.Create<int>();

	ASSERT_NE(result1, nullptr);
	ASSERT_NE(result2, nullptr);
	EXPECT_NE(result1, result2); // Unique pointers should be different
}

TEST(Scope, SharedCreate)
{
	cppaste::SharedScope shared;

	const auto result1 = shared.Create<int>();
	const auto result2 = shared.Create<int>();

	ASSERT_NE(result1, nullptr);
	ASSERT_NE(result2, nullptr);
	// Note: Each call creates a new shared_ptr, so they won't be equal
}

}
