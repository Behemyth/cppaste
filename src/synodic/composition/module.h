#pragma once

#include <algorithm>
#include <array>
#include <boost/di.hpp>
#include <concepts>
#include <tuple>

// TODO: Dynamic Module
// #include "boost/di/extension/providers/runtime_provider.hpp"

namespace synodic
{

	///--------------------------------------------------------------------------------
	/// @brief Module Configuration for an Interface type
	///--------------------------------------------------------------------------------

	template<typename Base, typename Interface>
	class ModuleInterface
	{
	public:
		using BaseType = Base;

		constexpr ModuleInterface() = default;
		// constexpr virtual ~ModuleInterface() = default;
	};

	///--------------------------------------------------------------------------------
	/// @brief Module Configuration for an Implementation type
	///--------------------------------------------------------------------------------

	template<typename Base, typename Derived, typename Implementation>
	class ModuleImplementation
	{
	public:
		using BaseType	  = Base;
		using DerivedType = Derived;

		constexpr ModuleImplementation() = default;
		// constexpr virtual ~ModuleImplementation() = default;

		// virtual uint8_t Priority() = 0;
	};


	///--------------------------------------------------------------------------------
	/// @brief User-facing Module configuration struct
	///--------------------------------------------------------------------------------

	struct ModuleParameters
	{
	};


	///--------------------------------------------------------------------------------
	/// @brief Compile time module
	///--------------------------------------------------------------------------------

	template<typename Interface, typename... Implementations>
	class Module final
	{
		template<typename Base, typename... Derived>
		friend constexpr auto CreateModule(const ModuleParameters& parameters);

	public:
		[[nodiscard]] constexpr auto Inject() const;

		template<typename T>
		[[nodiscard]] constexpr auto Create() const;
		template<template<typename...> typename T>
		[[nodiscard]] constexpr auto Create() const;

	private:
		explicit constexpr Module(const ModuleParameters& parameters);
	};

	// Create injector for the class interface
	template<typename Interface, typename... Implementations>
	constexpr auto Module<Interface, Implementations...>::Inject() const
	{

		// Generate array of priorities
		constexpr std::array<uint8_t, sizeof...(Implementations)> priorities = {
			{Implementations().Priority()...}};

		// TODO: Create index range

		// TODO: Sort the indices by priority value, highest priority first

		// Find the first implementation with the highest priority
		constexpr auto itr = std::find_if(priorities.begin(), priorities.end(), [](auto priority) {
			return priority > 0;
		});

		static_assert(
			itr != priorities.cend(),
			"There must be at least one implementation that gets instantiated.");

		// TODO: Replace std::distance with extracted index value
		constexpr auto index = std::distance(priorities.begin(), itr);

		// Bind a single type to the interface.
		return boost::di::make_injector(
			boost::di::bind<typename Interface::BaseType>()
				.template to<std::tuple_element_t<
					index, std::tuple<typename Implementations::DerivedType...>>>());
	}

	// Creation of types
	template<typename Interface, typename... Implementations>
	template<typename T>
	constexpr auto Module<Interface, Implementations...>::Create() const
	{
		return Inject().template create<T>();
	}

	// Creation of templated types
	template<typename Interface, typename... Implementations>
	template<template<typename...> typename T>
	constexpr auto Module<Interface, Implementations...>::Create() const
	{
		return Inject().template create<T>();
	}

	template<typename Interface, typename... Implementations>
	constexpr Module<Interface, Implementations...>::Module(const ModuleParameters& parameters)
	{
	}

	// Creates a static module which is evaluated at compile time.
	// TODO: Remove Implementations template
	template<typename Interface, typename... Implementations>
	constexpr auto CreateModule(const ModuleParameters& parameters)
	{
		// TODO: Pass along all types deriving from Interface c++23, reflection
		return Module<Interface, Implementations...>(parameters);
	}

}