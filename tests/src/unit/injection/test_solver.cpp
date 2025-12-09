#include <gtest/gtest.h>

import std;
import synodic.cppaste.injection;

namespace
{

TEST(Solver, SolverConstruction)
{
	cppaste::Solver<int> solver;
	ASSERT_TRUE(true); // Test construction succeeded
}

}
