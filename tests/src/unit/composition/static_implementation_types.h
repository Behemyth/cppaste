// Defines the module implementations for the test suite

#pragma once

#include "static_interface_types.h"
#include "synodic/composition/module.h"


class StaticImplementationA final :
	public synodic::StaticImplementation<StaticInterfaceA, StaticImplementationA>
{
public:
	constexpr StaticImplementationA() = default;

	[[nodiscard]] int Get() const
	{
		return 3;
	}

	static constexpr uint8_t Priority()
	{
		return 3;
	}
};


class StaticImplementationB final :
	public synodic::StaticImplementation<StaticInterfaceB, StaticImplementationB>
{
public:
	constexpr StaticImplementationB() = default;


	[[nodiscard]] int Get() const
	{
		return 2;
	}

	static constexpr uint8_t Priority()
	{
		return 2;
	}
};