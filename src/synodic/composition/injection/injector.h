#pragma once

#include <type_traits>

#include "dependency.h"

namespace synodic
{

	template<typename T, typename... Args>
	concept is_createable = std::is_constructible_v<T, Args...>() /*&&
							std::is_constructible<Args, type_traits::ctor_traits<Args>...>()*/
		;

	template<typename T>
	concept is_boundable = true;


	template<typename Configuration, typename... Dependencies>
	class Injector
	{

	public:
		template<typename... SubDependencies, typename... Args>
		Injector(Injector<SubDependencies>&& injector, Args... arguments);

		template<typename Interface, typename Implementation, typename... Args>
		Injector(Dependency<Interface, Implementation>&& injector, Args... arguments);


		template<is_createable T>
		T Create() const;

		template<template<typename...> typename T, is_createable R = bool>
		R Create() const;

	private:
	};


	template<typename Configuration, typename... Dependencies>
	template<typename... SubDependencies, typename... Args>
	Injector<Configuration, Dependencies...>::Injector(Injector<SubDependencies>&& injector, Args... arguments)
	{
		Injector(arguments...);
	}

	template<typename Configuration, typename... Dependencies>
	template<typename Interface, typename Implementation, typename... Args>
	Injector<Configuration, Dependencies...>::Injector(
		Dependency<Interface, Implementation>&& injector,
		Args... arguments)
	{
		Injector(arguments...);
	}

	template<typename Configuration, typename... Dependencies>
	template<is_createable T>
	T Injector<Configuration, Dependencies...>::Create() const
	{
		return T();
	}

	template<typename Configuration, typename... Dependencies>
	template<template<typename...> typename T, is_createable R>	 // TODO: Resolve type
	R Injector<Configuration, Dependencies...>::Create() const
	{
		// Top level type construction routine
		// R contains the isolated type to start traversing
		return R();
	}

}
