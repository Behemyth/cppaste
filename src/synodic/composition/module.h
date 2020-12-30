#pragma once

#include <algorithm>
#include <array>
#include <boost/di.hpp>
#include <numeric>
#include <tuple>

// TODO: Dynamic ModuleBase
// #include "boost/di/extension/providers/runtime_provider.hpp"


namespace synodic
{

	// TODO: Move this into a relevant module

	///--------------------------------------------------------------------------------
	/// @brief A helper struct that provides function to cast the this* to the given type
	///		statically. Meant to be privately inherited
	///--------------------------------------------------------------------------------

	template<typename T>
	struct CRTP
	{
		T& DerivedType()
		{
			return static_cast<T&>(*this);
		}
		T const& DerivedType() const
		{
			return static_cast<T const&>(*this);
		}
	};


	///--------------------------------------------------------------------------------
	/// @brief ModuleBase Configuration for Interface types
	///--------------------------------------------------------------------------------

	template<class T>
	concept is_static_interface = true;

	template<is_static_interface Interface>
	class StaticInterface
	{
	public:
		consteval StaticInterface() = default;
	};

	template<class T>
	concept is_dynamic_interface = true;

	template<is_dynamic_interface Interface>
	class DynamicInterface
	{
	public:
		constexpr DynamicInterface() = default;
	};


	///--------------------------------------------------------------------------------
	/// @brief ModuleBase Configuration for Implementation types
	///--------------------------------------------------------------------------------

	template<class Derived, class Base>
	concept is_static_implementation =
		!std::is_same_v<Derived, Base> && std::derived_from<Base, StaticInterface<Base>> && requires
	{
		Base::template ValidateModule<Derived>();
	};

	template<is_static_interface Interface, is_static_implementation<Interface> Implementation>
	class StaticImplementation : public Interface
	{
	public:
		constexpr StaticImplementation() = default;
	};


	template<class Derived, class Base>
	concept is_dynamic_implementation =
		!std::is_same_v<Derived, Base> && std::derived_from<Base, DynamicInterface<Base>>;

	template<is_dynamic_interface Interface, is_dynamic_implementation<Interface> Implementation>
	class DynamicImplementation : public Interface
	{
	public:
		constexpr DynamicImplementation() = default;
	};


	///--------------------------------------------------------------------------------
	/// @brief User-facing ModuleBase configuration struct
	///--------------------------------------------------------------------------------

	struct ModuleParameters
	{
	};


	///--------------------------------------------------------------------------------
	/// @brief ModuleBase
	///--------------------------------------------------------------------------------

	template<typename Interface, typename... Implementations>
	class ModuleBase
	{

	public:
		[[nodiscard]] constexpr auto Inject() const;

		template<typename T>
		[[nodiscard]] constexpr auto Create() const;
		template<template<typename...> typename T>
		[[nodiscard]] constexpr auto Create() const;

	protected:
		explicit constexpr ModuleBase(const ModuleParameters& parameters);
	};

	// Create injector for the class interface
	template<typename Interface, typename... Implementations>
	constexpr auto ModuleBase<Interface, Implementations...>::Inject() const
	{

		// Generate array of priorities
		constexpr std::array<uint8_t, sizeof...(Implementations)> priorities = {
			{Implementations::Priority()...}};

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
		constexpr auto index = static_cast<size_t>(std::distance(priorities.begin(), itr));

		// Bind a single type to the interface.
		return boost::di::make_injector(
			boost::di::bind<Interface>()
				.template to<std::tuple_element_t<index, std::tuple<Implementations...>>>());
	}

	// Creation of types
	template<typename Interface, typename... Implementations>
	template<typename T>
	constexpr auto ModuleBase<Interface, Implementations...>::Create() const
	{
		return Inject().template create<T>();
	}

	// Creation of templated types
	template<typename Interface, typename... Implementations>
	template<template<typename...> typename T>
	constexpr auto ModuleBase<Interface, Implementations...>::Create() const
	{
		return Inject().template create<T>();
	}

	template<typename Interface, typename... Implementations>
	constexpr ModuleBase<Interface, Implementations...>::ModuleBase(
		const ModuleParameters& parameters)
	{
	}


	///--------------------------------------------------------------------------------
	/// @brief StaticModule
	///--------------------------------------------------------------------------------

	template<is_static_interface Interface, is_static_implementation<Interface>... Implementations>
	class StaticModule final : public ModuleBase<Interface, Implementations...>
	{

		template<is_static_interface Base, is_static_implementation<Base>... Derived>
		friend constexpr auto CreateModule(const ModuleParameters& parameters);

		explicit constexpr StaticModule(const ModuleParameters& parameters);
	};

	template<is_static_interface Interface, is_static_implementation<Interface>... Implementations>
	constexpr StaticModule<Interface, Implementations...>::StaticModule(
		const ModuleParameters& parameters) :
		ModuleBase<Interface, Implementations...>(parameters)
	{
	}

	///--------------------------------------------------------------------------------
	/// @brief DynamicModule
	///--------------------------------------------------------------------------------

	template<
		is_dynamic_interface Interface,
		is_dynamic_implementation<Interface>... Implementations>
	class DynamicModule final : public ModuleBase<Interface, Implementations...>
	{

		template<is_dynamic_interface Base, is_dynamic_implementation<Base>... Derived>
		friend constexpr auto CreateModule(const ModuleParameters& parameters);

		explicit constexpr DynamicModule(const ModuleParameters& parameters);
	};

	template<
		is_dynamic_interface Interface,
		is_dynamic_implementation<Interface>... Implementations>
	constexpr DynamicModule<Interface, Implementations...>::DynamicModule(
		const ModuleParameters& parameters) :
		ModuleBase<Interface, Implementations...>(parameters)
	{
	}


	template<class T>
	concept is_static_module = std::derived_from<T, StaticModule<T>>;

	template<class T>
	concept is_dynamic_module = std::derived_from<T, DynamicModule<T>>;

	template<class T>
	concept is_module = is_static_module<T> || is_dynamic_module<T>;


	///--------------------------------------------------------------------------------
	/// @brief Interface factory functions
	///--------------------------------------------------------------------------------

	// Creates a static module which is evaluated at compile time.
	// TODO: Remove Implementations template
	template<is_static_interface Interface, is_static_implementation<Interface>... Implementations>
	constexpr auto CreateModule(const ModuleParameters& parameters)
	{
		// TODO: Pass along all types deriving from Interface c++23, reflection
		return StaticModule<Interface, Implementations...>(parameters);
	}

	// Creates a dynamic module which is evaluated at compile time.
	// TODO: Remove Implementations template
	template<
		is_dynamic_interface Interface,
		is_dynamic_implementation<Interface>... Implementations>
	constexpr auto CreateModule(const ModuleParameters& parameters)
	{
		// TODO: Pass along all types deriving from Interface c++23, reflection
		return DynamicModule<Interface, Implementations...>(parameters);
	}

}