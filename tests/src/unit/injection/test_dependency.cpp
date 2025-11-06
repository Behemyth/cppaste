import std;
import synodic.honesty.test;
import synodic.cppaste.injection;

using namespace honesty::test::literals;

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

	honesty::test::Suite SUITE(
		"dependency",
		[]() -> honesty::test::Generator
		{
			co_yield "value_constructor"_test = [](const honesty::test::Requirements& requirements)
			{
				cppaste::Dependency<int, int, cppaste::ScopeType::SHARED> dependency;
				requirements.Assert(true);	// Test construction succeeded
			};

			co_yield "derived_constructor"_test = [](const honesty::test::Requirements& requirements)
			{
				cppaste::Dependency<Interface, Implementation, cppaste::ScopeType::SHARED> dependency;
				requirements.Assert(true);	// Test construction succeeded
			};
		});

	honesty::test::SuiteRegistrar _(SUITE);
}
