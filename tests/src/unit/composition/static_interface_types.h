// Defines the module interfaces for the test suite

#pragma once

#include "synodic/composition/module.h"

template<typename T>
concept interface_a_concept = requires(T object)
{
	{
		object.Get()
	}
	->std::same_as<int>;
};

class StaticInterfaceA : public cppaste::StaticInterface<StaticInterfaceA>
{
public:
	constexpr StaticInterfaceA() = default;

	template<typename T>
	static constexpr bool ValidateModule()
	{
		return interface_a_concept<T>;
	}
};

template<typename T>
concept interface_b_concept = requires(T object)
{
	{
		object.Get()
	}
	->std::same_as<int>;
};

class StaticInterfaceB : public cppaste::StaticInterface<StaticInterfaceB>
{
public:
	constexpr StaticInterfaceB() = default;


	template<typename T>
	static constexpr bool ValidateModule()
	{
		return interface_b_concept<T>;
	}
};