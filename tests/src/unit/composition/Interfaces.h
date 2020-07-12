#pragma once

#include "module.h"

// TODO: Is the Module inheritance really necessary?
class BasicInterface : synodic::Module<BasicInterface>
{

public:

	virtual ~BasicInterface() = 0;
	
	virtual int Get() = 0;
	
};