// Defines the module implementations for the test suite

#pragma once

#include "../static_interface_types.h"
#include "synodic/composition/module.h"


class StaticImplementationA final :
	public synodic::StaticImplementation<StaticInterfaceA, StaticImplementationA>
{
public:
	constexpr StaticImplementationA() = default;

	[[nodiscard]] int Get() const;

	static constexpr uint8_t Priority()
	{
		return 3;
	}

	static constexpr bool Enabled()
	{
		return true;
	}
};