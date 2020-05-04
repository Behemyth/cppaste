#pragma once

class BasicImplementation2 final : public synodic::Module<BasicImplementation, BasicInterface>
{

public:
	~BasicImplementation2() override = default;

	int Get() override
	{
		return 5;
	}
};
