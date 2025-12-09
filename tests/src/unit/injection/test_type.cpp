#include <gtest/gtest.h>

import std;
import synodic.cppaste.injection;

namespace
{

TEST(Type, TypeConstruction)
{
	constexpr cppaste::Type<int> type;
	ASSERT_TRUE(true); // Test construction succeeded
}

}
