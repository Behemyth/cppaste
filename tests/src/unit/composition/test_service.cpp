#include <gtest/gtest.h>

import std;
import synodic.cppaste.composition;
import synodic.cppaste.test.calculator_fixtures;

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

	// Static service tests

	TEST(StaticService, BasicOperations)
	{
		SimpleCalcService service(SimpleCalculator{});
		ASSERT_EQ(service.Add(5, 10), 15);
		ASSERT_EQ(service.Multiply(3, 7), 21);
		EXPECT_EQ(service.GetName(), "SimpleCalculator");
	}

	TEST(StaticService, ConstexprService)
	{
		constexpr SimpleCalcService service(SimpleCalculator{});
		constexpr auto sum = service.Add(100, 200);
		constexpr auto product = service.Multiply(10, 10);

		ASSERT_EQ(sum, 300);
		ASSERT_EQ(product, 100);
	}

	TEST(StaticService, DifferentImplementations)
	{
		SimpleCalcService simple(SimpleCalculator{});
		AdvancedCalcService advanced(AdvancedCalculator{});

		ASSERT_EQ(simple.Add(5, 5), 10);
		ASSERT_EQ(advanced.Add(5, 5), 11);

		ASSERT_EQ(simple.Multiply(3, 3), 9);
		ASSERT_EQ(advanced.Multiply(3, 3), 18);
	}

	TEST(StaticService, ZeroCostAbstraction)
	{
		SimpleCalcService service(SimpleCalculator{});

		// All calls inline - no virtual dispatch overhead
		ASSERT_EQ(service.Add(10, 20), 30);
		ASSERT_EQ(service.Multiply(5, 6), 30);
		EXPECT_EQ(service.GetName(), "SimpleCalculator");
	}

	// Dynamic service tests - demonstrates single virtual dispatch architecture
	// DynamicCalculatorService<T> provides virtual overrides that call get_impl<T> directly
	// This eliminates the double-dispatch performance problem

	TEST(DynamicService, BasicOperations)
	{
		SimpleCalculatorService service(SimpleCalculator{});

		ASSERT_EQ(service.Add(5, 10), 15);
		ASSERT_EQ(service.Multiply(3, 7), 21);
		EXPECT_EQ(service.GetName(), "SimpleCalculator");
	}

	TEST(DynamicService, RuntimeImplementationSwap)
	{
		// Note: DynamicCalculatorService<T> is typed to a specific implementation.
		// To swap implementations at runtime, use different service instances.
		SimpleCalculatorService simple_service(SimpleCalculator{});
		AdvancedCalculatorService advanced_service(AdvancedCalculator{});

		// Verify each service works with its implementation
		ASSERT_EQ(simple_service.Add(5, 5), 10);
		ASSERT_EQ(simple_service.Multiply(3, 3), 9);
		EXPECT_EQ(simple_service.GetName(), "SimpleCalculator");

		ASSERT_EQ(advanced_service.Add(5, 5), 11);   // Bonus added
		ASSERT_EQ(advanced_service.Multiply(3, 3), 18); // Doubled
		EXPECT_EQ(advanced_service.GetName(), "AdvancedCalculator");
	}

	TEST(DynamicService, EmptyServiceAssignment)
	{
		SimpleCalculatorService service;

		// Assign implementation after construction
		service = SimpleCalculator{};

		ASSERT_EQ(service.Add(10, 20), 30);
		ASSERT_EQ(service.Multiply(4, 5), 20);
		EXPECT_EQ(service.GetName(), "SimpleCalculator");
	}

	TEST(DynamicService, CopySemantics)
	{
		SimpleCalculatorService service1(SimpleCalculator{});
		auto service2 = service1;

		ASSERT_EQ(service1.Add(2, 3), 5);
		ASSERT_EQ(service2.Add(2, 3), 5);

		// Both are independent copies of the same implementation type
		ASSERT_EQ(service1.Add(5, 5), 10);
		ASSERT_EQ(service2.Add(5, 5), 10);
	}

	TEST(DynamicService, MoveSemantics)
	{
		SimpleCalculatorService service1(SimpleCalculator{});
		auto service2 = std::move(service1);

		ASSERT_EQ(service2.Add(7, 8), 15);
		ASSERT_EQ(service2.Multiply(6, 4), 24);
	}

}
