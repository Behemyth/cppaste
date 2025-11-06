import std;
import synodic.honesty.test;
import synodic.cppaste.injection;

using namespace honesty::test::literals;

namespace
{
	honesty::test::Suite SUITE(
		"solver",
		[]() -> honesty::test::Generator
		{
			co_yield "solver_construction"_test = [](const honesty::test::Requirements& requirements)
			{
				cppaste::Solver<int> solver;
				requirements.Assert(true); // Test construction succeeded
			};
		});

	honesty::test::SuiteRegistrar _(SUITE);
}