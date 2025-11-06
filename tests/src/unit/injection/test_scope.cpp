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
		"scope",
		[]() -> honesty::test::Generator
		{
			co_yield "unique_create"_test = [](const honesty::test::Requirements& requirements)
			{
				cppaste::UniqueScope unique;

				const auto result1 = unique.Create<int>();
				const auto result2 = unique.Create<int>();

				requirements.Assert(result1 != nullptr);
				requirements.Assert(result2 != nullptr);
				requirements.Expect(result1 != result2); // Unique pointers should be different
			};

			co_yield "shared_create"_test = [](const honesty::test::Requirements& requirements)
			{
				cppaste::SharedScope shared;

				const auto result1 = shared.Create<int>();
				const auto result2 = shared.Create<int>();

				requirements.Assert(result1 != nullptr);
				requirements.Assert(result2 != nullptr);
				// Note: Each call creates a new shared_ptr, so they won't be equal
			};
		});

	honesty::test::SuiteRegistrar _(SUITE);
}