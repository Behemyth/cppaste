#pragma once

#include <concepts>

#include "scope.h"

class Interface;

namespace cppaste
{

	// A wrapper around types that provide injection metadata
	template<typename Expected, typename Given, ScopeType Scope>
	class Dependency
	{

	public:
		consteval Dependency() noexcept = default;
	};

}
