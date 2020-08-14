// Defines the module interfaces for the test suite

#pragma once

#include "synodic/composition/module.h"

class InterfaceA
{
public:
	InterfaceA()		  = default;
	virtual ~InterfaceA() = default;

	[[nodiscard]] virtual int Get() const = 0;
};

class InterfaceAM final : synodic::ModuleInterface<InterfaceA, InterfaceAM>
{
};

class InterfaceB
{
public:
	InterfaceB()		  = default;
	virtual ~InterfaceB() = default;

	template<typename T>
	[[nodiscard]] int Get() const
	{
		return 3;
	}
};

class InterfaceBM final : synodic::ModuleInterface<InterfaceB, InterfaceBM>
{
};