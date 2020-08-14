// Defines the module implementations for the test suite

#pragma once

#include "interface_types.h"
#include "synodic/composition/module.h"

class ImplementationC : public InterfaceB
{
public:
	constexpr ImplementationC() = default;

	template<typename T>
	[[nodiscard]] T Get() const
	{
		return T(3);
	}
};

class ImplementationCM :
	public synodic::ModuleImplementation<InterfaceB, ImplementationC>
{
public:
	constexpr ImplementationCM() = default;

	constexpr uint8_t Priority()
	{
		return 1;
	}
};

class ImplementationA : public InterfaceA
{
public:
	constexpr ImplementationA() = default;

	[[nodiscard]] int Get() const override
	{
		return 3;
	}
};

class ImplementationAM :
	public synodic::ModuleImplementation<InterfaceA, ImplementationA>
{
public:
	constexpr ImplementationAM() = default;

	constexpr uint8_t Priority()
	{
		return 1;
	}
};

class ImplementationB : public InterfaceA
{
public:
	constexpr ImplementationB() = default;

	[[nodiscard]] int Get() const override
	{
		return 4;
	}
};

class ImplementationBM :
	public synodic::ModuleImplementation<InterfaceA, ImplementationB>
{
public:
	constexpr ImplementationBM() = default;

	constexpr uint8_t Priority()
	{
		return 0;
	}
};