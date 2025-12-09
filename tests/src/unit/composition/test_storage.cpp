#include <gtest/gtest.h>

import std;
import synodic.cppaste.composition;
import synodic.cppaste.test.calculator_fixtures;

using namespace test_fixtures;

namespace
{

// Static storage tests

TEST(StaticStorage, DirectAccess)
{
	cppaste::StaticStorage<SimpleCalculator> storage(SimpleCalculator {});

	const auto calc = storage.get();
	ASSERT_EQ(calc.Add(2, 3), 5);
	ASSERT_EQ(calc.Multiply(4, 5), 20);
	EXPECT_EQ(calc.GetName(), "SimpleCalculator");
}

TEST(StaticStorage, ConstexprSupport)
{
	constexpr cppaste::StaticStorage<SimpleCalculator> storage(SimpleCalculator {});
	constexpr auto result = storage.get().Add(10, 20);

	ASSERT_EQ(result, 30);
}

TEST(StaticStorage, PerfectForwarding)
{
	// Test that get() properly forwards const/non-const
	cppaste::StaticStorage<SimpleCalculator> storage(SimpleCalculator {});
	const auto& constStorage = storage;

	const auto mutableCalc = storage.get();
	const auto constCalc   = constStorage.get();

	ASSERT_EQ(mutableCalc.Add(1, 1), 2);
	ASSERT_EQ(constCalc.Add(1, 1), 2);
}

// Dynamic storage tests

TEST(DynamicStorage, DirectAccess)
{
	cppaste::DynamicStorage<ICalculator> storage(SimpleCalculator {});

	const auto* calc = storage.get_concept();
	ASSERT_EQ(calc->Add(2, 3), 5);
	ASSERT_EQ(calc->Multiply(4, 5), 20);
	EXPECT_EQ(calc->GetName(), "SimpleCalculator");
}

TEST(DynamicStorage, CopySemantics)
{
	cppaste::DynamicStorage<ICalculator> storage1(SimpleCalculator {});
	cppaste::DynamicStorage<ICalculator> storage2 = storage1;

	const auto* calc1 = storage1.get_concept();
	const auto* calc2 = storage2.get_concept();

	ASSERT_EQ(calc1->Add(2, 3), 5);
	ASSERT_EQ(calc2->Add(2, 3), 5);
	ASSERT_NE(calc1, calc2);  // Different objects
}

TEST(DynamicStorage, MoveSemantics)
{
	cppaste::DynamicStorage<ICalculator> storage1(SimpleCalculator {});
	cppaste::DynamicStorage<ICalculator> storage2 = std::move(storage1);

	ASSERT_TRUE(storage1.empty());
	ASSERT_FALSE(storage2.empty());

	const auto* calc = storage2.get_concept();
	ASSERT_EQ(calc->Add(7, 8), 15);
}

TEST(DynamicStorage, EmptyState)
{
	cppaste::DynamicStorage<ICalculator> storage;

	ASSERT_TRUE(storage.empty());

	EXPECT_THROW(storage.get_concept(), std::runtime_error);
}

TEST(DynamicStorage, MutableAccess)
{
	cppaste::DynamicStorage<ICalculator> storage(SimpleCalculator {});

	auto* calc = storage.get_concept();
	ASSERT_EQ(calc->Add(10, 20), 30);
	ASSERT_EQ(calc->Multiply(5, 6), 30);
}

TEST(DynamicStorage, RuntimeSwap)
{
	cppaste::DynamicStorage<ICalculator> storage(SimpleCalculator {});

	// Verify initial implementation
	auto* calc = storage.get_concept();
	ASSERT_EQ(calc->Add(5, 5), 10);
	ASSERT_EQ(calc->Multiply(3, 3), 9);
	EXPECT_EQ(calc->GetName(), "SimpleCalculator");

	// Swap to different implementation at runtime
	storage = cppaste::DynamicStorage<ICalculator>(AdvancedCalculator {});

	// Verify new implementation
	calc = storage.get_concept();
	ASSERT_EQ(calc->Add(5, 5), 11);  // Bonus added
	ASSERT_EQ(calc->Multiply(3, 3), 18);  // Doubled
	EXPECT_EQ(calc->GetName(), "AdvancedCalculator");
}

}
