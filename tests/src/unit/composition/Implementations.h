#pragma once

#include "Interfaces.h"

class BasicImplementation final : public BasicInterface
{

public:

	~BasicImplementation() override = default;
	
	int Get() override
	{
		return 5;
	}
};
