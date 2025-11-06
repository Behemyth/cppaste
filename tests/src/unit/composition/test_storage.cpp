import std;
import synodic.honesty.test;
import synodic.cppaste.composition;
import synodic.cppaste.test.calculator_fixtures;

using namespace honesty::test::literals;
using namespace test_fixtures;

namespace
{
	honesty::test::Suite STATIC_STORAGE_SUITE(
		"static_storage",
		[]() -> honesty::test::Generator
		{
			co_yield "direct_access"_test = [](const honesty::test::Requirements& requirements)
			{
				cppaste::StaticStorage<SimpleCalculator> storage(SimpleCalculator {});

				const auto calc = storage.get();
				requirements.Assert(calc.Add(2, 3) == 5);
				requirements.Assert(calc.Multiply(4, 5) == 20);
				requirements.Expect(calc.GetName() == "SimpleCalculator");
			};
			co_yield "constexpr_support"_test = [](const honesty::test::Requirements& requirements)
			{
				constexpr cppaste::StaticStorage<SimpleCalculator> storage(SimpleCalculator {});
				constexpr auto result = storage.get().Add(10, 20);

				requirements.Assert(result == 30);
			};

			co_yield "perfect_forwarding"_test = [](const honesty::test::Requirements& requirements)
			{
				// Test that get() properly forwards const/non-const
				cppaste::StaticStorage<SimpleCalculator> storage(SimpleCalculator {});
				const auto& constStorage = storage;

				const auto mutableCalc = storage.get();
				const auto constCalc   = constStorage.get();

				requirements.Assert(mutableCalc.Add(1, 1) == 2);
				requirements.Assert(constCalc.Add(1, 1) == 2);
			};
		});

	honesty::test::SuiteRegistrar _1(STATIC_STORAGE_SUITE);

	honesty::test::Suite DYNAMIC_STORAGE_SUITE(
		"dynamic_storage",
		[]() -> honesty::test::Generator
		{
			co_yield "direct_access"_test = [](const honesty::test::Requirements& requirements)
			{
				cppaste::DynamicStorage<ICalculator> storage(SimpleCalculator {});

				const auto* calc = storage.get_concept();
				requirements.Assert(calc->Add(2, 3) == 5);
				requirements.Assert(calc->Multiply(4, 5) == 20);
				requirements.Expect(calc->GetName() == "SimpleCalculator");
			};

			co_yield "copy_semantics"_test = [](const honesty::test::Requirements& requirements)
			{
				cppaste::DynamicStorage<ICalculator> storage1(SimpleCalculator {});
				cppaste::DynamicStorage<ICalculator> storage2 = storage1;

				const auto* calc1 = storage1.get_concept();
				const auto* calc2 = storage2.get_concept();

				requirements.Assert(calc1->Add(2, 3) == 5);
				requirements.Assert(calc2->Add(2, 3) == 5);
				requirements.Assert(calc1 != calc2);  // Different objects
			};

			co_yield "move_semantics"_test = [](const honesty::test::Requirements& requirements)
			{
				cppaste::DynamicStorage<ICalculator> storage1(SimpleCalculator {});
				cppaste::DynamicStorage<ICalculator> storage2 = std::move(storage1);

				requirements.Assert(storage1.empty());
				requirements.Assert(!storage2.empty());

				const auto* calc = storage2.get_concept();
				requirements.Assert(calc->Add(7, 8) == 15);
			};

			co_yield "empty_state"_test = [](const honesty::test::Requirements& requirements)
			{
				cppaste::DynamicStorage<ICalculator> storage;

				requirements.Assert(storage.empty());

				bool threw = false;
				try
				{
					storage.get_concept();
				}
				catch (const std::runtime_error&)
				{
					threw = true;
				}
				requirements.Assert(threw);
			};

			co_yield "mutable_access"_test = [](const honesty::test::Requirements& requirements)
			{
				cppaste::DynamicStorage<ICalculator> storage(SimpleCalculator {});

				auto* calc = storage.get_concept();
				requirements.Assert(calc->Add(10, 20) == 30);
				requirements.Assert(calc->Multiply(5, 6) == 30);
			};

			co_yield "runtime_swap"_test = [](const honesty::test::Requirements& requirements)
			{
				cppaste::DynamicStorage<ICalculator> storage(SimpleCalculator {});

				// Verify initial implementation
				auto* calc = storage.get_concept();
				requirements.Assert(calc->Add(5, 5) == 10);
				requirements.Assert(calc->Multiply(3, 3) == 9);
				requirements.Expect(calc->GetName() == "SimpleCalculator");

				// Swap to different implementation at runtime
				storage = cppaste::DynamicStorage<ICalculator>(AdvancedCalculator {});

				// Verify new implementation
				calc = storage.get_concept();
				requirements.Assert(calc->Add(5, 5) == 11);  // Bonus added
				requirements.Assert(calc->Multiply(3, 3) == 18);  // Doubled
				requirements.Expect(calc->GetName() == "AdvancedCalculator");
			};
		});

	honesty::test::SuiteRegistrar _2(DYNAMIC_STORAGE_SUITE);
}
