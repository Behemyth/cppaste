#pragma once

#include "composition/module.h"

namespace test
{

	concept basic_interface = requires
	{
		true;
	};

	class BasicInterface
	{
		
	};
	
	//BasicModule<Implementation>
	template<basic_interface T>
	class BasicModule : synodic::Module<T, BasicInterface>
	{
	};
	

	


}
