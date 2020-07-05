#pragma once

#include <boost/di.hpp>

#include "module.h"

namespace synodic
{

	template<typename... Modules>
	class Package
	{

	public:
		// TODO: Constrain type
		explicit constexpr Package(const Modules&... modules) :
			modules_(std::forward_as_tuple(modules)...)
		{
		}

		[[nodiscard]] constexpr auto Inject() const
		{
			return std::apply(
				[](auto&... module) {
					return boost::di::make_injector(module.Inject()...);
				},
				modules_);
		}


		template<typename T>
		[[nodiscard]] constexpr auto Create() const;

		template<template<typename...> typename T>
		[[nodiscard]] constexpr auto Create() const;

	private:
		std::tuple<const Modules&...> modules_;
	};

	template<typename... Modules>
	template<typename T>
	constexpr auto Package<Modules...>::Create() const
	{
		return Inject().template create<T>();
	}

	template<typename... Modules>
	template<template<typename...> typename T>
	constexpr auto Package<Modules...>::Create() const
	{
		return Inject().template create<T>();
	}

}
