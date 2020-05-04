#pragma once

#include <boost/di.hpp>
#include <concepts>
#include <memory>

#include "boost/di/extension/providers/runtime_provider.hpp"

// Define EXPORTED for any platform
// TODO: wrap + use boost.dll functionality


namespace synodic
{

	template<typename T>
	concept interface = requires
	{
		true;
	};

	// class Interface
	//{

	//};

	//
	// template<typename Implementation, Interface Interface>
	// class Implementation
	//{
	// public:
	//	Implementation();

	// private:
	//};

	// template<typename Implementation, typename Interface>
	// Implementation<Implementation, Interface>::Implementation()
	//{
	//	// return boost::di::make_injector(boost::di::bind<Interface>().to<Implementation>());
	//}

	template<interface Interface>
	class ModuleInterface
	{
	public:
		template<class Implementation>
		ModuleInterface() :
			staticInjector_(
				boost::di::make_injector(boost::di::bind<Interface>().to<Implementation>()))
		{
		}

	private:
		boost::di::extension::runtime_injector dynamicInjector_;
		boost::di::injector<Interface> staticInjector_;
	};

	template<class Implementation, interface Interface>
	class Module : Interface
	{

	public:
		Module() :
			ModuleInterface(boost::di::bind<Interface>().to<Implementation>())
		{
		}
		~Module() = default;

	protected:
	private:
		// Force class registration
		constexpr static bool registered_;

	};

	template<class Implementation, interface Interface>
	bool Module<Implementation, Interface>::registered_ = true;

	template<interface Interface>
	std::unique_ptr<Interface> CreateModule()
	{
		// auto injector = boost::di::make_injector(
		//	boost::di::bind<Interface>().to<Implementation>()
		//);
		// return staticInjector_.template create<Interface>();
		//
		return nullptr;
	}

}