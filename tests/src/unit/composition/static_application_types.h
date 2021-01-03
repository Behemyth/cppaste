// Defines the module implementations for the test suite

#pragma once

#include "static_interface_types.h"


template<typename InterfaceType = StaticInterfaceA>
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
