#pragma once

#include <boost/di.hpp>
#include <concepts>


// Define EXPORTED for any platform
// TODO: wrap + use boost.dll functionality


namespace synodic
{

	// template<typename T>
	// concept static_interface = false;

	template<typename T>
	concept dynamic_interface = true;

	template<typename Derived, typename Base>
	concept injector = true;

	template<typename Base, typename Derived>
	class Injector
	{

		template<typename>
		friend class Module;
		
		static_assert(
			std::derived_from<Derived, Base>,
			"An Injector's `Derived` type is required to publicly inherit from the `Base` "
			"type.");

	public:
		constexpr Injector() = default;
		virtual ~Injector()	 = default;

		virtual bool Instantiate() = 0;

	private:
		boost::di::injector<Base> Inject()
		{
			return boost::di::make_injector(boost::di::bind<Base>().template to<Derived>());
		}
	};


	struct ModuleParameters
	{
	};

	template<typename Interface>
	class Module
	{

	public:
		// TODO: Once reflection rolls around in C++23, `implementations` can be deduced.
		template<typename... Implementations>
		explicit constexpr Module(
			const ModuleParameters& parameters,
			Implementations&... implementations) :
			staticInjector_(boost::di::make_injector(implementations.Inject()...)),
			enabled_(true)
		{
		}

		constexpr auto Inject()
		{
			return staticInjector_;
		}

		[[nodiscard]] bool Enabled() const
		{
			return enabled_;
		}

	private:
		boost::di::injector<Interface> staticInjector_;

		bool enabled_;
	};
}