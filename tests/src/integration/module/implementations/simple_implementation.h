#pragma once

class SimpleImplementation final : public synodic::Module<SimpleImplementation, SimpleInterface>
{

public:
	~SimpleImplementation() override = default;

	int Get() override
	{
		return 6;
	}
};