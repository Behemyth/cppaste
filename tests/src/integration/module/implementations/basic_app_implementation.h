#pragma once

#include "basic_implementation.h"

namespace test
{
	class BasicAppImplementation final
	{
	public:
		explicit BasicAppImplementation(BasicImplementation& basicImplementation) :
			value_(basicImplementation.Get())
		{
		}

		~BasicAppImplementation() = default;

		[[nodiscard]] int Get() const
		{
			return value_;
		}

	private:
		int value_;
	};
}