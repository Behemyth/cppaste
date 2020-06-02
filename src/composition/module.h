#pragma once

#include <boost/di.hpp>
#include <boost/di/extension/injections/extensible_injector.hpp>
#include <boost/di/extension/providers/runtime_provider.hpp>
#include <concepts>

// Define EXPORTED for any platform
// TODO: wrap + use boost.dll functionality


namespace synodic
{

	// template<typename T>
	// concept static_interface = false;

	template<typename T>
	concept dynamic_interface = true;


	template<typename Interface>
	class Module
	{

	public:
		constexpr Module() : enabled_(true)
		{
		}

		boost::di::injector<Interface> Injector()
		{
			return boost::di::make_injector();//boost::di::extension::make_extensible(staticInjector_));
		}

		template<typename Fn, typename... Args> requires std::invocable<Fn, Args...>
		static void Bind(Fn&& function, Args&&... arguments)
		{
			
			//binder_ = [this, function, arguments...]() {
			//	boost::di::make_injector(
			//		binder_(), std::invoke(function, std::forward<Args>(arguments)...));
			//};
		}

		[[nodiscard]] bool Enabled() const
		{
			return enabled_;
		}

	private:
		// TODO
		// boost::di::extension::runtime_injector dynamicInjector_;

		// Each module shares the same compile-time set ∴ static
		//static inline boost::di::injector<Interface> staticInjector_ = boost::di::make_injector();

		//std::function<boost::di::injector<Interface>()> binder_;
		
		bool enabled_;
	};

	// Interface is templated over the interface type to provide automatic registration
	// segmented by the interface type
	// template<dynamic_interface Base>
	// class Interface
	//{
	//	template<typename T>
	//	friend class Module<T>;


	// public:
	//	Interface()			 = default;
	//	virtual ~Interface() = default;

	// private:
	//	constexpr static auto Bind()
	//	{
	//		boost::di::make_injector();
	//	}
	//};

	template<typename Base, typename Derived>
	class Implementation
	{
		
		static_assert(
			std::derived_from<Derived, Base>,
			"An Implementation's `Derived` type is required to publicly inherit from the `Base` "
			"type.");

	public:
		Implementation(bool dummy)
		{
			(void)registered_;
		};
		virtual ~Implementation() = default;

		virtual bool Instantiate() = 0;

		constexpr static bool Register()
		{
			//Module<Base>::Bind([]() -> auto {
			//	return boost::di::make_injector(boost::di::bind<Base>().template to<Derived>());
			//});

			return true;
		}

	protected:
		// Force class registration
		static constexpr bool registered_ = Register();
	};

}