#pragma once

class DynamicInterface
{
public:
	DynamicInterface()			= default;
	virtual ~DynamicInterface() = default;

	virtual int Get() = 0;
};
