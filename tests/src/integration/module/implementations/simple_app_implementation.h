#pragma once

class AppImplementation final : public synodic::Module<AppImplementation, AppInterface>
{
public:
	AppImplementation(std::unique_ptr<BasicInterface>& module) :
		AppInterface(module), value_(module->Get())
	{
	}

	~AppImplementation() override = default;

	int Get() override
	{
		return value_;
	}

private:
	int value_;
};