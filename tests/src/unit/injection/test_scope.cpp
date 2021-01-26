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

		auto result1 = unique.Create<int>();
		auto result2 = unique.Create<int>();

		CHECK(result1 != result2);
	}

	TEST_CASE("Shared Create")
	{
		cppaste::SharedScope shared;

		auto result1 = shared.Create<int>();
		auto result2 = shared.Create<int>();

		CHECK(result1 == result2);
	}
	
}