#pragma once

#include <boost/di.hpp>

// Define EXPORTED for any platform
// TODO: wrap + use boost.dll functionality


namespace synodic
{

	// TODO: rename class to something more elegant
	// The primary purpose of the manager should be wrapping the modules `injects` with a single `inject` and create the app type
	template<class ... Modules>
	class ModuleManager
	{
				
	public:

		// TODO: Force all parameters to be of type Module<T>
		ModuleManager(Modules... modules);

		// TODO: Is a separate Create function needed?
		template<class T>
		constexpr T Create();
		
	private:

		
	};

	template<class... Modules>
	ModuleManager<Modules...>::ModuleManager(Modules... modules)
	{

	}

	template<class... Modules>
	template<class T>
	constexpr T ModuleManager<Modules ...>::Create()
	{
		auto injector = boost::di::make_injector(
			// TODO: Automatically wrap Modules types with Module<type>?
			// TODO: Let Interfaces derive from Module?
		);
		
		return injector.create<T>();
	}

	// TODO: Automatic registration of all Implementation types
	template<typename Interface>
	class Module
	{
	public:

		virtual ~Module() = 0;
		
	protected:

		// TODO: Make consteval instead of constexpr
		template<typename Implementation>
		constexpr auto Apply();
		
	private:
		Module();

		// TODO: static storage for Implementations? Is `di::extension::runtime_injector` required?
	};

	template<class Interface>
	Module<Interface>::Module()
	{
	}

	template<typename Interface>
	template<typename Implementation>
	constexpr auto Module<Interface>::Apply()
	{
		return boost::di::make_injector(boost::di::bind<Interface>().to<Implementation>());
	}

	
	// TODO: Is this class even necessary?
	template<typename Implementation, typename Interface>
	class ModuleImplementation
	{
	public:
		ModuleImplementation();
	
	private:
		
	};
	
	template<typename Implementation, typename Interface>
	ModuleImplementation<Implementation, Interface>::ModuleImplementation()
	{
		//return boost::di::make_injector(boost::di::bind<Interface>().to<Implementation>());
	}


	
}