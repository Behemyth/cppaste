// Defines the module implementations for the test suite

#pragma once

#include "../static_interface_types.h"
#include "synodic/composition/module.h"

class StaticImplementationC final :
	public synodic::StaticImplementation<StaticInterfaceA, StaticImplementationC>
{
public:
	constexpr StaticImplementationC() = default;

	[[nodiscard]] int Get() const;

	static constexpr uint8_t Priority()
	{
		return 5;
	}

	static constexpr bool Enabled()
	{
		return true;
	}
};