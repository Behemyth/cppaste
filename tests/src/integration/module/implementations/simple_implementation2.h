#pragma once

class SimpleImplementation2 final :
	public synodic::Module<SimpleInterface, BasicImplementation>
{

public:
	~SimpleImplementation2() override = default;

	int Get() override
	{
		return 7;
	}
};
