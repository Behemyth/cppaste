#pragma once

#include "synodic/type_traits.h"

namespace cppaste
{

	// The solver routines construct a type from configuration data
	template<typename T>
	class Solver
	{
	public:

	private:
		using ConstructorParameters = synodic::ConstructorParameters<T>;
	};

}
