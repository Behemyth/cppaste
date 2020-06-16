#pragma once

#include "interface.h"
#include "composition/module.h"

class ImplementationB : public DynamicInterface
{
public:
	ImplementationB()			= default;
	~ImplementationB() override = default;

	[[nodiscard]] int Get() override
	{
		return 4;
	}
};

class ImplementationBInjector : public synodic::Injector<DynamicInterface, ImplementationB>
{
public:
	ImplementationBInjector() : Injector<DynamicInterface, ImplementationB>()
	{
	}
	~ImplementationBInjector() override = default;

	bool Instantiate() override
	{
		return true;
	}
};