#pragma once

#include <algorithm>
#include <array>
#include <boost/di.hpp>
#include <numeric>
#include <tuple>

// TODO: Dynamic Module
// #include "boost/di/extension/providers/runtime_provider.hpp"

namespace synodic
{

	///--------------------------------------------------------------------------------
	/// @brief Module Configuration for an Interface type
	///--------------------------------------------------------------------------------

	template<typename Base>
	class ModuleConfig
	{
	public:
		constexpr ModuleConfig() = default;
		~ModuleConfig()			 = default;

		// TODO: virtual constexpr
		// virtual bool Priority() = 0;
	};


	///--------------------------------------------------------------------------------
	/// @brief Default configuration implementation
	/// TODO: Once reflection rolls around in C++23 derived classes can be deduced.
	///--------------------------------------------------------------------------------

	template<typename Base, typename Derived>
	class ImplementationConfiguration : public ModuleConfig<Base>
	{

	public:
		constexpr ImplementationConfiguration();
		~ImplementationConfiguration() = default;
	};

	template<typename Base, typename Derived>
	constexpr ImplementationConfiguration<Base, Derived>::ImplementationConfiguration()
	{
		// TODO: Write Base and Derived type
		static_assert(false, "ImplementationConfiguration<Base, Derived> needs to be declared");
	}


	///--------------------------------------------------------------------------------
	/// @brief Static module configuration struct
	///--------------------------------------------------------------------------------

	struct StaticModuleParameters
	{
	};


	///--------------------------------------------------------------------------------
	/// @brief Dynamic module configuration struct
	///--------------------------------------------------------------------------------

	struct DynamicModuleParameters
	{
	};

	
	///--------------------------------------------------------------------------------
	/// @brief Compile time module
	///--------------------------------------------------------------------------------

	template<typename Interface, typename... Implementations>
	class StaticModule final
	{
		template<typename Base, typename... Derived>
		friend auto CreateStaticModule(const StaticModuleParameters& parameters);

	public:
		[[nodiscard]] constexpr auto Inject() const;

		template<typename T>
		[[nodiscard]] constexpr auto Create() const;
		template<template<typename...> typename T>
		[[nodiscard]] constexpr auto Create() const;

	private:
		explicit constexpr StaticModule(const StaticModuleParameters& parameters);
	};

	// Create injector for the class interface
	template<typename Interface, typename... Implementations>
	constexpr auto StaticModule<Interface, Implementations...>::Inject() const
	{
		// Generate array of priorities
		constexpr std::array<uint8_t, sizeof...(Implementations)> priorities = {
			{ImplementationConfiguration<Interface, Implementations>().Priority()...}};

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
			boost::di::bind<Interface>()
				.template to<std::tuple_element_t<index, std::tuple<Implementations...>>>());
	}

	// Creation of types
	template<typename Interface, typename... Implementations>
	template<typename T>
	constexpr auto StaticModule<Interface, Implementations...>::Create() const
	{
		return Inject().template create<T>();
	}

	// Creation of templated types
	template<typename Interface, typename... Implementations>
	template<template<typename...> typename T>
	constexpr auto StaticModule<Interface, Implementations...>::Create() const
	{
		return Inject().template create<T>();
	}

	template<typename Interface, typename... Implementations>
	constexpr StaticModule<Interface, Implementations...>::StaticModule(
		const StaticModuleParameters& parameters)
	{
	}


	///--------------------------------------------------------------------------------
	/// @brief Run time module
	/// TODO: Implement
	///--------------------------------------------------------------------------------

	template<typename Interface>
	class DynamicModule final
	{

		template<typename Base>
		friend auto CreateDynamicModule(const DynamicModuleParameters& parameters);

	public:
	private:
		explicit constexpr DynamicModule(const DynamicModuleParameters& parameters);
	};

	template<typename Interface>
	constexpr DynamicModule<Interface>::DynamicModule(const DynamicModuleParameters& parameters)
	{
	}


	// Creates a static module which is evaluated at compile time.
	// TODO: Remove Implementations template
	template<typename Interface, typename... Implementations>
	auto CreateStaticModule(const StaticModuleParameters& parameters)
	{
		// TODO: Pass along all types deriving from Interface c++23, reflection
		return StaticModule<Interface, Implementations...>(parameters);
	}

	// Creates a dynamic module which is evaluated at run time.
	template<typename Interface>
	auto CreateDynamicModule(const DynamicModuleParameters& parameters)
	{
		return DynamicModule<Interface>(parameters);
	}

}