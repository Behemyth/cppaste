import std;
import synodic.honesty.test;
import synodic.cppaste.injection;

using namespace honesty::test::literals;

namespace
{
	honesty::test::Suite SUITE(
		"injector",
		[]() -> honesty::test::Generator
		{
			co_yield "with_dependency"_test = [](const honesty::test::Requirements& requirements)
			{
				// TODO: Implement when injector is ready
				// constexpr cppaste::Type<int> intType;
				// cppaste::Injector();
				requirements.Assert(true);
			};

			// TODO: Add more tests when implementation is complete
			// co_yield "with_injector"_test = [](const honesty::test::Requirements& requirements)
			// {
			//     cppaste::Injector();
			// };

			// co_yield "with_dependency_and_injector"_test = [](const honesty::test::Requirements& requirements)
			// {
			//     cppaste::Injector();
			// };
		});

	honesty::test::SuiteRegistrar _(SUITE);
}
