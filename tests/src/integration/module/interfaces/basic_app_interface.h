#pragma once

#include <memory>

#include "basic_interface.h"

namespace test
{
	class BasicAppInterface
	{
	public:
		BasicAppInterface(std::unique_ptr<test::BasicInterface>&)
		{
		}

		virtual ~BasicAppInterface() = 0;

		virtual int Get() = 0;
	};
}
