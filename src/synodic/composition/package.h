#pragma once

#include <boost/di.hpp>

#include "module.h"

namespace synodic
{

	///--------------------------------------------------------------------------------
	/// @brief Manages multiple modules
	///--------------------------------------------------------------------------------
	
	template<typename... Modules>
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
	template<typename... Modules>
	constexpr Package<Modules...>::Package(const Modules&... modules) :
		modules_(std::forward_as_tuple(modules)...)
	{
	}

	// Create injector from all module types
	template<typename... Modules>
	constexpr auto Package<Modules...>::Inject() const
	{
		return std::apply(
			[](auto&... module) {
				return boost::di::make_injector(module.Inject()...);
			},
			modules_);
	}
	
	// Create type
	template<typename... Modules>
	template<typename T>
	constexpr auto Package<Modules...>::Create() const
	{
		return Inject().template create<T>();
	}

	// Create templated type
	template<typename... Modules>
	template<template<typename...> typename T>
	constexpr auto Package<Modules...>::Create() const
	{
		return Inject().template create<T>();
	}

}
