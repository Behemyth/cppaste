// Defines the module implementations for the test suite

#pragma once

#include "dynamic_implementation_types.h"

// T& - singleton scope
class ReferenceApp
{
public:
	explicit ReferenceApp(DynamicImplementationA& implementation) : value_(implementation.Get())
	{
	}
	~ReferenceApp() = default;

	[[nodiscard]] int Get() const
	{
		return value_;
	}

private:
	int value_;
};

// std::unique_ptr - unique scope
class UniquePtrApp
{
public:
	explicit UniquePtrApp(const std::unique_ptr<DynamicInterfaceA> implementation) : value_(implementation->Get())
	{
	}
	~UniquePtrApp() = default;

	[[nodiscard]] int Get() const
	{
		return value_;
	}

private:
	int value_;
};