import std;
import synodic.honesty.test;
import synodic.cppaste.composition;
import synodic.cppaste.test.calculator_fixtures;

using namespace honesty::test::literals;
using namespace test_fixtures;

namespace
{
	// Static service tests using Service<Interface, Impl> directly
	// These demonstrate zero-cost abstraction with compile-time polymorphism
	// TODO(reflection): Auto-generate ICalculatorStatic<Derived> to eliminate manual CRTP wrapper

	// Manual CRTP wrapper for SimpleCalculator - what reflection will auto-generate
	struct SimpleCalcService : ICalculatorStatic<SimpleCalcService>
	{
		cppaste::Service<ICalculator, SimpleCalculator> service_;
		constexpr SimpleCalcService(SimpleCalculator impl) : service_(std::move(impl)) {}
		constexpr auto GetStorage(this auto&& self) -> decltype(auto)
		{
			return std::forward<decltype(self)>(self).service_.GetStorage();
		}
	};

	// Manual CRTP wrapper for AdvancedCalculator
	struct AdvancedCalcService : ICalculatorStatic<AdvancedCalcService>
	{
		cppaste::Service<ICalculator, AdvancedCalculator> service_;
		constexpr AdvancedCalcService(AdvancedCalculator impl) : service_(std::move(impl)) {}
		constexpr auto GetStorage(this auto&& self) -> decltype(auto)
		{
			return std::forward<decltype(self)>(self).service_.GetStorage();
		}
	};

	honesty::test::Suite STATIC_SERVICE_SUITE(
		"static_service",
		[]() -> honesty::test::Generator
		{
			co_yield "basic_operations"_test = [](const honesty::test::Requirements& requirements)
			{
				SimpleCalcService service(SimpleCalculator{});
				requirements.Assert(service.Add(5, 10) == 15);
				requirements.Assert(service.Multiply(3, 7) == 21);
				requirements.Expect(service.GetName() == "SimpleCalculator");
			};

			co_yield "constexpr_service"_test = [](const honesty::test::Requirements& requirements)
			{
				constexpr SimpleCalcService service(SimpleCalculator{});
				constexpr auto sum = service.Add(100, 200);
				constexpr auto product = service.Multiply(10, 10);

				requirements.Assert(sum == 300);
				requirements.Assert(product == 100);
			};

			co_yield "different_implementations"_test = [](const honesty::test::Requirements& requirements)
			{
				SimpleCalcService simple(SimpleCalculator{});
				AdvancedCalcService advanced(AdvancedCalculator{});

				requirements.Assert(simple.Add(5, 5) == 10);
				requirements.Assert(advanced.Add(5, 5) == 11);

				requirements.Assert(simple.Multiply(3, 3) == 9);
				requirements.Assert(advanced.Multiply(3, 3) == 18);
			};

			co_yield "zero_cost_abstraction"_test = [](const honesty::test::Requirements& requirements)
			{
				SimpleCalcService service(SimpleCalculator{});

				// All calls inline - no virtual dispatch overhead
				requirements.Assert(service.Add(10, 20) == 30);
				requirements.Assert(service.Multiply(5, 6) == 30);
				requirements.Expect(service.GetName() == "SimpleCalculator");
			};
		});

	honesty::test::SuiteRegistrar _1(STATIC_SERVICE_SUITE);

	// Dynamic service tests - demonstrates single virtual dispatch architecture
	// DynamicCalculatorService<T> provides virtual overrides that call get_impl<T> directly
	// This eliminates the double-dispatch performance problem

	honesty::test::Suite DYNAMIC_SERVICE_SUITE(
		"dynamic_service",
		[]() -> honesty::test::Generator
		{
			co_yield "basic_operations"_test = [](const honesty::test::Requirements& requirements)
			{
				SimpleCalculatorService service(SimpleCalculator{});

				requirements.Assert(service.Add(5, 10) == 15);
				requirements.Assert(service.Multiply(3, 7) == 21);
				requirements.Expect(service.GetName() == "SimpleCalculator");
			};

			co_yield "runtime_implementation_swap"_test = [](const honesty::test::Requirements& requirements)
			{
				SimpleCalculatorService service(SimpleCalculator{});

				// Verify initial implementation
				requirements.Assert(service.Add(5, 5) == 10);
				requirements.Assert(service.Multiply(3, 3) == 9);
				requirements.Expect(service.GetName() == "SimpleCalculator");

				// Swap to different implementation at runtime
				service = AdvancedCalculator{};

				// Verify new implementation
				requirements.Assert(service.Add(5, 5) == 11);   // Bonus added
				requirements.Assert(service.Multiply(3, 3) == 18); // Doubled
				requirements.Expect(service.GetName() == "AdvancedCalculator");
			};

			co_yield "empty_service_assignment"_test = [](const honesty::test::Requirements& requirements)
			{
				SimpleCalculatorService service;

				// Assign implementation after construction
				service = SimpleCalculator{};

				requirements.Assert(service.Add(10, 20) == 30);
				requirements.Assert(service.Multiply(4, 5) == 20);
				requirements.Expect(service.GetName() == "SimpleCalculator");
			};

			co_yield "copy_semantics"_test = [](const honesty::test::Requirements& requirements)
			{
				SimpleCalculatorService service1(SimpleCalculator{});
				auto service2 = service1;

				requirements.Assert(service1.Add(2, 3) == 5);
				requirements.Assert(service2.Add(2, 3) == 5);

				// Modify one, shouldn't affect the other (deep copy)
				service2 = AdvancedCalculator{};
				requirements.Assert(service1.Add(5, 5) == 10);  // Still simple
				requirements.Assert(service2.Add(5, 5) == 11);  // Now advanced
			};

			co_yield "move_semantics"_test = [](const honesty::test::Requirements& requirements)
			{
				SimpleCalculatorService service1(SimpleCalculator{});
				auto service2 = std::move(service1);

				requirements.Assert(service2.Add(7, 8) == 15);
				requirements.Assert(service2.Multiply(6, 4) == 24);
			};
		});

	honesty::test::SuiteRegistrar _2(DYNAMIC_SERVICE_SUITE);
}

