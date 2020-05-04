#pragma once

class BasicImplementation final : public synodic::Module<BasicImplementation, BasicInterface>
{

public:
	~BasicImplementation() override = default;

	int Get() override
	{
		return 5;
	}
};
