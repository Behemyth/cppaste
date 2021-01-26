#include "doctest/doctest.h"

#include "synodic/injection/scope.h"

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


TEST_SUITE("Scope")
{

	TEST_CASE("Unique Create")
	{
		cppaste::UniqueScope unique;

		const auto result1 = unique.Create<int>();
		const auto result2 = unique.Create<int>();

		//CHECK(result1 != result2);
	}

	TEST_CASE("Shared Create")
	{
		cppaste::SharedScope shared;

		const auto result1 = shared.Create<int>();
		const auto result2 = shared.Create<int>();

		//CHECK(result1 == result2);
	}
	
}