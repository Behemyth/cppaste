#pragma once

#include <memory>

#include "basic_interface.h"

class SimpleAppInterface
{
public:
	
	SimpleAppInterface(std::unique_ptr<BasicModule>& module)
	{
	}

	virtual ~SimpleAppInterface() = 0;

	virtual int Get() = 0;
};
