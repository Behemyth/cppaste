#pragma once

#include <concepts>

#include "scope.h"
#include "dependency.h"

namespace cppaste
{

	// A wrapper around types that provide injection metadata
	template<typename T, ScopeType Scope = ScopeType::SHARED>
	class Type
	{

	public:
		consteval Type() noexcept = default;


		template<std::derived_from<T> BindType>
		[[nodiscard]] consteval auto Bind() const noexcept;
	};


	template<typename T, ScopeType Scope>
	template<std::derived_from<T> BindType>
	consteval auto Type<T, Scope>::Bind() const noexcept
	{
		return Dependency<T, BindType, Scope>(); 
	}

}
