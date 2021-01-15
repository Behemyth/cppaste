#pragma once

#include "module.h"

namespace cppaste
{

	///--------------------------------------------------------------------------------
	/// @brief Manages multiple modules
	///--------------------------------------------------------------------------------

	template<std::derived_from<ModuleBase>... Modules>
	class Package
	{

	public:
		explicit constexpr Package(const Modules&... modules);

		template<typename T>
		[[nodiscard]] constexpr auto Create() const;

		template<template<typename...> typename T>
		[[nodiscard]] constexpr auto Create() const;

	private:
		[[nodiscard]] constexpr auto Inject() const;

		std::tuple<const Modules&...> modules_;
	};


	// TODO: Constrain Module type
	template<std::derived_from<ModuleBase>... Modules>
	constexpr Package<Modules...>::Package(const Modules&... modules) : modules_(modules...)
	{
	}

	// Create injector from all module types
	template<std::derived_from<ModuleBase>... Modules>
	constexpr auto Package<Modules...>::Inject() const
	{
		 return std::apply(
			[](auto&... synodicModule) {
				// TODO: Fix Injector type, possibly with factory?
				return Injector<bool>(synodicModule.Inject()...);
			},
			modules_);
	}

	// Create type
	template<std::derived_from<ModuleBase>... Modules>
	template<typename T>
	constexpr auto Package<Modules...>::Create() const
	{
		 return Inject().template Create<T>();
	}

	// Create templated type
	template<std::derived_from<ModuleBase>... Modules>
	template<template<typename...> typename T>
	constexpr auto Package<Modules...>::Create() const
	{
		 return Inject().template Create<T>();
	}

}
