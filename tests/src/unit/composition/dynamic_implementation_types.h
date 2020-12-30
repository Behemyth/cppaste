// Defines the module implementations for the test suite

#pragma once

#include "dynamic_interface_types.h"
#include "synodic/composition/module.h"

class DynamicImplementationA final : public synodic::DynamicImplementation<DynamicInterfaceA, DynamicImplementationA>
{
public:
	constexpr DynamicImplementationA() = default;

	[[nodiscard]] int Get() const override
	{
		return 3;
	}

	static constexpr uint8_t Priority()
	{
		return 1;
	}
};

class DynamicImplementationB final : public synodic::DynamicImplementation<DynamicInterfaceA, DynamicImplementationB>
{
public:
	constexpr DynamicImplementationB() = default;

	[[nodiscard]] int Get() const override
	{
		return 4;
	}

	static constexpr uint8_t Priority()
	{
		return 0;
	}
};