#include <gtest/gtest.h>

import std;
import synodic.cppaste.injection;

namespace
{

TEST(Type, TypeConstruction)
{
	[[maybe_unused]] constexpr cppaste::Type<int> type;
	ASSERT_TRUE(true); // Test construction succeeded
}

}
