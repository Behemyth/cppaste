// Defines the final types for the test suite

#pragma once

#include <memory>

#include "implementation_types.h"

// T& - singleton scope
class ReferenceApp
{
public:
	explicit ReferenceApp(ImplementationA& implementation) : value_(implementation.Get())
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
	explicit UniquePtrApp(std::unique_ptr<Interface> implementation) : value_(implementation->Get())
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

template<typename InterfaceType = class Interface>
class TemplateApp
{
public:
	explicit TemplateApp(const InterfaceType& implementation) : value_(implementation.Get())
	{
	}
	~TemplateApp() = default;

	[[nodiscard]] int Get() const
	{
		return value_;
	}

private:
	int value_;
};

template<typename InterfaceType = class InterfaceWithTemplateFunction>
class TemplateFunctionApp
{
public:
	explicit TemplateFunctionApp(const InterfaceType& implementation) :
		value_(implementation.template Get<int>())
	{
	}
	~TemplateFunctionApp() = default;

	[[nodiscard]] int Get() const
	{
		return value_;
	}

private:
	int value_;
};