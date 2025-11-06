import std;
import synodic.honesty.test;
import synodic.cppaste.injection;

using namespace honesty::test::literals;

namespace
{
	honesty::test::Suite SUITE(
		"type",
		[]() -> honesty::test::Generator
		{
			co_yield "type_construction"_test = [](const honesty::test::Requirements& requirements)
			{
				constexpr cppaste::Type<int> type;
				requirements.Assert(true); // Test construction succeeded
			};
		});

	honesty::test::SuiteRegistrar _(SUITE);
}