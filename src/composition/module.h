#pragma once

#include <algorithm>
#include <array>
#include <boost/di.hpp>
#include <numeric>
#include <tuple>

// #include "boost/di/extension/providers/runtime_provider.hpp"

namespace synodic
{

	// TODO: Once reflection rolls around in C++23 derived classes can be deduced.
	template<typename Base>
	class ModuleConfig
	{
	public:
		constexpr ModuleConfig() = default;
		~ModuleConfig()			 = default;

		// TODO: virtual constexpr
		// virtual bool Priority() = 0;
	};

	// Default configuration implementation
	template<typename Base, typename Derived>
	class ImplementationConfiguration : public ModuleConfig<Base>
	{

	public:
		constexpr ImplementationConfiguration()
		{
			static_assert(false, "ImplementationConfiguration<Base, Derived> needs to be declared");
		};
		~ImplementationConfiguration() = default;
	};

	// Static module configuration struct
	struct StaticModuleParameters
	{
	};

	// Dynamic module configuration struct
	struct DynamicModuleParameters
	{
	};

	template<typename Interface, typename... Implementations>
	class StaticModule final
	{
		// Hide public construction
		template<typename InterfaceIn, typename... ImplementationsIn>
		friend auto CreateStaticModule(const StaticModuleParameters& parameters);

	public:
		[[nodiscard]] constexpr auto Inject() const
		{
			// Generate array of priorities
			constexpr std::array<uint8_t, sizeof...(Implementations)> priorities = {
				{ImplementationConfiguration<Interface, Implementations>().Priority()...}};

			// TODO: Create index range

			// TODO: Sort the indices by priority value, highest priority first

			// Find the first implementation with the highest priority
			constexpr auto itr =
				std::find_if(priorities.begin(), priorities.end(), [](auto priority) {
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
		template<typename T>
		[[nodiscard]] constexpr auto Create() const
		{
			return Inject().template create<T>();
		}

		// Allow creation of templated types.
		// Templated types must have a default type so that the proper implementation can be
		// injected
		template<template<typename...> typename T>
		[[nodiscard]] constexpr auto Create() const
		{
			return Inject().template create<T>();
		}

	private:
		explicit constexpr StaticModule(const StaticModuleParameters& parameters)
		{
		}
	};


	// TODO: Implement
	template<typename Interface>
	class DynamicModule final
	{

		template<typename InterfaceIn>
		friend auto CreateDynamicModule(const DynamicModuleParameters& parameters);

	public:
	private:
		explicit constexpr DynamicModule(const DynamicModuleParameters& parameters)
		{
		}
	};

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