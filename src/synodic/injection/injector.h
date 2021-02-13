#pragma once

#include "configuration.h"
#include "dependency.h"
#include "solver.h"
#include "type.h"

namespace cppaste
{

	// The injector hold the type information to be used for injection and also provides creation
	// utilities

	// TODO: Create the concept
	template<typename T, typename... Args>
	concept is_createable = true;


	template<is_configuration Configuration = StackConfiguration, typename... Dependencies>
	class Injector
	{

	public:
		// Constructor handling an Injector type
		template<typename... InjectorDependencies, typename... Args>
		explicit constexpr Injector(
			Injector<InjectorDependencies...>&& injector,
			Args&&... arguments);

		// Constructor handling a Type type
		template<typename DependencyType, ScopeType Scope, typename... Args>
		explicit constexpr Injector(Type<DependencyType, Scope>&& type, Args&&... arguments);

		// Creates a type T from the available type information given to the constructor
		template<is_createable T>
		constexpr auto Create() const -> T;

		// An overload of the `Create` function that works for templated types. If applicable, the
		//		second template parameter R will be resolved to a concrete type that fulfills the
		//		template T
		template<template<typename...> typename T, is_createable R = bool>	// TODO: Resolve type
		constexpr auto Create() const -> R;

	private:
		using DependencyTuple = std::tuple<Dependencies...>;
	};


	template<is_configuration Configuration, typename... Dependencies>
	template<typename... InjectorDependencies, typename... Args>
	constexpr Injector<Configuration, Dependencies...>::Injector(
		Injector<InjectorDependencies...>&& injector,
		Args&&... arguments)
	{
		auto subInjector = Injector(arguments...);
	}

	template<is_configuration Configuration, typename... Dependencies>
	template<typename DependencyType, ScopeType Scope, typename... Args>
	constexpr Injector<Configuration, Dependencies...>::Injector(
		Type<DependencyType, Scope>&& type,
		Args&&... arguments)
	{
		auto subInjector =  Injector(arguments...);
	}

	template<is_configuration Configuration, typename... Dependencies>
	template<is_createable T>
	constexpr auto Injector<Configuration, Dependencies...>::Create() const -> T
	{
		Solver<T> solver;
		return T();
	}

	template<is_configuration Configuration, typename... Dependencies>
	template<template<typename...> typename T, is_createable R>	 // TODO: Resolve type
	constexpr auto Injector<Configuration, Dependencies...>::Create() const -> R
	{
		return R();
	}

	// Export injector types

	// TODO: Export the actual types instead of placeholders
	using StaticInjector  = int;
	using DynamicInjector = int;

}
