// Defines the module implementations for the test suite

#pragma once

#include "synodic/composition/module.h"
#include "interface_types.h"

class ImplementationWithTemplateFunction : public InterfaceWithTemplateFunction
{
public:
	ImplementationWithTemplateFunction()		   = default;
	~ImplementationWithTemplateFunction() override = default;

	template<typename T>
	[[nodiscard]] T Get() const
	{
		return T(3);
	}
};

template<>
class synodic::
	ImplementationConfiguration<InterfaceWithTemplateFunction, ImplementationWithTemplateFunction> :
	public ModuleConfig<InterfaceWithTemplateFunction>
{
public:
	ImplementationConfiguration()  = default;
	~ImplementationConfiguration() = default;

	constexpr uint8_t Priority()
	{
		return 1;
	}
};

class ImplementationA : public Interface
{
public:
	ImplementationA()			= default;
	~ImplementationA() override = default;

	[[nodiscard]] int Get() const override
	{
		return 3;
	}
};

template<>
class synodic::ImplementationConfiguration<Interface, ImplementationA> :
	public ModuleConfig<Interface>
{
public:
	ImplementationConfiguration()  = default;
	~ImplementationConfiguration() = default;

	constexpr uint8_t Priority()
	{
		return 1;
	}
};

class ImplementationB : public Interface
{
public:
	ImplementationB()			= default;
	~ImplementationB() override = default;

	[[nodiscard]] int Get() const override
	{
		return 4;
	}
};

template<>
class synodic::ImplementationConfiguration<Interface, ImplementationB> :
	public ModuleConfig<Interface>
{
public:
	ImplementationConfiguration()  = default;
	~ImplementationConfiguration() = default;

	constexpr uint8_t Priority()
	{
		return 0;
	}
};