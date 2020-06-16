#pragma once

#include "interface.h"
#include "composition/module.h"

class ImplementationA : public DynamicInterface
{
public:
	ImplementationA()			= default;
	~ImplementationA() override = default;

	[[nodiscard]] int Get() override
	{
		return 3;
	}
};

class ImplementationAInjector : public synodic::Injector<DynamicInterface, ImplementationA>
{
public: 
	ImplementationAInjector() : Injector<DynamicInterface, ImplementationA>()
	{	
	}
	~ImplementationAInjector() override = default;

	bool Instantiate()
	{
		return true;
	}
};