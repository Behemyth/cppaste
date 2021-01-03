// Defines the module implementations for the test suite

#pragma once

#include "../static_interface_types.h"
#include "synodic/composition/module.h"


class StaticImplementationB final :
	public synodic::StaticImplementation<StaticInterfaceB, StaticImplementationB>
{
public:
	constexpr StaticImplementationB() = default;


	[[nodiscard]] int Get() const;

	static constexpr uint8_t Priority()
	{
		return 2;
	}

	static constexpr bool Enabled()
	{
		return true;
	}
};