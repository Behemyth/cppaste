#pragma once

#include <memory>

#include "basic_interface.h"

class BasicAppInterface
{
public:
	
	BasicAppInterface(std::unique_ptr<BasicModule>& module)
	{
	}

	virtual ~BasicAppInterface() = 0;

	virtual int Get() = 0;
};
